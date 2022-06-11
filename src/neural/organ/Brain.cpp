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

#include "bio/neural/Brain.h"
#include "bio/common/Assert.h"
#include "bio/common/ReturnCodes.h"
#include "bio/common/String.h"
#include "bio/common/Types.h"
#include "bio/common/Time.h"
#include "bio/log/Engine.h"
#include <sstream>
#include <cstdlib>

using namespace bio;

Brain::Brain(Name name, Personality* personality, log::Engine* logEngine): Named(name), cellular::Cell(), m_preSetup(NULL), m_postSetup(NULL), m_createSynapses(NULL), m_initializeFramework(NULL), m_stopFramework(NULL), m_pe(personality)
{
    SetLogFilter(log::Filt::BRAIN);
    SetLogEngine(logEngine);

    if (m_pe)
    {
        m_descriptiveName += m_pe->GetName();
        m_descriptiveName += "_";
    }
    m_descriptiveName += Name();
    
    m_modApp = new organism::Organism();
    m_modApp->SetLogEngine(GetLogEngine());

    Register(m_modApp);

    AddEpitope("Brain");

    CreateTopLevelProtein("PreSetup");
    CreateTopLevelProtein("PostSetup");
    CreateTopLevelProtein("CreateSynapses");
    CreateTopLevelProtein("FoldNeurons");
    CreateTopLevelProtein("StopNeurons");

    Inform("%s has been born!", GetDescriptiveName()); }

Brain::~Brain()
{
    delete m_pe;
    delete m_modApp;
    Inform("%s has died.", GetDescriptiveName()); //FIXME threading....
    SetLogEngine(NULL);
}

Name Brain::GetDescriptiveName() const
{
    //FIXME: I'm pretty sure this causes memory leaks.
    return m_descriptiveName.c_str();
}

void Brain::TranscribeTopLevelProteins()
{
    m_preSetup = GetProtein("PreSetup");
    m_postSetup = GetProtein("PostSetup");
    m_createSynapses = GetProtein("CreateSynapses");
    m_initializeFramework = GetProtein("FoldNeurons");
    m_stopFramework = GetProtein("StopNeurons");

    cellular::Cell::TranscribeTopLevelProteins();
}

void Brain::Start()
{
    cellular::Cell::Epitopes brainEpitopes;
    brainEpitopes.push_back(EpitopeFromString("Brain"));

    LoadPlasmids();
    GetApplicator()->ApplyPlasmidsFor("Brain", true);
    GetApplicator()->TranscribeTopLevelProteins(brainEpitopes, organism::Organism::ANY);
    GetApplicator()->FoldAllProteins(brainEpitopes, organism::Organism::ANY);
    PostConstructorProtein();
    PreSetup();
    CreatePersonality();
    CreateSynapses();
    CreateNeurons();
    CreateNeuralFramework();
    GetApplicator()->ApplyPlasmidsFor("Brain", false);
    GetApplicator()->TranscribeTopLevelProteins(brainEpitopes, organism::Organism::NONE);
    GetApplicator()->FoldAllProteins(brainEpitopes, organism::Organism::NONE);
    GetApplicator()->CallPostConstructorProteins(brainEpitopes, organism::Organism::NONE);
    FoldNeuralFramework();
    PostSetup();
}

void Brain::Poll()
{
    m_pe->Poll();
    for (Neurons::iterator nrn = m_neuronsToPoll.begin(); nrn != m_neuronsToPoll.end(); ++nrn)
    {
        (*nrn)->Poll();
    }
    m_lastPollTimestamp = time::GetCurrentTimestamp();
}

void Brain::Stop()
{
    cellular::Cell::Epitopes brainEpitopes;
    brainEpitopes.push_back(EpitopeFromString("Brain"));

    StopNeuralFramework();
    GetApplicator()->CallPreDestructorProteins(brainEpitopes, organism::Organism::NONE);
    PreDestructorProtein();
    InvokeApoptosis();
}

void Brain::LoadPlasmids()
{
    Inform("LoadPlasmids finished successfully");
}
void Brain::AddPlasmid(cellular::Plasmid* newPlasmid)
{
    //The comments about this method in the .h are a lie.
    //There is no harm in bypassing this (the following is public)

    GetApplicator()->AddPlasmid(newPlasmid);
}
ReturnCode Brain::PreSetup()
{
    ReturnCode ret = StandardActivate(m_preSetup);
    if (ret == ret::Success())
    {
        Inform("PreSetup finished successfully");
    }
    else 
    {
        Inform("PreSetup failed: %s", ret::CodeTracker::Instance().NameFromId(ret));
    }
    return ret;
}
ReturnCode Brain::PostSetup()
{
    ReturnCode ret = StandardActivate(m_postSetup);
    if (ret == ret::Success())
    {
        Inform("PostSetup finished successfully");
    }
    else 
    {
        Inform("PostSetup failed: %s", ret::CodeTracker::Instance().NameFromId(ret));
    }
    return ret;
}

