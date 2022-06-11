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

/**
The Brain provides an easy to use interface for creating an intelligent creature.
There is only one Brain per creature but multiple creatures may use the same Brain configuration (but not instance).
The Brain holds all the Neurons needed to handle inputs, outputs, and other operations within the neural framework for your project.
It is also responsible for instantiating, connecting, and configuring them.
*/

#include "bio/cellular/Organism.h"
#include "bio/common/Assert.h"
#include "bio/common/PollObject.h"
#include "bio/common/Named.h"
#include "bio/common/Functions.h"
#include "bio/personality/Personality.h"
#include "../../../lib_bio/inc/bio/cellular/cell/Synapse.h"
#include "../../../lib_bio/inc/bio/cellular/cell/Neuron.h"
#include "Neuropil.h"

class BrainImpl;

namespace bio {

typedef uint32_t BrainId;

class Brain : public cellular::Cell, public PollObject, public Named<BrainId>
{
public:
    /**
    Brains are NamedClasses for debugging and management purposes. However, the default ID for a brain is not used. Additionally, the NameTracker for the Brain is left NULL. If you would like to manage more than one Brain, feel free to:
    1. BIO_NAME_TRACKER_SINGLETON(MyBrainTracker, BrainId)
    2. call SetTracker(MyBrainTracker)
    NOTE: SetTracker can only be called once per Brain!
    The log::Engine will be provided to all cellular::Plasmids, Neuropils, Neurons, and Synapses created within *this. If no log::Engine* is provided, logs will not be available. See log/Engine.h for more info. More log::Engine access methods are provided in the logging section (below).
    */
    Brain(Name name="vanilla", Personality* personality = new Personality(), log::Engine* logEngine = NULL);
    virtual ~Brain();

    /**
    RETURNS: "${Personality::Name}_${Brain::Name}"
    */
    Name GetDescriptiveName() const;

    /**
    You may just call Start() instead of the following; details are include only if you would like more control.
    The following methods should be called IN ORDER when starting an organism:
    1. LoadPlasmids (if desired)
    2. organism::Organism:: 
        a. ApplyPlasmidsFor("Brain",true)
        b. TranscribeTopLevelProteins({"Brain"}, Organism::ANY)
        c. FoldAllProteins({"Brain"}, Organism::ANY)
    3. PostConstructorProtein()
    4. PreSetup()
    5. CreatePersonality()
    6. CreateSynapses()
    7. CreateNeurons()
    8. CreateNeuralFramework()
    9. organism::Organism:: 
        a. ApplyPlasmidsFor("Brain", false)
        b. TranscribeTopLevelProteins({"Brain"}, Organism::NONE)
        c. FoldAllProteins({"Brain"}, Organism::NONE)
        d. CallPostConstructorProteins({"Brain"}, Organism::NONE)
    10. FoldNeuralFramework()
    11. PostSetup()
    */
    virtual void Start();

    /**
    During operation, you may call Poll method on the clock.
    Alternatively, especially if you are using a multi-threaded system, you may store pointers to individual Neuropils and Poll each of them individually.
    This method Polls the following, in order: All Neurons, in the order they were added to *this
    NOTE: This Poll method DOES NOT respect m_pollPeriodMS of the Brain
    TODO: add support for polling neurons multiple times or out of order, even though its probably indicative of poor design.
    */
    virtual void Poll();

    /**
    Stop(), like Start, takes care of the following for you: When shutting down the creature, call the following methods IN ORDER:
    1. StopNeuralFramework()
    2. organism::Organism:: CallPreDestructorProteins({"Brain"}, Organism::NONE);
    3. PreDestructorProtein()
    4. InvokeApoptosis() This "kills" the creature, deleting all parts of the Neural Framework
    */
    virtual void Stop();

