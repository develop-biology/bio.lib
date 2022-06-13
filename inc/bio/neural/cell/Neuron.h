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

#include <bio.h>
#include "common/Class.h"
#include "Aspect.h"
#include "common/Types.h" //neural types

namespace bio {
namespace neural {

class Synapse;

class Personality;

/**
 * Any class that deals with Synapses should also deal with Neurons, so this is okay to have here.
 */
typedef std::vector< Synapse* > Synapses;

/**
 * Neurons perform critical functions in sensory processing, learning and memory, and actuation (e.g. muscle contraction).
 * The exact function of a Neuron is left up to the user to define.
 * A Neuron becomes active when ExciteTrigger() is true. This can be determined on Poll() or whenever impulses are received.
 */
class Neuron :
	public neural::Class< Neuron >,
	public cellular::Cell
{
public:

	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(neural, Neuron
	)

	BIO_DEFAULT_IDENTIFIABLE_CONSTRUCTORS(
		neural,
		Cell,
		&neural::NeuronPerspective::Instance(),
		filter::Neural()
	)

	/**
	 * Copies all values in *this.
	 * Synapses are NOT copied.
	 * @param other
	 */
	Neuron(const Neuron& other);

	//START: Recommended overrides

	/**
	These are the most important methods for every Neuron.
	However, they may not all be required for your Neuron.
	*/

	/**
	RETURNS: whether or not *this should be active.
	It is up to each individual neuron to determine what is appropriate here.
	*/
	virtual bool ExciteTrigger() const
	{
		return false;
	}

	/**
	Aspects are key values within a neuron.
	They are defined in Aspect.h
	Resetting aspects of *this, when there are no changes affecting it, should effectively remove any rounding errors that have accumulated.
	You must implement this for each aspect you create.
	This will only be called if there are no incoming synapses affecting the aspect and if the aspect should be reset (see SetAspectReset(), below)
	IMPORTANT: To use this, you must also call CreateAspect(...)
	*/
	virtual void Reset(const AspectId aspect)
	{
		if (aspect)
			return;
	} //just to avoid compiler warnings.

	/**
	The above can also be accessed through the fc::Impulse methods, below.
	*/

	/**
	OnPoll carries out time-based requirements of the specific neuron.
	For example, a sensor Neuron might use OnPoll() to check the status of whatever hardware it is reading.
	OnPoll will often contain neuron-specific code and handle the propagation of data through the neural framework. The reason data propagation is handled here is that incoming impulse expiration is handled directly before this is called. Thus, this method will have the most accurate view of the state of the Neuron (unless another method is implemented to handle incoming data).
	Poll order is: 1. Confirm *this should be Polled, based on GetPollPeriod()
	 2. PrePoll
	 3. Internal Neuron update (including checking all incoming synapses)
	 4. OnPoll
	*/
	virtual void OnPoll();

	/**
	Activated at the top of Poll(), before internal stuffs and before OnPoll().
	*/
	virtual void PrePoll();

	//END: Recommended overrides

	/**
	Learn tells the neuron when to apply any learning operations.
	This is a no-op unless the Learning plasmid is used.
	While this method is virtual, it is NOT advised you override it, unless you know exactly what you are doing and have your own learning system to integrate with.
	----WHEN TO USE----
	Consider what you want your system to learn about.
	If you want to learn/predict when a neuron will be active, call this when the neuron becomes active. NOTE that you might not want this done on RisingEdgeImpulse or some other location that could be just as easily triggered by any incoming excitatory synapse as innate activity. For example, sensors call Learn when they receive data from a driver That way, any neurons connected to them will start predicting real world inputs (as opposed to other neural activity).
	For learning to other inputs, Learn will be called on addition of a "DriveLearning" synapse. For more info, see the mod_learning-neural docs.
	*/
	virtual void Learn();

	//START: Impulses

	/**
	______Impulse() methods are called by DetermineAndCallImpulseTriggers().
	It is up to children of *this to determine what these methods do.
	NOTE: Impulses should not set the FiringCondition, which is returned by GetReasonForFiring. Only DetermineImpulseTriggers() will do that.
	If you would like to make your own Impulses, use the CreateImpulse() method and populate a cellular::Protein of the given Name.
	*/

	/**
	The default Impulses are: "RisingEdge" Called if newly activated
	"Continuing" Called if active and previously active
	"FallingEdge" Called when *this stops being active
	*/

	/**
	This uses ExciteTrigger to set the FiringCondition of *this and calls whatever _____Impulse() method is appropriate. See fc::Trigger, below for more on how those calls are made.
	RETURNS whether or not *this should be active.
	*/
	bool DetermineAndCallImpulseTriggers();

	/**
	Causes DetermineAndCallImpulseTriggers() to start at RisingEdgeImpulse(), if it would otherwise call ContinuingImpulse().
	*/
	void StartAtRisingEdge();

	/**
	What "active" means is up to each neuron to determine for itself. For example, a sensor may be active when a significant amount of stimulus is received, whereas a behavior may be active whenever it makes a call to an actuator.
	When a Neuron is not sending continuing impulses, it will be considered active until proven otherwise (e.g. falling edge is sent). In such a case, GetTimeLastActive() will return the current timestamp.
	NOTE: if a neuron is not sending continuing impulses, ActiveUntil will not trigger a falling edge at the indicated time. The methods below are for reporting only and do not affect operation.
	For how to keep a neuron active for a time after an initial stimulus, see PersistFor or PersistUntil (below)
	*/
	void ActiveNow(); //Sets the time active to the current timestamp
	void ActiveUntil(Timestamp time); //Sets the time active to any time

	/**
	RETURNS: last time *this was active
	See ActiveNow, above.
	*/
	Timestamp GetTimeLastActive() const;

	/**
	RETURNS: whether or not *this is active.
	*/
	bool IsActive() const;

	BIO_ID_WITH_TRACKER(FiringCondition, uint8_t
	)

	/**
	Default FiringConditions are as follows: */
	struct fc
	{
	public:
		static FiringConditionId None();

		static FiringConditionId Any();

		static FiringConditionId RisingEdge();

		static FiringConditionId Continuing();

		static FiringConditionId FallingEdge();

		/**
		Firing Condition Impulses are simple function pointer wrappers that allow different functions to be called upon different triggers.
		fc::Impulses can be enabled/disabled for ease of use.
		fc::Impulses should specify the RETURN value for their caller, DetermineAndCallImpulseTriggers(), via the bool shouldBeActive.
		Impulses use Name instead of ProteinId as they need to be created before proteins can be added to them.
		*/
		class Impulse
		{
		public:
			Impulse(
				FiringConditionId trigger = None(),
				Name impulseName = ""
				euron* caller = NULL,
				bool shouldBeActive = true
			)
				:
				m_trigger(trigger),
				m_impulseName(impulseName),
				m_impulse(NULL),
				m_caller(caller),
				m_shouldBeActive(shouldBeActive)
			{
			}

			FiringConditionId GetTrigger() const
			{
				return m_trigger;
			}

			Name GetImpulseName() const
			{
				return m_impulseName;
			}

			cellular::Protein* GetImpulseProtein()
			{
				return m_impulse;
			}

			void SetCaller(Neuron* caller)
			{
				m_caller = caller;
				m_impulse = caller->GetProtein(m_impulseName);
			}

			bool Send()
			{
				if (m_impulse->IsEnabled())
				{
					m_caller->StandardActivate(m_impulse);
				}
				return m_shouldBeActive;
			}

		protected:
			FiringConditionId m_trigger;
			Name m_impulseName;
			cellular::Protein* m_impulse;
			Neuron* m_caller;
			bool m_shouldBeActive;
		};

		typedef std::vector <Impulse> Impulses;
	};

	/**
	This updates all Impulses to use *this as their caller
	This must be called whenever *this is copied, cloned, etc.
	*/
	void UpdateImpulseCallers();

	/**
	Creates an Impulse and adds it to *this
	This will create a new top-level-protein, if one does not exist.
	*/
	void CreateImpulse(
		FiringConditionId trigger,
		Name impulseName,
		bool shouldBeActive = true
	);

	/**
	RETURNS the impulse object that calls the given impulseName
	NOTE that impulseName is the Name of a Protein in *this.
	*/
	fc::Impulse* GetImpulse(Name impulseName);

	/**
	Removed the given Impulse from *this
	NOTE: this does not remove the Protein with the given name from *this. If you would like to remove the protein as well, please use RemoveProtein(GetProtein(impulseName))
	*/
	void RemoveImpulse(Name impulseName);

	void EnableImpulse(Name impulseName);

	void DisableImpulse(Name impulseName);

	bool IsImpulseEnabled(Name impulseName);

	/**
	RETURNS: the FiringCondition that caused *this to be active.
	This is ONLY set by DetermineAndCallImpulseTriggers.
	If you would like to control this manually, please adjust your ExciteTrigger so that DetermineAndCallImpulseTriggers gives the results you desire.
	*/
	FiringConditionId GetReasonForFiring() const;

	/**
	RETURNS: whether or not *this is "active" due to persistence.
	*/
	bool IsPersisting(const Timestamp now) const;

	bool IsPersisting() const
	{
		return IsPersisting(time::GetCurrentTimestamp());
	}

	//END: Impulses

	//START: Configuration/Settings

	/**
	Sets whether or not *this will attempt to call Reset on the given aspect.
	*/
	void SetAspectReset(
		const AspectId aspect,
		bool shouldReset
	);

	/**
	Neurons may stay active (persist) for a given amount of time after their excite trigger.
	Calling Persist___() will result in *this remaining active for the indicated time.
	Once *this is no longer persisting, FallingEdge will be called.
	NOTE: Persistence requires the continuing impulses only set Active___() if and only if ExciteTrigger() is true (as should always be the case).

	FIXME: PersistUntil needs to only be applied once.
	FIXME: Is this even a useful feature?
	*/
	void PersistFor(const TimeMS ms); //Always applied
	void PersistUntil(const Timestamp time); //Sets PersistFor with (time-now).

	//END: Configuration/Settings

	//START: Neuropil Interaction

	/**Neurons are stored and accessed through Neuropils.
	Thus, to add an extra layer of helpful access, Neurons can be added to multiple containers.
	For example, if you want to ignore all sensors related to light, you could create a "light" Neuropil and add all the appropriate sensors there. Then, the logic that handles ignoring light could check if the Neuron is in the "light" container, regardless of it being in the "sensor" container or anywhere else.
	*/

	/**
	Returns Name() or the name of the first neuropil that has access to this if Name() is too long (too long is >20 chars)
	*/
	std::string PrettyID() const;

	/**
	RETURNS: whether or not *this has been added to a neuropil of the given Id
	*/
	bool DoesNeuropilHaveAccess(NeuropilId neuropilId) const;

	NeuropilIds GetNeuropilsWithAccess() const;

	unsigned int GetNumNeuropilsWtihAccess() const;

	//END: Neuropil Interaction

	//START: Aspect interaction

	//These uses AspectId as a means of enforcing the aspect's existence within *this.

	/**
	Tells *this to keep track of a new AspectId.
	*/
	void CreateAspect(
		AspectId aspect,
		bool shouldReset = true
	);

	/**
	RETURNS: Whether or not the given aspect has shouldReset as true.
	*/
	bool ShouldResetAspect(const AspectId aspect) const; //false if not found.

	/**
	RETURNS: number of Synapses affecting the given aspect that are added UINT_MAX if invalid
	*/
	unsigned int GetCountFor(const AspectId aspect) const;

	/**
	calls CheckForReset on all Aspects
	*/
	void ResetAllPossibleAspects();

	/**
	RETURNS: all Synapses that affect the given aspect.
	*/
	Synapses GetSynapsesFor(const AspectId aspect) const;

	//END: Aspect interaction

	//START: Synapses

	/**
	Utilizes the Synapse::ConfigureFor method to create a new Synapse between *this and the target
	The target is whatever you want *this to send data to.
	The synapseType specifies how you want to send that data.
	The synapseType can be as simple as a Synapse* with some Traits added or as complex as a custom derived class.
	RETURNS: the configured synapse that has been added to *this.
	If a synapse to the same target and of the same ID already exists, the existing synapse will be RETURNED but not re-added.
	*/
	Synapse* ConnectTo(
		Neuron* target,
		const Synapse* synapseType
	);

	/**
	Each Neuron can have Synapses to and from it that allow data to be sent and received by other Neurons.
	For more information on Synapses, see Synapse.h
	*/

	/**
	Adds a new Synapse to receive data from if it does not already exist.
	*/
	void AddIncomingSynapse(Synapse* synapse);

	/**
	Removes an incoming Synapse to *this if it can be found.
	*/
	void RemoveIncomingSynapse(Synapse* synapse);

	/**
	RETURNS: all Synapses to *this.
	*/
	Synapses GetAllIncomingSynapses() const;

	/**
	RETURNS: the number of incoming synapses.
	*/
	unsigned int GetIncomingSynapsesSize() const;

	/**
	RETURNS: the number of incoming synapses with the given traits.
	*/
	unsigned int GetIncomingSynapsesSize(const cellular::Cell::Traits& selection) const;

	/**
	RETURNS the number of synapses that are currently added and have the specified traits.
	Partial matches RETURN true.
	*/
	unsigned int GetNumAddedSynapsesOf(const cellular::Cell::Traits& selection) const;

	/**
	Same as above, but for a specific AspectId.
	*/
	unsigned int GetNumAddedSynapsesOf(const AspectId aspect) const;

	/**
	RequestProcessingOf checks if the given synapse should be added (or removed) and will perform the necessary proteins.
	The only reason to use this method, as opposed to letting Poll check the given synapse, is to have it be processed before another synapse i.e. this method gives you more control over the order in which synapses are processed. With that said, Poll will still process synapses in the order they are added, it will just skip any synapses that have already been processed, either by a previous Poll or by this method.
	*/
	void RequestProcessingOf(Synapse* synapse);

	/**
	AddSynapse is used to process any incoming data.
	WARNING: USE WITH CAUTION!! This forcibly adds the synapse! if the Synapse should not be added (i.e. ShouldBeRemoved is true), it will be removed on the next clock tick.
	*/
	void AddSynapse(Synapse* toProcess);

	/**
	RemoveSynapse simply removes any information from the given synapse. It is used to unprocess or cleanup any data from that synapse For example, if AddSynapse() adds the value of an input to *this, RemoveSynapse might subtract the value of that input from *this
	WARNING: USE WITH CAUTION!! This forcibly removes the synapse!
	*/
	void RemoveSynapse(Synapse* toProcess);

	/**
	These do what you would expect.
	*/
	void RemoveSynapseTo(
		Neuron* target,
		SynapseId cid
	);

	void RemoveSynapsesTo(Neuron* target);

	void RemoveSynapsesTo(
		Neuron* target,
		molecular::Epitope purpose
	);

	void RemoveSynapsesTo(
		Neuron* target,
		cellular::Cell::Traits purpose
	);

	/**
	These should also be pretty self explanatory
	They RETURN the actual Synapse*s used.
	*/
	Synapse* GetSynapseTo(
		Neuron* target,
		SynapseId cid
	);

	Synapses GetSynapsesTo(Neuron* target);

	Synapses GetSynapsesTo(
		Neuron* target,
		molecular::Epitope purpose
	);

	Synapses GetSynapsesTo(
		Neuron* target,
		cellular::Cell::Traits purpose
	);

	Synapses GetAllOutgoingSynapses();

	/**
	This is called directly before sending a datum.
	Activates "PreSend" protein by default
	*/
	virtual void PreSend();

	/**
	Calls Synapse::ProcessOutgoing for each outgoing Synapse.
	*/
	virtual void Transmit();

	/**
	This is called directly after sending a datum.
	Activates "PostSend" protein by default
	*/
	virtual void PostSend();

	/**
	Same as Transmit, but with additional qualifiers.
	Specifying "any" trait (true) allows for partial matches, while false forces complete matches (i.e. the only traits in a Synapse are those in selection)
	*/
	virtual void TransmitSelectively(
		cellular::Cell::Traits selection,
		bool any
	);

	/**
	calls the above method with only 1 trait and any=true
	*/
	virtual void TransmitSelectively(molecular::Epitope selection);

	/**
	Sends an ExpireSignal() to all downstream neurons.
	This signifies the data from *this are no longer relevant and should be ignored/removed.
	*/
	void ExpireSentData();

	//END: Synapses

	//START: Personality things

	/**
	RETURNS: a pointer to the personality engine.
	The pointer is initial set by the Brain.
	This can be used to listen to personality levels, or influence them. For example, a food sensor could only become active if the creature is hungry. Once active, said food sensor could turn up love, because the creature loves food.
	*/
	Personality* GetPersonality()
	{
		return m_pe;
	}

	const Personality* GetPersonality() const
	{
		return m_pe;
	}

	//END: Personality things

	//START: cellular::Cell overrides, see that class for details

	virtual void TranscribeTopLevelProteins();

	virtual void FoldAllProteins(void* arg = NULL);

	virtual bool Register(organism::Organism* applicator);

	//END: cellular::Cell overrides

	//START: LoggerObject overrides

	virtual void SetLogEngine(log::Engine* logEngine);

	//END: LoggerObject overrides

	//START: These are not for you.

	/**
	Poll is meant to be called on a clock.
	Poll will perform all neuron related upkeep and then call OnPoll() to handle any clock related neural protein.
	See OnPoll more info.
	*/
	void Poll();

	/**
	The neuron will receive a personality engine pointer from the brain.
	*/
	void SetPersonality(Personality* pe);

	/**
	Adds the container id to *this
	This allows DoesNeuropilHaveAccess to return true with the given id.
	*/
	void AddedToNeuropil(NeuropilId neuropilId);

	/**
	Removes the given container entry from *this.
	*/
	void RemovedFromNeuropil(NeuropilId neuropilId);

	//END: These are not for you.

protected:
	/**
	Proteins (see cellular::Cell.h for more information)
	*/
	cellular::Protein* m_risingEdge;
	cellular::Protein* m_continuing;
	cellular::Protein* m_fallingEdge;
	cellular::Protein* m_prePoll;
	cellular::Protein* m_poll;
	cellular::Protein* m_preSend;
	cellular::Protein* m_postSend;
	cellular::Protein* m_learn;

	/**
	If you define your own firing conditions, make sure to overload this!
	You'll want to make this handle the logic for how to set m_firingReason
	RETURNS whether or not there is a impulse to send
	See the current definition for an example
	*/
	virtual bool DetermineImpulseTriggers(bool trigger);

	fc::Impulses m_impulses;

	fc::Impulses::iterator GetImpulseIttr(Name impulseName);

	fc::Impulses::const_iterator GetImpulseIttr(Name impulseName) const;

	void ProcessAllSynapses(); //handles new and expired data.
	void ProcessSynapse(Synapse* synapse);

	void RetrieveDataSelectively(
		cellular::Cell::Traits selection,
		bool any
	);

	void RetrieveDataSelectively(
		molecular::Epitope selection,
		bool any
	);

	void TransmitCommon(
		cellular::Cell::Traits selection,
		bool any
	);

	//START: More Aspect stuff

	Aspects::iterator GetAspectIttr(AspectId aspect);

	Aspects::const_iterator GetAspectIttr(AspectId aspect) const;

	void IncrementCountFor(Aspect aspect); //by 1
	void DecrementCountFor(Aspect aspect); //by 1
	void CheckForReset(const Aspect aspect); //will reset if it should.

	//END: More Aspect stuff

private:
	Synapses m_outgoingSynapses;
	Synapses m_incomingSynapses;

	Timestamp m_lastActive;
	bool m_isActive;

	/**
	Records the number of incoming impulses that have changed the given aspect
	*/
	Aspects m_aspects;

	/**
	Ids of containers *this should be in.
	*/
	NeuropilIds m_containers;

	NeuropilIds::iterator FindContainer(NeuropilId neuropilId);

	NeuropilIds::const_iterator FindContainer(NeuropilId neuropilId) const;

	FiringConditionId m_firingReason;

	/**
	Used to differentiate rising edge from continuing excitation triggers.
	*/
	bool m_continuingImpulse;

	/**
	Causes FallingEdge to be delayed until after the indicated time.
	*/
	TimeMS m_persistFor;

	Personality* m_pe;
};

typedef std::vector< Neuron* > Neurons;
typedef std::vector< const Neuron* > ConstNeurons;
typedef std::vector <Neurons> NeuronGroups;

} //neural namespace;
} //bio namespace