ReturnCode Brain::CreateSynapses()
{
    ReturnCode ret = StandardActivate(m_createSynapses);
    LogSynapses();
    if (ret == ret::Success())
    {
        Inform("CreateSynapses finished successfully");
    }
    else 
    {
        Inform("CreateSynapses failed: %s", ret::CodeTracker::Instance().NameFromId(ret));
    }
    return ret;
}
ReturnCode Brain::FoldNeuralFramework()
{
    ReturnCode ret = StandardActivate(m_initializeFramework);
    if (ret == ret::Success())
    {
        Inform("FoldNeuralFramework finished successfully");
    }
    else 
    {
        Inform("FoldNeuralFramework failed: %s", ret::CodeTracker::Instance().NameFromId(ret));
    }
    return ret;
}
ReturnCode Brain::StopNeuralFramework()
{
    ReturnCode ret = StandardActivate(m_stopFramework);
    if (ret == ret::Success())
    {
        Inform("StopNeuralFramework finished successfully");
    }
    else 
    {
        Inform("StopNeuralFramework failed: %s", ret::CodeTracker::Instance().NameFromId(ret));
    }
    return ret;
}
void Brain::InvokeApoptosis()
{
    //TODO: this can have some more checks.
    for (Neurons::iterator nrn = m_allNeurons.begin(); nrn != m_allNeurons.end(); ++nrn)
    {
        DestroyNeuronCommon(*nrn);
    }
    m_allNeurons.clear();
    m_neuronsToPoll.clear();
    DeleteNeuropils();
    Inform("InvokeApoptosis finished successfully"); }

Neuron* Brain::StoreNeuronCommon(Neuron* toStore)
{
    if (!toStore)
    {
        Error("Cannot store null neuron");
        return NULL;
    }

    toStore->SetLogEngine(GetLogEngine());
    toStore->SetPersonality(GetPersonality());

    if (!toStore->Register(m_modApp))
    {
        Warn("Could not register %s. Storing anyway.", toStore->GetName());
    }

    m_allNeurons.push_back(toStore);
    m_neuronsToPoll.push_back(toStore);
    return toStore;
}

Neuron* Brain::StoreNeuron(Neuron* neuronToStore, Names neuropils)
{
    Neuron* store = StoreNeuronCommon(neuronToStore);
    if (!store)
    {
        return NULL;
    }
    Debug("Storing Neuron %s within %s", neuronToStore->GetName(), string::FromVectorOfStrings(neuropils).c_str());
    for (Names::iterator neuropil = neuropils.begin(); neuropil != neuropils.end(); ++neuropil)
    {
        if (!strlen(*neuropil))
        {
            continue;
        }
        Neuropil* storeIn = GetNeuropil(*neuropil, true);
        if (!storeIn)
        {
            continue;
        }
        storeIn->AddNeuron(store);
    }
    return store;
}
Neuron* Brain::CreateNeuron(Name name, Names neuropils)
{
    return StoreNeuron(new Neuron(name), neuropils);
}