    /**
    NOTE: the virtual methods below (also referenced in Start and Stop) can be overridden and the Brain methods can be ignored, but this will disconnect the Brain from the protein system and prevent the default logs from being printed. Because of this, it is highly recommended that you call these parent methods in your derived brain.
    /**The message printed is as follows: FunctionName finished successfully OR FunctionName failed: ReturnCodeAsString
    //void functions will always log that they were successful. These methods also only check the return code of the Proteins they activate and have no means of checking the status of derived methods

    /**
    Use LoadPlasmids to create and add ProteinPlasmids to the Organism.
    This method is not necessary for applications that will not be taking advantage of the Protein system. However, it is required, unless you know exactly what you're doing.
    */
    virtual void LoadPlasmids();

    /**
    This method is required to Add a plasmid.
    If you bypass this by calling GetApplicator()->AddPlasmid, the plasmid will not be properly configured.
    */
    void AddPlasmid(cellular::Plasmid* newPlasmid);

    template<class MODULE>
    void AddPlasmid() {AddPlasmid(new MODULE(GetLogEngine()));}

    /**
    Activate anything necessary before setting up the Neural Framework.
    virtual ReturnCode PreSetup();

    /**
    The Personality requires PersonalityVars (see Personality.h for more info)
    It would be a good idea to read these values in from a file, but that is not required.
    TODO: implement cross platform file RW system.
    NOTE: The Personality instance that can be retrieved via GetPersonality is created on construction, so this method is only for creating PersonalityVars and adding them to the existing Personality instance.
    */
    virtual ReturnCode CreatePersonality();

    /**
    Creates all the Synapse*s that will be used within CreateNeuralFramework.
    Plasmids or other systems that define Synapse features may add their own proteins to this method that create Synapses for later use. If this is done, the IDs of the created synapses should be documented or otherwise defined in the plasmid, etc.
    If you wish to define your own Synapses, override this method and use the CreateSynapse method to get a base synapse which can be configured to your liking. OR Create a child class of Synapse and new it within this method, followed by StoreSynapse, which will return the ID of the created Synapse for later use.
    IMPORTANT: when overriding this method, don't forget to call the parent's method too!
    If desired, custom sets of SynapseIds may be made by stating at GetIDBlockForSynapses(# of enum items ) When using this, make sure to manually AssignID to each of the corresponding Synapses. For an example of this, see the documentation, below, for GetIDBlockForSynapses
    */
    virtual ReturnCode CreateSynapses();

    /**
    Create all Neurons that will be used in CreateNeuralFramework.
    This is also a good place to create the Neuropils that your Neurons will be stored in.
    See StoreNeuron and CreateNeuron (below) for more info
    Please don't name any 2 Neurons the same name.
    Same with Neuropils (Neuropils).
    Any duplicate names will just add confusion to your programs and introduce potential points of error, if not outright failure. For more info on naming requirements, see Neuron.h and Neuropil.h
    No logs will be printed for this method.
    */
    virtual ReturnCode CreateNeurons() = 0;

    /**
    Setup the Neural Framework.
    This is where all Synapses should be made and where most configuration should happen.
    Helper functions for configuring or otherwise setting up neurons, synapses, or any other part of the brain or Neural Framework should be called here.
    No logs will be printed for this method.
    */
    virtual ReturnCode CreateNeuralFramework() = 0;

    /**
    Make sure everything is ready to go.
    virtual ReturnCode FoldNeuralFramework();

    /**
    Do anything that must be done to ensure smooth operation of the Neural Framework.
    This step is where most pre-start, post-init cleanup would take place.
    */
    virtual ReturnCode PostSetup();

    /**
    Stop the Neural Framework.
    After this, data should no longer be flowing through the network.
    */
    virtual ReturnCode StopNeuralFramework();

    /**
    Apoptosis in biology is programmed cell death.
    Similarly, this method "kills" (deletes) all Neurons (kinda like cells) in *this.
    It also destroys all Neuropils.
    After this is called, the Neural Framework ceases to exist.
    */
    virtual void InvokeApoptosis();

//START: Neuron Creation and Access

