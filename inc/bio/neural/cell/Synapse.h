/*
 * This file is a part of the Biology project by eons LLC.
 * Biology (aka Develop Biology) is a framework for approaching software
 * development from a natural sciences perspective.
 *
 * Copyright (C) 2022 Séon O'Shannon & eons LLC
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include "bio/cellular/Cell.h"
#include "bio/common/ReturnCodes.h"
#include "bio/common/Time.h"
#include "common/Types.h" //neural types

/**
This is unneeded here, but it should be added to all necessary files via synapse.h -> neuron.h -> brain.h
*/
#include "bio/common/Functions.h"

namespace bio {
namespace neural {

class Neuron;

/**
Synapses take data from one neuron, the "source", and make it available to another neuron, the "target".
In order to remove data that was previously added while simultaneously allowing more data to be pushed through *this, a history of the data in *this must be kept.
This history need only be a single value.
The reason for this is that if a new datum is pushed through this that would overwrite data that has not been added, the previous, unadded data are no longer useful and can be freely overwritten.
Similarly, if the old datum always represents what is currently added to to the target, there is no need to maintain a history of more than this single datum.
For example a Synapse that passes a float might have: float m_unprocessed float m_processed This type of 2 value system will be a common motif among bio::Synapses. The unprocessed, or "new", value is a value from the source that has not yet been added to the target. Once this value is added, it becomes processed, or "old".
In general, Synapses look like: [SOURCE]----[*this]---->[TARGET]
When sending data, [SOURCE]-->ProcessOutgoing()-->Update() [SOURCE]-->ExpireSignal() (totally optional)
When receiving data, ProcessAddition()-->[TARGET] (target calls ProcessAddition, which affects ProcessRemoval()-->[TARGET] the target in some way)
*/

class Synapse :
	public cellular::Cell,
	public Named< SynapseId >
{
public:

	/**
	When creating template Synapses for use with ConfigureFor (below), use the default constructor.
	Creating synapses outside of Neuron::ConnectTo(...) is not supported
	If you do something weird and don't StoreSynapse(*this) with the Brain, and decide to use Neuron::ConnectTo anyway, make sure you've at least Register()ed *this with the Brain's GetApplicator(). However, he behavior of such code is undefined, regardless.
	*/
	Synapse(Name name = SynapseTracker::InvalidName());

	Synapse(const Synapse& rhs);

	virtual ~Synapse();

	//START: Required overrides

	/**
	*this may be cloned in order to create a new synapse based on the configuration in *this.
	This should only be done before *this is configured for neuronal use.
	To use *this between any pair of neurons, use ConfigureFor() (below).
	The synapse RETURNED will not be configured for any specific neurons.
	*/
	virtual Synapse* Clone() const;

	/**
	Override this method to RETURN whether or not your Synapse's value is equivalent to zero (e.g. zero vector, empty set, etc.)
	If using an unprocessed -> processed scheme, this refers to the unprocessed value.
	This is used by ShouldBeProcessed
	FIXME: Decide if this should be protein-based.
	*/
	virtual bool IsZero() const
	{
		return false;
	}

	/**
	Used for Expire signals, where it is useful to forget what the processed data was.
	*/
	virtual void MakeZero()
	{
	}

	/**
	Called after ProcessOutgoing calls Update(time)
	RETURNS: the timestamp that Update will use to set the time at which *this should be added.
	For example, if the source says this should be added at the current time, and *this has some sort of delay (perhaps from learning), this method will return the current time plus the indicated delay.
	*/
	virtual Timestamp PrepareForAddition(Timestamp addDatumAt)
	{
		return addDatumAt;
	}

	/**
	Called directly after *this is added, PrepareForRemoval does whatever is necessary for ProcessRemoval to work.
	For example, if *this uses a New and Old value, this method would set Old to New and zero out New.
	*/
	virtual void PrepareForRemoval()
	{
	}

	/**
	Both the following methods modify the receiver according to the purpose of *this (e.g. incrementing/decrementing the receiver's value)
	Removal is always the opposite of Addition such that: p = Removal(Addition(p)) && p = Addition(Removal(p))
	EXTREME care must be taken when the above is not possible.
	*/
	virtual ReturnCode ProcessAddition();

	virtual ReturnCode ProcessRemoval();

	/**
	If you would like to do something after *this has been fully added to the target, put that code in the appropriate Post____() method.
	*/
	virtual ReturnCode PostAddition();

	virtual ReturnCode PostRemoval();

	//END: Required Overrides

	//START: Functional Overrides

	/**
	These can all be substituted for proteins, if so desired.
	*/

	/**
	The Neuron interface is how Neurons will interact with *this
	/**This includes sending and receiving data through *this
	//All Neuron interface methods will return a ReturnCode indicative of success or error.

	/**
	ProcessOutgoing takes information from the source Neuron and makes it available to the target Neuron.
	The data accessed by *this is copied and the copy is stored in *this for access by the target Neuron. This allows the source neuron to continue editing the data it passed, as well as for *this to manipulate data from the source without affecting other Synapses.
	If *this is going to send data to the target, ProcessOutgoing MUST call Update(time to add the given data)
	*/
	virtual ReturnCode ProcessOutgoing()
	{
		if (m_processOutgoing)
		{
			return StandardActivate(m_processOutgoing); //MUST CALL UPDATE!
		}
		else
		{
			if (!IsZero() || AreZerosAllowed())
			{
				//This could be where you set your synapse's values, or you can use PrepareForAddition. Both of these locations are logically the same.

				Update(time::GetCurrentTimestamp());
				return ret::Success();
			}
		}
		return ret::NoErrorNoSuccess();
	}

	/**
	This method will allow the target to give some information back to *this
	Such a call may be useful in learning, etc.
	*/
	virtual void Callback(void* arg = NULL);

	//END: Functional Overrides

	//START: Additional Overrides

	/**
	RETURNS: true if *this should be added at the current time, and the value of the data are >0 || AreZerosAllowed is true. false otherwise
	This method does not need to be overwritten. See the IsZero, above.
	*/
	virtual bool ShouldBeAdded() const;

	/**
	RETURNS: true if *this has expired.
	*/
	virtual bool ShouldBeRemoved() const;

	/**
	Called when *this is configured for use between the two given neurons.
	This will be called on a newly created Clone() of *this.
	*/
	virtual void AddtionalConfiguration(
		const Neuron* source,
		Neuron* target
	)
	{
		if (source && target)
			return;
	} //Just to avoid compiler warnings.

	/**
	For rare circumstances, where *this might need to be re-added
	TODO: is there a better solution?
	*/
	virtual void ReReady()
	{
	}

	//END: Additional Overrides

	//START: Configuration/Settings

	/**
	RETURNS: "GetSourceName() to GetTargetName(): GetName()"
	*/
	std::string Identity() const;

	/**
	Effects state what changes *this will make to the downstream neuron.
	These should be added by either the Protein Rules in *this or the ctor of the child class of *this.
	*/
	void AddEffect(const AspectId effect);

	bool DoesAffect(const AspectId aspect);

	AspectIds GetEffects() const;

	/**
	The following methods control whether or not a datum with value 0 should be allowed to propagate through *this.
	It is up to the methods within *this (or children) to respect this.
	Zeros are NOT allowed by default.
	*/
	bool AreZerosAllowed() const;

	void SetZerosAllowed(bool allowed);

	/**
	Prevents *this from being removed due to expiration time.
	NOTE: this has no effect on ExpireSignal calls.
	By default, Synapses will NOT expire.
	*/
	bool DoesExpire() const;

	void SetDoesExpire(const bool does);

	/**
	RETURNS: the time that *this should remain added for.
	*/
	TimeMS GetExpiration() const;

	void SetExpiration(TimeMS lastFor);

	/**
	Sets the expiration time such that *this will expire at the given time.
	Has no effect if the given time is later than the current time.
	*/
	void ExtendExpirationUntil(Timestamp timeToExpire);

	/**
	If your synapse defies the typical addition / removal structure (changes applied by removing old and adding new), then this method will allow your synapse to bypass the is-already-added check imposed by the target Neuron.
	This is useful for synapses that build up over time.
	*/
	void AllowMultipleAdditions(bool allow = true);

	bool AreMultipleAdditionsAllowed() const;

	/**
	Inverse of above.
	*/
	void AllowMultipleRemovals(bool allow = true);

	bool AreMultipleRemovalsAllowed() const;

	//END: Configuration/Settings

	//START: Additional Access

	/**
	RETURNS: what is connected to *this, the source.
	*/
	const Neuron* GetSourceNeuron() const;

	Name GetSourceName() const;

	NeuronId GetSourceId() const;

	/**
	RETURNS: what *this is connected to, the target.
	*/
	Neuron* GetTargetNeuron();

	const Neuron* GetTargetNeuron() const;

	Name GetTargetName() const;

	NeuronId GetTargetId() const;

	/**
	RETURNS: the timestamp at which *this should be added to the target
	*/
	Timestamp GetTimeToAdd() const;

	/**
	RETURNS: the last time Update() was called.
	*/
	Timestamp GetTimeLastUpdated() const;

	/**
	IsAdded keeps track of whether or not the data from *this are present within the target neuron.
	*/
	bool IsAdded() const;

	/**
	RETURNS: whether or not *this has information for the target.
	*/
	bool IsReady() const;

	//END: Additional Access

	//START: Neural framework methods

	//You don't need to worry about these.

	/**
	Tells the downstream neuron not to try to add *this.
	*/
	void NoLongerReady();

	/**
	This should be called with ProcessOutgoing
	When this is called, ShouldBeProcessed will return true on or after addDatumAt and before addDatumAt + timeout See ShouldBeProcessed for more info.
	IMPORTANT: when overriding this, make sure to call this method too!
	*/
	void Update(Timestamp addDatumAt);

	/**
	Removes data from the target
	*/
	void ExpireSignal();

	/**
	Calls PrepareForRemoval()
	When this is called, ShouldBeProcessed will return false
	*/
	void Added(); //called when data is added, usually by ProcessAddition

	void Removed(); //called when data is removed, usually by ProcessRemoval

	/**
	ConfigureFor is used by Neuron::ConnectTo to clone *this such that it may be added between the given source and target
	This will call target->AddIncomingSynapse
	RETURNS: a clone of *this but with the given target
	*/
	Synapse* ConfigureFor(
		const Neuron* source,
		Neuron* target
	) const;

	//END: Neural framework methods

	//START: cellular::Cell overrides

	virtual void TranscribeTopLevelProteins();

	virtual void FoldAllProteins(void* arg = NULL); //arg = Sender

	//END: cellular::Cell overrides

protected:
	/**
	Proteins. See cellular::Cell.h for more details
	*/
	cellular::Protein* m_callback;
	cellular::Protein* m_processOutgoing;
	cellular::Protein* m_processAddition;
	cellular::Protein* m_postAddition;
	cellular::Protein* m_processRemoval;
	cellular::Protein* m_postRemoval;
	cellular::Protein* m_expireSignal;

	const Neuron* m_source;
	Neuron* m_target;

	/**
	Used when the target object is unknown or has not been created yet.
	*/
	NeuronId m_targetID;

	AspectIds m_effects;

	SynapseId m_id;

	bool m_isReady;
	bool m_isAdded;
	bool m_areZerosAllowed;
	bool m_doesExpire;
	bool m_allowMultipleAdditions;
	bool m_allowMultipleRemovals;

	/*
	TODO: Do we want to make processing immediately an option?
	*/

	TimeMS m_expiration; //how long until *this should be removed
	Timestamp m_addAt;
	Timestamp m_addedAt;
	Timestamp m_lastUpdated;
};

} //neural namespace
} //bio namespace