void Brain::DestroyNeuronCommon(Neuron* toDestroy)
{
    if (!toDestroy)
    {
        Error("Cannot delete null neuron");
    }
    NeuropilIds affectedNeuropils = toDestroy->GetNeuropilsWithAccess();
    for (NeuropilIds::iterator neuropil = affectedNeuropils.begin(); neuropil != affectedNeuropils.end(); ++neuropil)
    {
        Neuropil* affected = GetNeuropil(*neuropil, true);
        bioassert(affected);
        if (!affected->Remove(toDestroy))
        {
            Error("Could not remove %s from %s", toDestroy->GetName(), affected->GetName());
        }
    }
    delete toDestroy;
}
void Brain::DestroyNeuron(NeuronId neuronId, NeuropilId neuropilId)
{
    Neuron* toDestroy = GetNeuron(neuronId, neuropilId);
    if (!toDestroy)
    {
        Error("Could not destroy %s. No such neuron exists in %s.", NeuronTracker::Instance().NameFromId(neuronId) europilTracker::Instance().NameFromId(neuropilId));
        return;
    }
    //TODO: this could be optimized by getting the iterator from the name search
    DestroyNeuron(toDestroy);
}
void Brain::DestroyNeuron(Neuron* toDestroy)
{
    Neurons::iterator toRem = GetIteratorFor(toDestroy);

    if (toRem == m_allNeurons.end())
    {
        Error("Could not destroy %s, it does not exist within the brain", toDestroy->GetName());
        return;
    }
    DestroyNeuronCommon(*toRem);
    m_allNeurons.erase(toRem);
}
Neuron* Brain::GetNeuron(NeuronId neuronId)
{
    for (Neurons::iterator nrn = m_allNeurons.begin(); nrn != m_allNeurons.end(); ++nrn)
    {
        if (*(*nrn) == neuronId)
        {
            return *nrn;
        }
    }
    Warn("Could not find neuron %s", NeuronTracker::Instance().NameFromId(neuronId));
    return NULL;
}
Neuron* Brain::GetNeuron(NeuronId neuronId, NeuropilId neuropilId)
{
    Neuropil* container = GetNeuropil(neuropilId);
    if (container)
    {
        Neuron* ret = container->GetNeuron(neuronId);
        if (ret)
        {
            return ret;
        }
    }
    Warn("Could not find neuron %s within %s", NeuropilTracker::Instance().NameFromId(neuronId) europilTracker::Instance().NameFromId(neuropilId));
    return NULL;
}
unsigned int Brain::GetNumNeurons() const
{
    return m_neuropils.size();
}
Neurons* Brain::GetAllNeurons()
{
    return &m_allNeurons;
}
Synapse* Brain::CreateSynapse(Name name)
{
    Synapse* ret = new Synapse(name);
    StoreSynapse(ret);
    return ret;
}
Synapse* Brain::CreateSynapseBasedOn(SynapseId toCopyId, Name newName)
{
    SynapseId newId = SynapseTracker::Instance().IdFromName(newName);
    if (GetSynapse(newId))
    {
        Error("Error cloning synapse %s: name already taken: %s", SynapseTracker::Instance().NameFromId(toCopyId), newName);
        return NULL;
    }
    Synapse* synapseToCopy = GetSynapse(toCopyId);
    if (!ctionToCopy)
    {
        Error("Error cloning synapse %s: sourceNeuron not found", SynapseTracker::Instance().NameFromId(toCopyId));
        return NULL;
    }
    Synapse* ret = synapseToCopy->Clone();
    ret->SetId(newId);
    StoreSynapse(ret);
    return ret;
}
SynapseId Brain::StoreSynapse(Synapse* synapseToStore)
{
    if (ctionToStore->GetId() == SynapseTracker::InvalidId())
    {
        Error("Can't store a synapse lacking a valid Id");
        return SynapseTracker::InvalidId();
    }

    if (GetIteratorFor(ctionToStore->GetId()) != m_synapses.end())
    {
        Warn("Synapse %u is already stored", synapseToStore->GetId());
        return synapseToStore->GetId();
    }
    
    if (!ctionToStore->IsRegistered())
    {
        synapseToStore->Register(m_modApp);
    }
    synapseToStore->SetLogEngine(GetLogEngine());
    m_synapses.push_back(ctionToStore);
    return synapseToStore->GetId();
}
Synapse* Brain::GetSynapse(const SynapseId cid)
{
    for (Synapses::iterator syn = m_synapses.begin(); syn != m_synapses.end(); ++syn)
    {
        if ((*syn)->GetId() == cid)
        {
            return *syn;
        }
    }
    return NULL;
}
Neuropil* Brain::CreateNeuropil(Name name)
{
    if (!GetNeuropil(name, false))
    {
        Neuropil* createdNeuropil = new Neuropil(name);
        createdNeuropil->SetLogEngine(GetLogEngine());
        m_neuropils.push_back(createdNeuropil);
        Debug("Created neuropil %s", name);
        return createdNeuropil;
    }
    else
    {
        Error("Neuropil %s already exists in the Brain and will not be added.", name);
    }
    return NULL;
}
Neuropils* Brain::GetNeuropils()
{
    return &m_neuropils;
}
unsigned int Brain::GetNumNeuropils()
{
    return m_neuropils.size();
}
Neuropil* Brain::GetNeuropil(NeuropilId neuropilId, bool warn)
{
    for (Neuropils::iterator neuropil = m_neuropils.begin(); neuropil != m_neuropils.end(); ++neuropil)
    {
        if (*(*neuropil) == neuropilId)
        {
            return *neuropil;
        }
    }
    if (warn)
    {
        Warn("Could not find neuropil %s", NeuropilTracker::Instance().NameFromId(neuropilId));
    }
    return NULL;
}
void Brain::DeleteNeuropils()
{
    for (Neuropils::iterator neuropil = m_neuropils.begin(); neuropil != m_neuropils.end(); ++neuropil)
    {
        bioassert(*neuropil);
        if ((*neuropil)->Size())
        {
            Error("Cannot delete non-empty neuropil %s", (*neuropil)->GetName());
            continue;
        }
        delete *neuropil;
    }
    m_neuropils.clear();
}