    /**
    All Neurons used must be stored in *this.
    Neurons can either be newed and stored in *this using StoreNeuron, or both steps may be done at once by calling CreateNeuron.
    In either case, existing Neuropils may be specified. The created or stored Neuron will then be added to the indicated Neuropils. An easy method for up to 3 Neuropils has been provided. If you wish to specify more than 3 Neuropils, supply a vector instead.
    Alternatively, you may also use GetNeuropil and Neuropil::AddNeuron but only AFTER Creating or Storing the Neuron in *this.
    RETURNS: the stored or created Neuron on success ULL otherwise
    */
    Neuron* StoreNeuron(Neuron* neuronToStore, Names neuropils);

    /**
    Create is the same as Store, but returns a pointer of the given type.
    */
    template <typename T>
    T* StoreNeuronType(T* neuron, Names neuropils)
    {
        if (!StoreNeuron(neuron, neuropils))
            return NULL;
        return neuron;
    }

    /**
    To create a new, generic neuron, use the following
    Neuron* CreateNeuron(Name name, Names neuropils);
    Create a template neuron by passing in a type
    */
    template <typename T>
    T* CreateNeuron(Name name, Names neuropils)
    {
        T* neuron = new T(name);
        if (!StoreNeuron(neuron, neuropils))
            return NULL;
        return neuron;
    }

    /**
    You should not have to delete Neurons after you create them. BUT, if you really need to, use these methods.
    */
    void DestroyNeuron(NeuronId neuronId, NeuropilId neuropilId);
    void DestroyNeuron(Neuron* toDestroy);

    /**
    To get a Neuron, use one of the following methods. NOTE: because neuron name/Id uniqueness is only checked within neuropils, anything that does not specify the neuropil will return the first neuron of the given name/Id. This can be handy in reducing code if you know what you are doing, but be careful not to have 2 neurons of the same name.
    */
    Neuron* GetNeuron(NeuronId neuronId);
    Neuron* GetNeuron(NeuronId neuronId, NeuropilId neuropilId);
    template <typename T> //For easy casting
    T* GetNeuron(NeuronId neuronId)
    {
        return Cast<T, Neuron>(GetNeuron(neuronId));
    }
    template <typename T> //For easy casting
    T* GetNeuron(NeuronId neuronId, NeuropilId neuropilId)
    {
        return Cast<T, Neuron>(GetNeuron(neuronId, neuropilId));
    }

    /**
    Neurons are organized into compartments called Neuropils.
    Neuropils allow the Brain to perform batch proteins on certain sets of Neurons.
    To use Neuropils, they must be created using CreateNeuropil
    As described above eurons may be added to Neuropils created this way.
    To add a Neuron to a Neuropil later, simply (with return checking): GetNeuropil(...)->AddNeuron(...)
    For more information on Neuropils, see Neuropil.h
    NOTE: multiple neuropils of the same id are NOT allowed.
    If the id of neuropil matches one already in *this, it will not be added.
    */
    Neuropil* CreateNeuropil(Name name);

    /**
    RETURNS: a pointer to all the Neuropils in *this.
    */
    Neuropils* GetNeuropils();

    /**
    RETURNS: the single neuropil specified ULL otherwise.
    If warn is false, warnings about unfound neuropils will be suppressed.
    */
    Neuropil* GetNeuropil(NeuropilId neuropilId, bool warn=true);
    Neuropil* GetNeuropil(Name neuropilName, bool warn=true)
    {
        return GetNeuropil(NeuropilTracker::Instance().IdFromName(neuropilName), warn);
    }
    
    /**
    RETURNS: the number of Neuropils in *this.
    */
    unsigned int GetNumNeuropils();

    /**
    RETURNS: the total number of Neurons in *this.
    */
    unsigned int GetNumNeurons() const;

