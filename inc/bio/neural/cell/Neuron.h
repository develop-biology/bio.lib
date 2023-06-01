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

#include "Synapse.h"
#include "bio/neural/Impulse.h"

namespace bio {
namespace neural {

/**
 * When to use a Neuron: <br />
 * 1. You want explicit control over who can use what data when (structure). <br />
 * 2. You want to process many signals in many different threads (asynchronicity). <br />
 * 3. If Diffusing many signals through the Solution system is cumbersome and Neurons are more efficient. <br />
 * <br />
 * To use a Neuron, you must define the following: <br />
 * 1. The Neuron child class; this is a Cell which holds whatever values and does whatever work you want. <br />
 * 2. Define an ExciteTrigger which will determine what data is sent through which Neurites when (see below for more on that). <br />
 * 3. Define the Neuron's Neurites, which connect it to other Neurons; this is the structure of your Organism's nervous system. <br />
 * <br />
 * In real-life biology, neurons are implemented with solutions, membranes / vesicles, and proteins. These neurons (in theory) diffuse an arbitrary number of solutes (aka potentials) at once, and equalize carefully crafted imbalances in electrochemical gradients. Thus, neurons allow for fast, optimized delivery of information through structured channels which enforce requirements of how and when data may be accessed. <br />
 * This is the spirit which we attempt to emulate here. <br />
 */
class Neuron :
	public neural::Class< Neuron >,
	public StemCell,
	public Covalent< chemical::LinearMotif< Impulse* > >,
	public Covalent< chemical::LinearMotif< Dendrite* > >, //Neurite
	public Covalent< chemical::LinearMotif< Axon* > > //Neurite
{
public:

	/**
	 * Disambiguate Class methods. <br />
	 */
	BIO_DISAMBIGUATE_REQUIRED_CLASS_METHODS(neural, Neuron)
	BIO_DISAMBIGUATE_OPTIONAL_CLASS_METHODS(genetic, Neuron)

	BIO_DEFAULT_IDENTIFIABLE_CONSTRUCTORS_WITH_COMMON_CONSTRUCTOR(
		neural,
		Neuron,
		filter::Neural()
	)

	virtual ~Neuron();

	//START: Recommended overrides

	/*
	 * These are the most important methods for every Neuron.
	 * However, they may not all be required for your Neuron.
	 */

	/**
	 * Use this method to populate any member variable Protein*s. <br />
	 * You'll want to do this to speed up your code by bypassing the dynamic execution provided by genetic::Expressor. <br />
	 */
	virtual Code CacheProteins();

	/**
	 * If you use CacheProteins, you'll likely want to create your default Proteins here. <br />
	 * This will prevent dereferencing null or garbage pointers when using your cached Proteins. <br />
	 */
	virtual Code CreateDefaultProteins();

	/**
	 * It is up to each individual Neuron to determine what is appropriate here. <br />
	 * @return whether or not *this should be active
	 */
	virtual bool ExciteTrigger() const
	{

		// YOUR CODE GOES HERE

		return false;
	}
	
	// The above can also be accessed through the Impulse methods, below.

	/**
	 * Activated at the just before Crest(), before internal stuffs and before Crest(). <br />
	 */
	virtual void PreCrest();

	//END: Recommended overrides

	/**
	 * Learn tells the Neuron when to apply any learning operations. <br />
	 * This is a no-op unless the Learning plasmid is used. <br />
	 * While this method is virtual, it is NOT advised you override it, unless you know exactly what you are doing and have your own learning system to integrate with. <br />
	 * ----WHEN TO USE---- <br />
	 * Consider what you want your system to learn about. <br />
	 * If you want to learn/predict when a Neuron will be active, call this when the Neuron becomes active. NOTE that you might not want this done on RisingEdgeImpulse or some other location that could be just as easily triggered by any incoming excitatory Synapse as innate activity. For example, sensors call Learn when they receive data from a driver That way, any neurons connected to them will start predicting real world inputs (as opposed to other neural activity). <br />
	 * For learning to other inputs, Learn will be called on addition of a "DriveLearning" Synapse. For more info, see the mod_learning-neural docs. <br />
	 */
	virtual void Learn();

	//START: Impulses

	/*
	 * ______Impulse() methods are called by DetermineAndCallImpulseTriggers(). <br />
	 * It is up to children of *this to determine what these methods do. <br />
	 * NOTE: Impulses should not set the FiringCondition, which is returned by GetReasonForFiring. Only DetermineImpulseTriggers() will do that. <br />
	 * If you would like to make your own Impulses, use the CreateImpulse() method and populate a molecular::Protein of the given Name. <br />
	 */

	/*
	 * The default Impulses are: "RisingEdge" Called if newly activated. <br />
	 * "Continuing" Called if active and previously active. <br />
	 * "FallingEdge" Called when *this stops being active. <br />
	 */

	/**
	 * This uses ExciteTrigger to set the FiringCondition of *this and calls whatever _____Impulse() method is appropriate. See firing_condition::Trigger, below for more on how those calls are made. <br />
	 * @return whether or not *this should be active. <br />
	 */
	bool DetermineAndCallImpulseTriggers();

	/**
	 * Causes DetermineAndCallImpulseTriggers() to start at RisingEdgeImpulse(), if it would otherwise call ContinuingImpulse(). <br />
	 */
	void StartAtRisingEdge();

	/*
	 * What "active" means is up to each Neuron to determine for itself. For example, a sensor may be active when a significant amount of stimulus is received, whereas a behavior may be active whenever it makes a call to an actuator. <br />
	 * When a Neuron is not sending continuing impulses, it will be considered active until proven otherwise (e.g. falling edge is sent). In such a case, GetTimeLastActive() will return the current timestamp. <br />
	 * NOTE: if a Neuron is not sending continuing impulses, ActiveUntil will not trigger a falling edge at the indicated time. The methods below are for reporting only and do not affect operation. <br />
	 * For how to keep a Neuron active for a time after an initial stimulus, see PersistFor or PersistUntil (below). <br />
	 * NOTE: activity may also by checked & modified through States. For example, this->Has< State >(::bio::state::Active()). <br />
	 */

	/**
	 * Set the time active to the current timestamp. <br />
	 */
	void ActiveNow();

	/**
	 * Manually set the time active. <br />
	 * @param time
	 */
	void ActiveUntil(Timestamp time);

	/**
	 * See ActiveNow, above.
	 * @return: last time *this was active
	 */
	Timestamp GetTimeLastActive() const;

	/**
	 * NOTE: activity may also by checked like this->Has< State >(::bio::state::Active()). <br />
	 * @return: whether or not *this is active.
	 */
	bool IsActive() const;

	/**
	 * This updates all Impulses to use *this as their caller. <br />
	 * This must be called whenever *this is copied, cloned, etc. <br />
	 */
	void UpdateImpulseCallers();

	/**
	 * Creates an Impulse and adds it to *this. <br />
	 * This will create a new top-level-protein, if one does not exist. <br />
	 * @param trigger the FiringCondition that will cause this Impulse to be sent.
	 * @param impulseName the Name of the Protein that will be used to process the Impulse.
	 * @param shouldBeActive whether or not this Impulse should cause *this to be active.
	 */
	void CreateImpulse(
		const FiringCondition& trigger,
		const Name& impulseName,
		bool shouldBeActive = true
	);

	/**
	 * This is ONLY set by DetermineAndCallImpulseTriggers. <br />
	 * If you would like to control this manually, please adjust your ExciteTrigger so that DetermineAndCallImpulseTriggers gives the results you desire. <br />
	 * @return: the FiringCondition that caused *this to be active.
	 */
	FiringCondition GetReasonForFiring() const;

	/**
	 * @param now the current timestamp.
	 * @return whether or not *this is "active" due to persistence.
	 */
	bool IsPersisting(const Timestamp now) const;

	/**
	 * @return whether or not *this is "active" due to persistence.
	 */
	bool IsPersisting() const
	{
		return IsPersisting(physical::GetCurrentTimestamp());
	}

	//END: Impulses

	//START: Configuration/Settings
	/*
	 * Neurons may stay active (persist) for a given amount of time after their excite trigger. <br />
	 * Calling Persist___() will result in *this remaining active for the indicated time. <br />
	 * Once *this is no longer persisting, FallingEdge will be called. <br />
	 * NOTE: Persistence requires the continuing impulses only set Active___() if and only if ExciteTrigger() is true (as should always be the case). <br />

	 * FIXME: PersistUntil needs to only be applied once.
	 * FIXME: Is this even a useful feature?
	 */

	/**
	 * Always applied. <br />
	 * @param ms the number of milliseconds that *this should persist for.
	 */
	void PersistFor(const Milliseconds ms);

	/**
	 * Sets PersistFor with (time-now). <br />
	 * @param time
	 */
	void PersistUntil(const Timestamp time);

	//END: Configuration/Settings


	//START: Synapses

	/**
	 * Utilizes the Synapse::ConfigureFor method to create a new Synapse between *this and the target <br/>
	 * The target is whatever you want *this to send data to. <br/>
	 * The synapseType specifies how you want to send that data. <br/>
	 * The synapseType can be as simple as a Synapse* with some TranscriptionFactors added or as complex as a custom derived class. <br/>
	 * If a Synapse to the same target and of the same Id already exists, the existing Synapse will be RETURNED but not re-added. <br/>
	 * @param target: the Neuron to send data to.
	 * @param synapseType: the Synapse to use to send data.
	 * @return: the configured Synapse that has been added to *this. <br/>
	 */
	Synapse* ConnectTo(
		Neuron* target,
		const Synapse* synapseType
	);

	/**
	 * RequestProcessingOf checks if the data coming from the given Dendrite should be added (or removed) and will perform the necessary proteins. <br />
	 * The only reason to use this method, as opposed to letting Crest check the given Dendrite, is to have it be processed before another Dendrite i.e. this method gives you more control over the order in which Dendrites are processed. With that said, Crest will still process Dendrite in the order they are added, it will just skip any Dendrites which have already been processed, either by a previous Crest or by this method. <br />
	 * @param dendrite
	*/
	void RequestProcessingOf(Dendrite* dendrite);

	/**
	 * This is called directly before sending a datum. <br />
	 * Activates "PreSend" protein by default. <br />
	 */
	virtual void PreSend();

	/**
	 * Calls Synapse::ProcessOutgoing for each Axon. <br />
	 * @param selection (optional) the Affinity to use for selecting which Axons to send data through.
	 */
	virtual Code Transmit(Affinity* selection = NULL);

	/**
	 * This is called directly after sending a datum. <br />
	 * Activates "PostSend" protein by default. <br />
	 */
	virtual void PostSend();

	/**
	 * Sends an DepotentiateSignal() to all downstream neurons. <br />
	 * This signifies the data from *this are no longer relevant and should be ignored/removed. <br />
	 * @param selection (optional) the Affinity to use for selecting which Axons to expire data from.
	 */
	virtual Code DepotentiateSentData(Affinity* selection = NULL);

	//END: Synapses

	//START: These are not for you.

	/**
	 * physical::Periodic method; called every clock tick. <br />
	 * Performs all upkeep operations. <br />
	 */
	virtual bool CheckIn();

	/**
	 * Optional cellular method. <br />
	 * @return StemCell::Crest() by default.
	 */
	virtual Code Crest();

	/**
	 * Optional cellular method. <br />
	 * @return StemCell::Apoptose() by default.
	 */
	virtual Code Apoptose();

	//END: These are not for you.

protected:

	virtual void CommonConstructor();

	/**
	 * Proteins (see cellular::Cell.h for more information)
	 */
	molecular::Protein* mcRisingEdge;
	molecular::Protein* mcContinuing;
	molecular::Protein* mcFallingEdge;
	molecular::Protein* mcPreCrest;
	molecular::Protein* mcPreSend;
	molecular::Protein* mcPostSend;
	molecular::Protein* mcLearn;

	/**
	 * If you define your own firing conditions, make sure to overload this! <br />
	 * You'll want to make this handle the logic for how to set mFiringReason. <br />
 	 * See the current definition for an example. <br />
 	 * @param trigger
 	 * @return whether or not there is a impulse to send
	*/
	virtual bool DetermineImpulseTriggers(bool trigger);

	/**
	 * Add the data sent by the given Dendrite to *this. <br />
	 * @param dendrite
	 */
	 virtual void PotentiateDendrite(Dendrite* dendrite);

	/**
	 * Remove the data sent by the given Dendrite from *this. <br />
	 * @param dendrite
	 */
	virtual void DepotentiateDendrite(Dendrite* dendrite);

	/**
	 * Handles new and expired data. <br />
	 * @param selection (optional) the Affinity to use for selecting which Dendrites to bring data in from.
	 */
	virtual void ProcessDendrites(Affinity* selection = NULL);

	/**
	 * How a single Dendrite is processed. <br />
	 * @param dendrite
	 */
	virtual Code ProcessDendrite(Dendrite* dendrite);

private:
	Timestamp mLastActive;

	FiringCondition mFiringReason;

	/**
	 * Used to differentiate rising edge from continuing excitation triggers.
	 */
	bool mContinuingImpulse;

	/**
	 * Causes FallingEdge to be delayed until after the indicated time.
	 */
	Milliseconds mPersistFor;
};

} //neural namespace
} //bio namespace