bool Brain::FilterSet(log::Filt::Plasmid plasmid, log::Filt::Level level)
{
    if (!GetLogEngine())
    {
        return false;
    }
    return GetLogEngine()->FilterSet(plasmid, level);
}
bool Brain::FilterSet(const char* plasmid, const char* level)
{
    if (!GetLogEngine())
    {
        return false;
    }
    return GetLogEngine()->FilterSet(plasmid, level);
}

void Brain::LogSynapses()
{
    Debug("There are %lu synapses", m_synapses.size());
    for (Synapses::iterator syn = m_synapses.begin(); syn != m_synapses.end(); ++syn)
    {
        Debug("%u: %s", (*syn)->GetId(), (*syn)->GetName());
    }
}
void Brain::LogNeurons()
{
    Debug("There are %lu neurons", m_allNeurons.size());
    for (Neurons::iterator nrn = m_allNeurons.begin(); nrn != m_allNeurons.end(); ++nrn)
    {
        Debug("Neuron: %s", (*nrn)->GetName());
    }
}
void Brain::LogNeuropils()
{
    Debug("There are %lu neuropils", m_neuropils.size());
    for (Neuropils::iterator neuropil = m_neuropils.begin(); neuropil != m_neuropils.end(); ++neuropil)
    {
        Debug("Neuropil: %s (%lu neurons)", (*neuropil)->GetName(), (*neuropil)->Size());
    }
}

void Brain::SetPollPeriod(TimeMS period)
{
    for (Neuropils::iterator neuropil = m_neuropils.begin(); neuropil != m_neuropils.end(); ++neuropil)
    {
        (*neuropil)->SetPollPeriod(period);
    }
    PollObject::SetPollPeriod(period);
}
void Brain::SetPollPeriod(TimeMS period, NeuropilId neuropilId)
{
    Neuropil* toSet = GetNeuropil(neuropilId);
    if (!toSet)
    {
        return;
    }
    toSet->SetPollPeriod(period);
}

Timestamp Brain::StaggerPolling(TimeMS offset, Timestamp startTime)
{
    if (!startTime)
    {
        startTime = time::GetCurrentTimestamp();
    }
    for (Neuropils::iterator neuropil = m_neuropils.begin(); neuropil != m_neuropils.end(); ++neuropil)
    {
        startTime = (*neuropil)->StaggerPolling(offset, startTime);
    }
    return startTime;
}
Timestamp Brain::StaggerPolling(TimeMS offset, NeuropilId neuropilId, Timestamp startTime)
{
    if (!startTime)
    {
        startTime = time::GetCurrentTimestamp();
    }
    Neuropil* toSet = GetNeuropil(neuropilId);
    if (!toSet)
    {
        return startTime;
    }
    startTime = toSet->StaggerPolling(offset, startTime);
    return startTime;
}

void Brain::DoNotPoll(Neuron* neuron)
{
    Neurons::iterator nrn = m_neuronsToPoll.begin();
    for (; nrn != m_neuronsToPoll.end(); ++nrn)
    {
        if (*nrn == neuron)
        {
            break;
        }
    }
    if (nrn != m_neuronsToPoll.end())
    {
        m_neuronsToPoll.erase(nrn);
    }
}
void Brain::DoNotPoll(NeuronId neuronId, NeuropilId neuropilId)
{
    DoNotPoll(GetNeuron(neuronId, neuropilId));
}
void Brain::DoNotPoll(NeuropilId neuropilId)
{
    Neuropil* neuropil = GetNeuropil(neuropilId);
    if (!neuropil)
    {
        return;
    }
    for (Neurons::iterator nrn = neuropil->GetAllNeurons()->begin(); nrn != neuropil->GetAllNeurons()->end(); ++nrn)
    {
        DoNotPoll(*nrn);
    }
}