    /**
    DO NOT USE THIS!!!!
    unless you absolutely have to.
    RETURNS: a pointer to the full list of all Neurons in the Brain.
    */
    Neurons* GetAllNeurons();

//END: Neuron Creation and Access

//START: Synapse Creation and Access

    //Synapses are created in the CreatSynapses method above.

    /**
    CreateSynapse RETURNS a generic Synapse* to be configured.
    All created Synapses are automatically stored in *this.
    name must be unique and not already used in *this
    */
    Synapse* CreateSynapse(Name name);

    /**
    Copies the given synapse, assigns a unique ID to it, stores it in *this, and RETURNS it.
    newName must be unique and not already used in *this
    */
    Synapse* CreateSynapseBasedOn(SynapseId toCopyId, Name newName);

    /**
    Storing a synapse allows it to be used later.
    RETURNS: the unique ID bestowed upon the synapse according to its name.
    */
    SynapseId StoreSynapse(Synapse* synapseToStore);

    /**
    RETURNS: a Synapse* of the given ID.
    */
    Synapse* GetSynapse(const SynapseId cid);

    /**
    RETURNS: the synapse from source to target with the given ID
    */
    Synapse* GetSynapse(Neuron* source, Neuron* target, SynapseId cid);

    /**
    RETURNS: the synapse from fromNeuron to toNeuron with matching ID.
    NOTE: if there is more than one Neuron with a given name, the first Neuron found will be used. If this is an issue, please use the GetSynapse method below.
    */
    Synapse* GetSynapse(NeuronId fromNeuronId, NeuronId toNeuronId, SynapseId cid);

    /**
    Same as GetSynapse above, but more reliable as it searches through the indicated neuropil to find the source/from Neuron and the same for the target/to Neuron.
    */
    Synapse* GetSynapse(NeuronId fromNeuronId, NeuropilId sourceNeuropilId euronId toNeuronId europilId targetNeuropilId, SynapseId cid);

//END: Synapse Creation and Access

//START: Personality stuffs

    /**
    RETURNS: a pointer to the personality engine used in *this
    */
    Personality* GetPersonality();

//END: Personality stuffs

//START: Timing

    /**
    Calls SetPollPeriod on all Neurons and the Personality.
    */
    virtual void SetPollPeriod(TimeMS period); //PollObject override.

    /**
    Only calls SetPollPeriod for the given neuropil.
    */
    virtual void SetPollPeriod(TimeMS period, NeuropilId NeuropilId);

    /**
    To avoid bursts in cpu usage when a large number of neurons are polled at once, the respective poll times can be offset so that they will be polled on different clock ticks.
    The first lastPollTimestamp will start at startTime, unless it is 0, then the current time will be used.
    RETURNS: the timestamp to be used for the next neuron in the series i.e. startTime + numNeurons * offset
    TODO: is this a good idea?
    */
    Timestamp StaggerPolling(TimeMS offset, Timestamp startTime = 0);
    Timestamp StaggerPolling(TimeMS offset, NeuropilId NeuropilId, Timestamp startTime = 0);

    /**
    Some neurons may handle all of their operations outside of polling, to save cpu time in those cases, you can call the following, which will cause the indicated neurons not to be polled on the clock.
    void DoNotPoll(Neuron* neuron);
    void DoNotPoll(NeuronId neuronId, NeuropilId neuropilId);
    void DoNotPoll(NeuropilId neuropilId);

//END: Timing

//START: Axon Guidance

    Creates a synapse from source to target
    This method requires that a synapse of the given ID be defined in the CreateSynapses method above.
    RETURNS the Synapse* created, if successful. NULL otherwise. Also see Neuron::ConnectTo
    */
    Synapse* Connect(Neuron* source, Neuron* target, SynapseId cid);
    Synapse* Connect(Neuron* source, Neuron* target, Synapse* synapseType);

    /**
    Same as above, but searches for the indicated neurons
    Synapse* Connect(NeuronId sourceNeuronId, NeuronId targetNeuronId, SynapseId cid);
    Synapse* Connect(NeuronId sourceNeuronId, NeuronId targetNeuronId, Synapse* synapseType);

    /**
    Same as above, but more reliable as it searches through the indicated neuropil to find the source/from Neuron, same for the target/to Neuron (same situation as GetSynapse).
    Synapse* Connect(NeuronId sourceNeuronId, NeuropilId sourceNeuropilId euronId targetNeuronId europilId targetNeuropilId, SynapseId cid);
    Synapse* Connect(NeuronId sourceNeuronId, NeuropilId sourceNeuropilId euronId targetNeuronId europilId targetNeuropilId, Synapse* synapseType);

    /**
    Connects all Neurons in sourceNeuropil to all Neurons in targetNeuropil.
    RETURNS: false if any neuropils were not found; true otherwise.
    */
    bool ConnectNeuropils(NeuropilId sourceNeuropilId, NeuropilId targetNeuropilId, SynapseId cid);

    /**
    Same as above, but the target is a single Neuron, rather than all Neurons in a neuropil
    bool ConnectNeuropilTo(NeuropilId sourceNeuropilId, NeuronId neuronTargetId europilId neuronNeuropilId, SynapseId cid);

    /**
    Inverse of above.
    RETURNS the same.
    */
    bool ConnectToNeuropil(NeuronId neuronSourceId, NeuropilId sourceNeuropilId europilId targetNeuropilId, SynapseId cid);
    bool ConnectToNeuropil(NeuronId neuronSourceId, NeuropilId sourceNeuropilId europilId targetNeuropilId, const Synapse* synapseType);

    /**
    Calls ConnectNeuropils with the same source and target.
    */
    bool CreateSynapseBetweenAllNeuronsIn(NeuropilId neuropilId, SynapseId cid);

    /**
    Clones each Neuron in the indicated Neuropil and places them in the placeIn Neuropil.
    Will create the placeIn Neuropil if it does not exist.
    RETURNS: the NeuropilId of PlaceIn
    */
    NeuropilId DuplicateNeuropil(NeuropilId sourceNeuropilId, Name placeIn);

    /**
    Creates a copy of the given sourceNeuropil (if it exists) but connects the new neurons in such a way that only the most active neuron in the original container will trigger its corresponding copy
    All new neurons will have the same name as those they correspond to, but will be placed in a neuropil of the given name
    Excitatory and inhibitory synapse IDs are required. If using the Core plasmid, Core::Excitatory() and Core::LateralInhibitory() are recommended. Any other synapses may have undefined results.
    RETURNS: true on success, false otherwise.
    FIXME: add cloning feature to Neuron, right now, this just creates generic neurons.
    */
    NeuropilId CreateWinnerTakeAllFor(NeuropilId sourceNeuropilId, Name placeIn, SynapseId excitatoryId, SynapseId inhibitoryId);

    /**
    IMPORTANT: This only works if both Neuropils are the same size!!
    Creates a Synapse of cid between the first Neuron in sourceNeuropil and the first Neuron in targetNeuropil and so on until it creates a Synapse of cid between the n^th Neuron in sourceNeuropil and the n^th Neuron in targetNeuropil.
    RETURNS: true on success, false otherwise.
    */
    bool CreateOneToOneSynapsesBetween(NeuropilId sourceNeuropilId, NeuropilId targetNeuropilId, SynapseId cid);

    /**
    This method will create a number of synapses from each neuron in sourceNeuropil to targetNeuropil.
    The chance of a synapse being created is given by probability p.
    p is a scalar between 0 and 100.
    */
    bool CreateRandomSynapsesBetween(NeuropilId sourceNeuropilId, NeuropilId targetNeuropilId, unsigned int p, SynapseId cid);

//END: Axon Guidance

//START: Protein System Interface