//Helper Function definitions
void Brain::AddEpitopeToAllNeurons(Epitope trait)
{
    for (Neurons::iterator nrn = GetAllNeurons()->begin(); nrn != GetAllNeurons()->end(); ++nrn)
    {
        (*nrn)->AddEpitope(trait);
    }
}

bool Brain::AddEpitopeToAllNeuronsIn(Epitope trait, NeuropilId neuropilId)
{
    Neuropil* neuropil = GetNeuropil(neuropilId);
    if (!neuropil)
    {
        return false;
    }
    for (Neurons::iterator nrn = neuropil->GetAllNeurons()->begin(); nrn != neuropil->GetAllNeurons()->end(); ++nrn)
    {
        (*nrn)->AddEpitope(trait);
    }
    return true;
}

void Brain::RemoveEpitopeFromAllNeurons(const Epitope& trait)
{
    for (Neurons::iterator nrn = GetAllNeurons()->begin(); nrn != GetAllNeurons()->end(); ++nrn)
    {
        (*nrn)->RemoveEpitope(trait);
    }
}

bool Brain::RemoveEpitopeFromAllNeuronsIn(const Epitope& trait, NeuropilId neuropilId)
{
    Neuropil* neuropil = GetNeuropil(neuropilId);
    if (!neuropil)
    {
        return false;
    }
    for (Neurons::iterator nrn = neuropil->GetAllNeurons()->begin(); nrn != neuropil->GetAllNeurons()->end(); ++nrn)
    {
        (*nrn)->RemoveEpitope(trait);
    }
    return true;
}

Synapse* Brain::GetSynapse(Neuron* from, Neuron* to, SynapseId cid)
{
    if (!from)
    {
        return NULL;
    }
    return from->GetSynapseTo(to, cid);
}

Synapse* Brain::GetSynapse(NeuronId fromNeuronId, NeuronId toNeuronId, SynapseId cid)
{
    return GetSynapse(GetNeuron(fromNeuronId), GetNeuron(toNeuronId), cid);
}
Synapse* Brain::Connect(Neuron* from, Neuron* to, Synapse* synapseType)
{
    if (!from) 
    {
        Error("Failed to create Synapse: No sourceNeuron");
        return NULL;
    }
    if (!to)
    {
        Error("Failed to create Synapse: No targetNeuron");
        return NULL;
    }
    if (from == to)
    {
        Error("Failed to create Synapse: Cannot connect a neuron to itself");
        return NULL;
    }
    if (!ctionType)
    {
        Error("Failed to create Synapse: Invalid Synapse type");
        return NULL;
    }
    Synapse* ret = from->ConnectTo(to, synapseType);
    if (ret)
    {
        return ret;
    }
    else
    {
        Error("Failed to create Synapse");
        return NULL;
    }
}
Synapse* Brain::Connect(Neuron* from, Neuron* to, SynapseId cid)
{
    return Connect(from, to, GetSynapse(cid));
}

Synapse* Brain::Connect(NeuronId sourceNeuronId, NeuropilId sourceNeuropilId euronId targetNeuronId europilId targetNeuropilId, SynapseId cid)
{
    Synapse* ret = Connect(GetNeuron(sourceNeuronId, sourceNeuropilId), GetNeuron(targetNeuronId, targetNeuropilId), GetSynapse(cid));
    if (!ret) //catch block
    { 
        Error("    Arguments were: %s (%s) to %s (%s) with CID %u", sourceNeuronId, sourceNeuropilId, targetNeuronId, targetNeuropilId, cid);
    }
    return ret;
}

Synapse* Brain::Connect(NeuronId sourceNeuronId, NeuropilId sourceNeuropilId euronId targetNeuronId europilId targetNeuropilId, Synapse* synapseType)
{
    Synapse* ret = Connect(GetNeuron(sourceNeuronId, sourceNeuropilId), GetNeuron(targetNeuronId, targetNeuropilId), synapseType);
    if (!ret) //catch block
    {
        Error("    Arguments were: %s (%s) to %s (%s) with Synapse %p", sourceNeuronId, sourceNeuropilId, targetNeuronId, targetNeuropilId, synapseType);
    }
    return ret;
}

Synapse* Brain::Connect(NeuronId sourceNeuronId, NeuronId targetNeuronId, Synapse* synapseType)
{
    Synapse* ret = Connect(GetNeuron(sourceNeuronId), GetNeuron(targetNeuronId), synapseType);
    if (!ret) //catch block
    {
        Error("    Arguments were: %s to %s with Synapse %p", sourceNeuronId, targetNeuronId, synapseType);
    }
    return ret;
}