    /**
    Adds the indicated epitope to all Neurons.
    */
    void AddEpitopeToAllNeurons(Epitope epitope);

    /**
    Same as above, but applies only to Neurons in neuropil.
    RETURNS: false if the neuropil cannot be found; true otherwise.
    */
    bool AddEpitopeToAllNeuronsIn(Epitope epitope, NeuropilId neuropilId);

    /**
    Remove the indicated epitope from all Neurons.
    */
    void RemoveEpitopeFromAllNeurons(const Epitope& epitope);

    /**
    Same as above, but applies only to Neurons in neuropil.
    RETURNS: false if the neuropil cannot be found; true otherwise.
    */
    bool RemoveEpitopeFromAllNeuronsIn(const Epitope& epitope, NeuropilId neuropilId);

    /**
    Adds a epitope to the synapse returned by GetSynapse(...)
    RETURNS: false if any Neuron or Synapse could not be found. true otherwise.
    */
    bool AddEpitopeToSynapse(molecular::Epitope toAdd, NeuronId fromNeuronId euronId toNeuronId, SynapseId cid);

    /**
    Like the above for all synapses from all neurons in the neuropil.
    RETURNS: the same as above.
    */
    bool AddEpitopeToAllSynapsesFrom(molecular::Epitope toAdd, NeuropilId NeuropilId);

    /**
    Same as above for the synapses that have a target in the targetNeuropil.
    RETURNS: the same as above
    */
    bool AddEpitopeToAllSynapsesFromTo(molecular::Epitope toAdd, NeuropilId sourceNeuropilId europilId targetNeuropil);

    /**
    Removes a epitope to the synapse returned by GetSynapse(...)
    RETURNS: false if any Neuron or Synapse could not be found or if the synapse did not contain the epitope to begin with. true otherwise.
    */
    bool RemoveEpitopeFromSynapse(molecular::Epitope toRemove, NeuronId fromNeuronId euronId toNeuronId, SynapseId cid);

    /**
    stores the given value and id in the given Neuron.
    RETURNS: false if the neuron could not be found, true otherwise
    */
    bool SetFor(cellular::Cell::MolecularValueId id, cellular::Cell::MolecularValue value, NeuronId neuronId europilId neuropilId);

    template<typename T>
    bool SetAsFor(cellular::Cell::MolecularValueId id, const T& value, NeuronId neuronId europilId neuropilId)
    {
        return SetFor(id, &value, neuronId, neuropilId);
    }

    /**
    Adds a StoredValue and Id to all Neurons. For more information on StoredValues, please see Cell.
    */
    void SetForAllNeurons(cellular::Cell::MolecularValueId id, cellular::Cell::MolecularValue value);

    template<typename T>
    void SetAsForAllNeurons(cellular::Cell::MolecularValueId id, const T& value)
    {
        SetForAllNeurons(id, &value);
    }

    /**
    Same as SetForAllNeurons, but limited to all neurons in the specified neuropil.
    RETURNS: false if the neuropil could not be found; true otherwise.
    */
    bool SetForAllNeuronsIn(cellular::Cell::MolecularValueId id, cellular::Cell::MolecularValue value, NeuropilId neuropilId);

    template<typename T>
    bool SetAsForAllNeuronsIn(cellular::Cell::MolecularValueId id, const T& value, NeuropilId neuropilId)
    {
        return SetForAllNeuronsIn(id, &value, neuropilId);
    }

    /**
    Adds a StoredValue to the Synapse. See cellular::Cell for more information on StoredValues.
    RETURNS: false if any Neuron or Synapse could not be found. true otherwise.
    */
    bool SetForSynapse(cellular::Cell::MolecularValueId id, cellular::Cell::MolecularValue value, NeuronId fromNeuronId euronId toNeuronId, SynapseId cid);