Synapse* Brain::Connect(NeuronId sourceNeuronId, NeuronId targetNeuronId, SynapseId cid)
{
    Synapse* ret =  Connect(GetNeuron(sourceNeuronId), GetNeuron(targetNeuronId), GetSynapse(cid));
    if (!ret) //catch block
    {
        Error("    Arguments were: %s to %s with CID %u", sourceNeuronId, targetNeuronId, cid);
    }
    return ret;
}

bool Brain::ConnectNeuropils(NeuropilId sourceNeuropilId, NeuropilId targetNeuropilId, SynapseId cid)
{
    Neuropil* sourceNeuropil = GetNeuropil(sourceNeuropilId);
    if (!sourceNeuropil)
    {
        return false;
    }
    Neuropil* targetNeuropil = GetNeuropil(targetNeuropilId);
    if (!targetNeuropil)
    {
        return false;
    }

    for (Neurons::iterator src = sourceNeuropil->GetAllNeurons()->begin(); src != sourceNeuropil->GetAllNeurons()->end(); ++src)
    {
        for (Neurons::iterator tgt = targetNeuropil->GetAllNeurons()->begin(); tgt != targetNeuropil->GetAllNeurons()->end(); ++tgt)
        {
            if (*src == *tgt)
            {
                continue;
            }
            Connect(*src, *tgt, cid);
        }
    }
    return true;
}

bool Brain::ConnectNeuropilTo(NeuropilId sourceNeuropilId, NeuronId targetNeuronId europilId targetNeuropilId, SynapseId cid)
{
    Neuropil* sourceNeuropil = GetNeuropil(sourceNeuropilId);
    if (!sourceNeuropil)
    {
        return false;
    }
    Neuron* targetNeuron = GetNeuron(targetNeuronId, targetNeuropilId);
    if (!targetNeuron)
    {
        return false;
    }

    for (Neurons::iterator src = sourceNeuropil->GetAllNeurons()->begin(); src != sourceNeuropil->GetAllNeurons()->end(); ++src)
    {
        Connect(*src, targetNeuron, cid);
    }
    return true;
}

bool Brain::ConnectToNeuropil(NeuronId neuronSourceId, NeuropilId sourceNeuropilId europilId targetNeuropilId, SynapseId cid)
{
    Neuron* sourceNeuron = GetNeuron(neuronSourceId, sourceNeuropilId);
    if (!sourceNeuron)
    {
        return false;
    }
    Neuropil* targetNeuron = GetNeuropil(targetNeuropilId);
    if (!targetNeuron)
    {
        return false;
    }

    for (Neurons::iterator tgt = targetNeuron->GetAllNeurons()->begin(); tgt != targetNeuron->GetAllNeurons()->end(); ++tgt)
    {
        Connect(sourceNeuron, *tgt, cid);
    }
    return true;
}

bool Brain::CreateSynapseBetweenAllNeuronsIn(NeuropilId neuropilId, SynapseId cid)
{
    return ConnectNeuropils(neuropilId, neuropilId, cid);
}

bool Brain::RemoveEpitopeFromSynapse(molecular::Epitope toRemove, NeuronId fromNeuronId euronId toNeuronId, SynapseId cid)
{
    Synapse* synapse = GetSynapse(fromNeuronId, toNeuronId, cid);
    if (!synapse)
    {
        return false;
    }
    synapse->RemoveEpitope(toRemove);
    return true;
}
bool Brain::AddEpitopeToSynapse(molecular::Epitope toAdd, NeuronId fromNeuronId euronId toNeuronId, SynapseId cid)
{
    Synapse* synapse = GetSynapse(fromNeuronId, toNeuronId, cid);
    if (!synapse)
    {
        return false;
    }
    synapse->AddEpitope(toAdd);
    return true;
}

bool Brain::AddEpitopeToAllSynapsesFrom(molecular::Epitope toAdd, NeuropilId neuropilId)
{
    Neuropil* neuropil = GetNeuropil(neuropilId);
    if (!neuropil)
    {
        return false;
    }
    for (Neurons::iterator src = neuropil->GetAllNeurons()->begin(); src != neuropil->GetAllNeurons()->end(); ++src)
    {
        Synapses synapsesOut = (*src)->GetAllOutgoingSynapses();
        for (Synapses::iterator syn = synapsesOut.begin(); syn != synapsesOut.end(); ++syn)
        {
            (*syn)->AddEpitope(toAdd);
        }
    }
    return true;
}

bool Brain::AddEpitopeToAllSynapsesFromTo(molecular::Epitope toAdd, NeuropilId sourceNeuropilId europilId targetNeuropil)
{
    Neuropil* neuropil = GetNeuropil(sourceNeuropilId);
    if (!neuropil)
    {
        return false;
    }
    for (Neurons::iterator src = neuropil->GetAllNeurons()->begin(); src != neuropil->GetAllNeurons()->end(); ++src)
    {
        Synapses synapsesOut = (*src)->GetAllOutgoingSynapses();
        for (Synapses::iterator syn = synapsesOut.begin(); syn != synapsesOut.end(); ++syn)
        {
            Neuron* targetNeuron = (*syn)->GetTargetNeuron();
            if (!targetNeuron)
            {
                continue;
            }
            if (targetNeuron->DoesNeuropilHaveAccess(targetNeuropil))
            {
                (*syn)->AddEpitope(toAdd);
            }
        }
    }
    return true;
}

bool Brain::SetForSynapse(cellular::Cell::MolecularValueId id, cellular::Cell::MolecularValue value, NeuronId fromNeuronId euronId toNeuronId, SynapseId cid)
{
    Synapse* synapse = GetSynapse(fromNeuronId, toNeuronId, cid);
    if (!synapse)
    {
        return false;
    }
    synapse->Set(id, value);
    return true;
}

bool Brain::SetForAllSynapsesFrom(cellular::Cell::MolecularValueId id, cellular::Cell::MolecularValue value, NeuropilId neuropilId, SynapseId cid)
{
    Neuropil* neuropil = GetNeuropil(neuropilId);
    if (!neuropil)
    {
        return false;
    }
    for (Neurons::iterator nrn = neuropil->GetAllNeurons()->begin(); nrn != neuropil->GetAllNeurons()->end(); ++nrn)
    {
        Synapses synapsesOut = (*nrn)->GetAllOutgoingSynapses();
        for (Synapses::iterator syn = synapsesOut.begin(); syn != synapsesOut.end(); ++syn)
        {
            if ((*syn)->GetId() != cid)
            {
                continue;
            }
            (*syn)->Set(id, value);
        }
    }
    return true;
}

bool Brain::SetForAllSynapsesTo(cellular::Cell::MolecularValueId id, cellular::Cell::MolecularValue value, NeuropilId neuropilId, SynapseId cid)
{
    Neuropil* neuropil = GetNeuropil(neuropilId);
    if (!neuropil)
    {
        return false;
    }
    for (Neurons::iterator nrn = neuropil->GetAllNeurons()->begin(); nrn != neuropil->GetAllNeurons()->end(); ++nrn)
    {
        Synapses synapsesIn = (*nrn)->GetAllIncomingSynapses();
        for (Synapses::iterator syn = synapsesIn.begin(); syn != synapsesIn.end(); ++syn)
        {
            if ((*syn)->GetId() != cid)
            {
                continue;
            }
            (*syn)->Set(id, value);
        }
    }
    return true;
}

bool Brain::SetForAllSynapsesFromTo(cellular::Cell::MolecularValueId id, cellular::Cell::MolecularValue value, NeuropilId sourceNeuropilId europilId targetNeuropilId, SynapseId cid)
{
    Neuropil* sourceNeuron = GetNeuropil(sourceNeuropilId);
    if (!sourceNeuron)
    {
        return false;
    }
    Neuropil* targetNeuron = GetNeuropil(targetNeuropilId);
    if (!targetNeuron)
    {
        return false;
    }
    for (Neurons::iterator src = sourceNeuron->GetAllNeurons()->begin(); src != sourceNeuron->GetAllNeurons()->end(); ++src)
    {
        for (Neurons::iterator tgt = targetNeuron->GetAllNeurons()->begin(); tgt != targetNeuron->GetAllNeurons()->end(); ++tgt)
        {
            Synapses synapses = (*src)->GetSynapsesTo(*tgt);
            for (Synapses::iterator syn = synapses.begin(); syn != synapses.end(); ++syn)
            {
                if ((*syn)->GetId() != cid)
                {
                    continue;
                }
                (*syn)->Set(id, value);
            }
        }
    }
    return true;
}

bool Brain::SetFor(cellular::Cell::MolecularValueId id, cellular::Cell::MolecularValue value, NeuronId neuronId europilId neuropilId)
{
    Neuron* storeIn = GetNeuron(neuronId, neuropilId);
    if (!storeIn)
    {
        return false;
    }
    storeIn->Set(id, value);
    return true;
}