    template<typename T>
    bool SetAsForSynapse(cellular::Cell::MolecularValueId id, const T& value, NeuronId fromNeuronId euronId toNeuronId, SynapseId cid)
    {
        return SetForSynapse(id, &value, fromNeuronId, toNeuronId, cid);
    }

    /**
    Same as above, but for all synapses from all neurons in a container
    */
    bool SetForAllSynapsesFrom(cellular::Cell::MolecularValueId id, cellular::Cell::MolecularValue value, NeuropilId neuropilId, SynapseId cid);

    template<typename T>
    bool SetAsForAllSynapsesFrom(cellular::Cell::MolecularValueId id, const T&  value, NeuropilId neuropilId, SynapseId cid)
    {
        return SetForAllSynapsesFrom(id &value, neuropilId, cid);
    }

    /**
    Above but for all synapses to all neurons in the given neuropil
    */
    bool SetForAllSynapsesTo(cellular::Cell::MolecularValueId id, cellular::Cell::MolecularValue value, NeuropilId neuropilId, SynapseId cid);

    template<typename T>
    bool SetAsForAllSynapsesTo(cellular::Cell::MolecularValueId id, const T&  value, NeuropilId neuropilId, SynapseId cid)
    {
        return SetForAllSynapsesFrom(id &value, neuropilId, cid);
    }

    /**
    Same as above, but for all synapses from all neurons in a neuropil to all neurons in another neuropil
    */
    bool SetForAllSynapsesFromTo(cellular::Cell::MolecularValueId id, cellular::Cell::MolecularValue value, NeuropilId sourceNeuropilId europilId targetNeuropilId, SynapseId cid);

    template<typename T>
    bool SetForAllSynapsesFromTo(cellular::Cell::MolecularValueId id, const T& value, NeuropilId sourceNeuropilId europilId targetNeuropilId, SynapseId cid)
    {
        return SetForAllSynapsesFromTo(id, &value, sourceNeuropilId, targetNeuropilId, cid);
    }

//END: Protein System Interface

//START: Logging

    /**
    The following methods are forwarded from log::Engine; see that class for details.
    */
    bool FilterSet(log::Filt::Plasmid plasmid, log::Filt::Level level);
    bool FilterSet(const char* plasmid, const char* level);

    /**
    The following methods are for debug use.
    They will give you an idea of what the current brain state looks like.
    Each will print out all (but reasonably) available information on the given topic (Neurons, Synapses europils)
    The logs will be printed as BRAIN DEBUG
    */
    void LogSynapses();
    void LogNeurons();
    void LogNeuropils();

//END: Logging

//START: cellular::Cell overrides

    virtual void TranscribeTopLevelProteins();

//END: cellular::Cell overrides

protected:
    /**
    Proteins. See cellular::Cell.h for more details
    */
    cellular::Protein* m_preSetup;
    cellular::Protein* m_postSetup;
    cellular::Protein* m_createSynapses;
    cellular::Protein* m_initializeFramework;
    cellular::Protein* m_stopFramework;

private: /**
    Transcribe descriptive name.
    See GetDescriptiveName()
    */
    std::string m_descriptiveName;

    /**
    For speed purposes, this will not check for duplicates.
    */
    Neuron* StoreNeuronCommon(Neuron* toStore);

    /**
    Erases toRem from m_allNeurons after to deleting *toRem.
    */
    void DestroyNeuronCommon(Neuron* toDestroy);

    Neurons::iterator GetIteratorFor(Neuron* neuron);
    Synapses::iterator GetIteratorFor(Synapse* synapse);
    Synapses::iterator GetIteratorFor(SynapseId cid);

    void DeleteNeuropils();

    Neurons m_allNeurons;
    Neurons m_neuronsToPoll;
    Neuropils m_neuropils;
    Synapses m_synapses;

    Personality* m_pe;
    organism::Organism* m_modApp;
};

} //bio namespace