void Brain::SetForAllNeurons(cellular::Cell::MolecularValueId id, cellular::Cell::MolecularValue value)
{
    for (Neurons::iterator nrn = GetAllNeurons()->begin(); nrn != GetAllNeurons()->end(); ++nrn)
    {
        (*nrn)->Set(id,value);
    }
}

bool Brain::SetForAllNeuronsIn(cellular::Cell::MolecularValueId id, cellular::Cell::MolecularValue value, NeuropilId neuropilId)
{
    Neuropil* neuropil = GetNeuropil(neuropilId);
    if (!neuropil)
    {
        return false;
    }
    for (Neurons::iterator nrn = neuropil->GetAllNeurons()->begin(); nrn != neuropil->GetAllNeurons()->end(); ++nrn)
    {
        (*nrn)->Set(id,value);
    }
    return true;
}

NeuropilId Brain::DuplicateNeuropil(NeuropilId sourceNeuropilId, Name placeIn)
{
    Neuropil* sourceNeuron = GetNeuropil(sourceNeuropilId);
    if (!sourceNeuron)
    {
        return NeuropilTracker::InvalidId();
    }

    Neuropil* dest = GetNeuropil(placeIn, false);
    if (!dest)
    {
        dest = CreateNeuropil(placeIn);
    }

    for (Neurons::iterator nrn = sourceNeuron->GetAllNeurons()->begin(); nrn != sourceNeuron->GetAllNeurons()->end(); ++nrn)
    {
        Neuron* wta = StoreNeuron((*nrn)->Clone(), {placeIn});
        wta->RemovedFromNeuropil(sourceNeuron->GetId());
    }
    return dest->GetId();
}

NeuropilId Brain::CreateWinnerTakeAllFor(NeuropilId sourceNeuropilId, Name placeIn, SynapseId excitatoryId, SynapseId inhibitoryId)
{
    NeuropilId targetNeuropilId = DuplicateNeuropil(sourceNeuropilId, placeIn);
    if (!CreateOneToOneSynapsesBetween(sourceNeuropilId, targetNeuropilId, excitatoryId))
    {
        return false;
    }
    if (!CreateSynapseBetweenAllNeuronsIn(targetNeuropilId, inhibitoryId))
    {
        return false;
    }
    return true;
}

bool Brain::CreateOneToOneSynapsesBetween(NeuropilId sourceNeuropilId, NeuropilId targetNeuropilId, SynapseId cid)
{
    Neuropil* sourceNeuropil = GetNeuropil(sourceNeuropilId);
    if (!sourceNeuropil)
    {
        return false;
    }
    Neuropil* targetNeuropil = GetNeuropil(targetNeuropilId);
    if (!targetNeuropil)
    {
        return false;
    }
    if (sourceNeuropil == targetNeuropil)
    {
        return false;
    }
    if (sourceNeuropil->Size() != targetNeuropil->Size())
    {
        return false;
    }
    Synapse* synapse = GetSynapse(cid);
    if (!synapse)
    {
        return false;
    }

    for (unsigned int i = 0; i < sourceNeuropil->Size(); ++i)
    {
        (*sourceNeuropil)[i]->ConnectTo((*targetNeuropil)[i], synapse);
    }
    return true;
}

bool Brain::CreateRandomSynapsesBetween(NeuropilId sourceNeuropilId, NeuropilId targetNeuropilId, unsigned int p, SynapseId cid)
{
    Neuropil* sourceNeuropil = GetNeuropil(sourceNeuropilId);
    if (!sourceNeuropil)
    {
        return false;
    }
    Neuropil* targetNeuropil = GetNeuropil(targetNeuropilId);
    if (!targetNeuropil)
    {
        return false;
    }

    Synapse* synapse = GetSynapse(cid);
    if (!synapse)
    {
        return false;
    }

    for (Neurons::iterator src = sourceNeuropil->GetAllNeurons()->begin(); src != sourceNeuropil->GetAllNeurons()->end(); ++src)
    {
        for (Neurons::iterator tgt = targetNeuropil->GetAllNeurons()->begin(); tgt != targetNeuropil->GetAllNeurons()->end(); ++tgt)
        {
            unsigned int randVal = rand() % 100;
            if (randVal <= p)
            {
                (*src)->ConnectTo(*tgt, synapse);
            }
        }
    }

    return true;
}
