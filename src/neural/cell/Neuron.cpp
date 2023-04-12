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

#include "bio/neural/Neuron.h"
#include "bio/neural/Synapse.h"
#include "bio/common/ReturnCodes.h"
#include "bio/common/Time.h"
#include "bio/cellular/Protein.h"

#include <string.h>
#include <climits>

BIO_NEURON_TRACK_FIRING_CONDITION(None)
BIO_NEURON_TRACK_FIRING_CONDITION(Any)
BIO_NEURON_TRACK_FIRING_CONDITION(RisingEdge)
BIO_NEURON_TRACK_FIRING_CONDITION(Continuing)
BIO_NEURON_TRACK_FIRING_CONDITION(FallingEdge)

using namespace bio;

Neuron::Neuron(Name name) : Named(name, &NeuronTracker::Instance()), m_prePoll(NULL), m_poll(NULL), m_preSend(NULL), m_postSend(NULL), m_learn(NULL), m_lastActive(0), m_isActive(false), m_firingReason(FiringConditionTracker::InvalidId()), m_continuingImpulse(false), m_persistFor(0)
{
    SetLogFilter(log::Filt::NEURON);

    AddTrait("Neuron");

    CreateTopLevelProtein("PreCrest");
    CreateTopLevelProtein("Poll");
    CreateTopLevelProtein("PreSend");
    CreateTopLevelProtein("PostSend");
    CreateTopLevelProtein("Learn");

    CreateImpulse(fc::RisingEdge(), "RisingEdge");
    CreateImpulse(fc::Continuing(), "ContinuingImpulse");
    CreateImpulse(fc::FallingEdge(), "FallingEdge", false);
}

Neuron::Neuron(const Neuron& rhs) : Named(rhs), cellular::Cell(rhs), m_lastActive(0), m_isActive(false), m_firingReason(FiringConditionTracker::InvalidId()), m_continuingImpulse(rhs.m_continuingImpulse), m_persistFor(rhs.m_persistFor)
    //Cell copy ctor takes care of proteins. They will be junk until transcribed.
{
    //TODO: Consider CommonCtor()
    SetLogFilter(log::Filt::NEURON);
    AddTrait("Neuron");
}

Neuron::~Neuron() 
{
    Debug("Destroying %s", GetName());
    for (Synapses::iterator syn = m_outgoingSynapses.begin(); syn != m_outgoingSynapses.end(); ++syn)
    {
        delete (*syn);
        *syn = NULL;
    }
    m_outgoingSynapses.clear();
}
Neuron* Neuron::Clone() const
{
    return new Neuron(*this);
}

bool Neuron::Register(organism::Organism* applicator)
{
    //TODO: If this is still default, remove it.
    return cellular::Cell::Register(applicator);
}
bool Neuron::DoesNeuropilHaveAccess(NeuropilId neuropilId) const
{
    NeuropilIds::const_iterator npl = FindContainer(neuropilId);
    if (npl == m_containers.end())
    {
        return false;
    }
    return true;
}
NeuropilIds Neuron::GetNeuropilsWithAccess() const
{
    return m_containers;
}
unsigned int Neuron::GetNumNeuropilsWtihAccess() const
{
    return m_containers.size();
}
void Neuron::AddedToNeuropil(NeuropilId neuropilId)
{
    if (DoesNeuropilHaveAccess(neuropilId))
    {
        return;
    }
    m_containers.push_back(neuropilId);
}
void Neuron::RemovedFromNeuropil(NeuropilId neuropilId)
{
    NeuropilIds::iterator npl = FindContainer(neuropilId);
    if (npl != m_containers.end()) 
    {
        m_containers.erase(npl);
    }
}
NeuropilIds::iterator Neuron::FindContainer(NeuropilId neuropilId)
{
    return std::find(m_containers.begin(), m_containers.end(), neuropilId);
}
NeuropilIds::const_iterator Neuron::FindContainer(NeuropilId neuropilId) const
{
    return std::find(m_containers.begin(), m_containers.end(), neuropilId);
}

void Neuron::TranscribeTopLevelProteins()
{
    m_prePoll = GetProtein("PreCrest");
    m_poll = GetProtein("Poll");
    m_preSend = GetProtein("PreSend");
    m_postSend = GetProtein("PostSend");
    m_learn = GetProtein("Learn");

    UpdateImpulseCallers();

    cellular::Cell::TranscribeTopLevelProteins();
}

void Neuron::FoldAllProteins(void* arg)
{
    //Inform("Initializing %s", GetName()); Synapses synapsesOut = GetAllOutgoingSynapses(); for (Synapses::iterator out = synapsesOut.begin(); out != synapsesOut.end(); ++out)
    {
        (*out)->FoldAllProteins(this);
    }
    cellular::Cell::FoldAllProteins(arg);
}

std::string Neuron::PrettyID() const
{
    if (strlen(GetName()) > 20 && m_containers.size()) 
    {
        //Here, the container and the name are both too large, so lets return the first 10 characters of both with a ':' to separate them.
        std::string ret="";
        ret += *m_containers.begin(); //FIXME: This should be a Name, not Id.
        ret = ret.substr(0,10);
        ret += ":";
        std::string name = GetName();
        ret += name.substr(0,10);
        return ret;
    }
    else 
    {
        return GetName();
    }
}

void Neuron::PreCrest()
{
    StandardActivate(m_prePoll);
}

void Neuron::Poll()
{
    // Debug("Polling %s", GetName());
    Timestamp now = time::GetCurrentTimestamp();
    if (m_pollPeriodMS + m_lastPollTimestamp > now)
    {
        return;
    }
    m_lastPollTimestamp = now;

    PreCrest();
    ProcessAllSynapses();
    Crest();
}

void Neuron::Crest()
{
    StandardActivate(m_poll);
}

void Neuron::UpdateImpulseCallers() 
{
    for(fc::Impulses::iterator imp = m_impulses.begin(); imp != m_impulses.end(); ++imp)
    {
        imp->SetCaller(this);
    }
}

void Neuron::CreateImpulse(FiringConditionId trigger, Name impulseName, bool shouldBeActive)
{
    if (GetImpulse(impulseName))
    {
        return;
    }
    fc::Impulse toAdd(trigger, impulseName, this, shouldBeActive);
    m_impulses.push_back(toAdd);
}

Neuron::fc::Impulse* Neuron::GetImpulse(Name impulseName)
{
    fc::Impulses::iterator imp = GetImpulseIttr(impulseName);
    if (imp == m_impulses.end())
    {
        return NULL;
    }
    return &*imp;
}
void Neuron::RemoveImpulse(Name impulseName) 
{
    fc::Impulses::iterator imp = GetImpulseIttr(impulseName);
    if (imp != m_impulses.end())
    {
        m_impulses.erase(imp);
    }
}

void Neuron::EnableImpulse(Name impulseName)
{
    fc::Impulse* imp = GetImpulse(impulseName);
    if (!imp)
    {
        return;
    }
    cellular::Protein* act = imp->GetImpulseProtein();
    if (!act)
    {
        return;
    }
    act->Enable();
}
void Neuron::DisableImpulse(Name impulseName)
{
    fc::Impulse* imp = GetImpulse(impulseName);
    if (!imp)
    {
        return;
    }
    cellular::Protein* act = imp->GetImpulseProtein();
    if (!act)
    {
        return;
    }
    act->Disable();
}
bool Neuron::IsImpulseEnabled(Name impulseName)
{
    fc::Impulse* imp = GetImpulse(impulseName);
    if (!imp)
    {
        return false;
    }
    cellular::Protein* act = imp->GetImpulseProtein();
    if (!act)
    {
        return false;
    }
    return act->IsEnabled();
}

Neuron::fc::Impulses::iterator Neuron::GetImpulseIttr(Name impulseName)
{
    for(fc::Impulses::iterator imp = m_impulses.begin(); imp != m_impulses.end(); ++imp)
    {
        if (strcmp(imp->GetImpulseName(), impulseName) == 0)
        {
            return imp;
        }
    }
    return m_impulses.end();
}
Neuron::fc::Impulses::const_iterator Neuron::GetImpulseIttr(Name impulseName) const
{
    for(fc::Impulses::const_iterator imp = m_impulses.begin(); imp != m_impulses.end(); ++imp)
    {
        if (strcmp(imp->GetImpulseName(), impulseName) == 0)
        {
            return imp;
        }
    }
    return m_impulses.end();
}

bool Neuron::DetermineAndCallImpulseTriggers()
{
    bool ret = false;
    
    if (!DetermineImpulseTriggers(ExciteTrigger()))
    {
        return ret;
    }
    
    for (fc::Impulses::iterator imp = m_impulses.begin(); imp < m_impulses.end(); ++imp)
    {
        if (GetReasonForFiring() == imp->GetTrigger()) 
        {
            //TODO: is this the correct return value?
            ret = imp->Send();
        }
    }
    
    return ret;
}

Neuron::FiringConditionId Neuron::GetReasonForFiring() const
{
    return m_firingReason;
}

void Neuron::StartAtRisingEdge()
{
    m_continuingImpulse=false;
}
bool Neuron::IsPersisting(const Timestamp now) const
{
    return now < m_lastActive + m_persistFor;
}
void Neuron::PersistFor(const TimeMS ms)
{
    m_persistFor = ms;
}
void Neuron::PersistUntil(const Timestamp time)
{
    m_persistFor = time - time::GetCurrentTimestamp();
}

void Neuron::ActiveNow()
{
    Timestamp now = time::GetCurrentTimestamp();
    if (now > m_lastActive)
    {
        m_lastActive = now;
    }
}
void Neuron::ActiveUntil(Timestamp time)
{
    m_lastActive = time;
}
Timestamp Neuron::GetTimeLastActive() const
{
    if (m_isActive)
    {
        return time::GetCurrentTimestamp();
    }
    return m_lastActive;
}
bool Neuron::IsActive() const
{
    return m_isActive;
}

void Neuron::Learn()
{
    StandardActivate(m_learn);
}

void Neuron::AddIncomingSynapse(Synapse* synapse)
{
    Synapses::iterator syn = m_incomingSynapses.begin();
    for (; syn!=m_incomingSynapses.end(); ++syn) 
    {
        if (*syn == synapse)
        {
            return; //already exists
        }
    }
    m_incomingSynapses.push_back(synapse);
}

void Neuron::RemoveIncomingSynapse(Synapse* synapse)
{
    Synapses::iterator iter;
    for (iter = m_incomingSynapses.begin(); iter != m_incomingSynapses.end();)
    {
        if (*iter == synapse) 
        {
            //Remove the pointer to the learning engine from *this.
            iter = m_incomingSynapses.erase(iter);
        }
        else 
        {
            ++iter;
        }
    }
}

Synapses Neuron::GetAllIncomingSynapses() const
{
    return m_incomingSynapses;
}

unsigned int Neuron::GetIncomingSynapsesSize() const
{
    unsigned int ret = m_incomingSynapses.size();
    Inform("%s has %u incoming synapses", GetName(), ret); return ret; }

unsigned int Neuron::GetIncomingSynapsesSize(const cellular::Cell::Traits& selection) const
{
    unsigned int ret = 0;
    for (Synapses::const_iterator syn = m_incomingSynapses.begin(); syn != m_incomingSynapses.end(); ++syn)
    {
        if ((*syn)->DoTraitsContain(selection))
        {
            ++ret;
        }
    }
    Inform("%s has %u incoming %s synapses (out of %u)", Name(), ret, cellular::Cell::ConvertTraitsToString(selection).c_str(), GetIncomingSynapsesSize()); 
    return ret;
}

unsigned int Neuron::GetNumAddedSynapsesOf(const cellular::Cell::Traits& selection) const
{
    unsigned int ret = 0;
    for (Synapses::const_iterator syn = m_incomingSynapses.begin(); syn != m_incomingSynapses.end(); ++syn)
    {
        if ((*syn)->IsAdded() && (*syn)->DoTraitsContain(selection))
        {
            ++ret;
        }
    }
    return ret;
}

unsigned int Neuron::GetNumAddedSynapsesOf(const AspectId aspect) const
{
    unsigned int ret = 0;
    for (Synapses::const_iterator syn = m_incomingSynapses.begin(); syn != m_incomingSynapses.end(); ++syn)
    {
        if ((*syn)->IsAdded() && (*syn)->DoesAffect(aspect))
        {
            ++ret;
        }
    }
    return ret;
}

Synapse* Neuron::ConnectTo(Neuron* target, const Synapse* synapseType)
{
    if (!target) 
    {
        Error("%s failed to create a synapse due to: invalid target", GetName());
        return NULL;
    }
    if (!ctionType) 
    {
        Error("%s failed to create a synapse due to: no synapse to configure", GetName());
        return NULL;
    }

    for (Synapses::iterator syn = m_outgoingSynapses.begin(); syn!=m_outgoingSynapses.end(); ++syn)
    {
        if ((*syn)->GetTargetNeuron() == target && (*syn)->GetId() == synapseType->GetId())
        {
            Error("%s failed to create a synapse due to: already exists", GetName());
            return *syn;
        }
    }
    Synapse* out = synapseType->ConfigureFor(this, target);
    if (!out) 
    {
        Error("Unable to configure synapse between %s and %s", GetName(), target->GetName());
        return NULL;
    }
    out->SetLogEngine(GetLogEngine());
    m_outgoingSynapses.push_back(out);

    Debug("%s created new synapse (%s) to %s", GetName(), out->GetName(), out->GetTargetName());
    return out;
}

void Neuron::RemoveSynapseTo(Neuron* target, SynapseId cid)
{
    for (Synapses::iterator syn = m_outgoingSynapses.begin(); syn != m_outgoingSynapses.end();)
    {
        if ((*syn)->GetTargetNeuron() != target || (*syn)->GetId() != cid) 
        {
            ++syn;
            continue;
        }

        delete *syn;
        *syn = NULL;
        syn = m_outgoingSynapses.erase(syn);
        break;
    }
}
void Neuron::RemoveSynapsesTo(Neuron* target)
{
    for (Synapses::iterator syn = m_outgoingSynapses.begin(); syn != m_outgoingSynapses.end();)
    {
        if ((*syn)->GetTargetNeuron() == target) 
        {
            delete *syn;
            *syn = NULL;
            syn = m_outgoingSynapses.erase(syn);
        }
        else 
        {
            ++syn;
        }
    }
}
void Neuron::RemoveSynapsesTo(Neuron* target, molecular::Epitope purpose)
{
    for (Synapses::iterator syn = m_outgoingSynapses.begin(); syn != m_outgoingSynapses.end();)
    {
        if ((*syn)->GetTargetNeuron() == target && (*syn)->DoesTraitExist(purpose))
        {
            delete *syn;
            *syn = NULL;
            syn = m_outgoingSynapses.erase(syn);
        }
        else 
        {
            ++syn;
        }
    }
}
void Neuron::RemoveSynapsesTo(Neuron* target, cellular::Cell::Traits purpose)
{
    for (Synapses::iterator syn = m_outgoingSynapses.begin(); syn != m_outgoingSynapses.end();)
    {
        if ((*syn)->GetTargetNeuron() == target) 
        {
            bool allUsesFound = true;
            for (cellular::Cell::Traits::iterator use = purpose.begin(); use != purpose.end(); ++use)
            {
                if (!(*syn)->DoesTraitExist(*use))
                {
                    allUsesFound=false;
                }
            }
            if (allUsesFound) 
            {
                delete *syn;
                *syn = NULL;
                syn = m_outgoingSynapses.erase(syn);
            }
            else 
            {
                ++syn;
            }
        }
        else 
        {
            ++syn;
        }
    }
}

Synapse* Neuron::GetSynapseTo(Neuron* target, SynapseId cid)
{
    for (Synapses::iterator syn = m_outgoingSynapses.begin(); syn != m_outgoingSynapses.end(); ++syn)
    {
        if ((*syn)->GetTargetNeuron() == target && *(*syn) == cid)
        {
            return *syn;
        }
    }
    return NULL;
}
Synapses Neuron::GetSynapsesTo(Neuron* target)
{
    Synapses ret;
    for (Synapses::iterator syn = m_outgoingSynapses.begin(); syn != m_outgoingSynapses.end(); ++syn)
    {
        if ((*syn)->GetTargetNeuron() == target)
        {
            ret.push_back(*syn);
        }
    }
    return ret;
}
Synapses Neuron::GetSynapsesTo(Neuron* target, molecular::Epitope purpose)
{
    cellular::Cell::Traits buf;
    buf.push_back(purpose);
    return GetSynapsesTo(target, buf);
}
Synapses Neuron::GetSynapsesTo(Neuron* target, cellular::Cell::Traits purpose)
{
    Synapses ret;
    for (Synapses::iterator syn = m_outgoingSynapses.begin(); syn != m_outgoingSynapses.end(); ++syn)
    {
        if ((*syn)->GetTargetNeuron() == target && (*syn)->DoTraitsContain(purpose))
        {
            ret.push_back(*syn);
        }
    }
    return ret;
}

//THIS CAN BE DANGEROUS!!!
Synapses Neuron::GetAllOutgoingSynapses()
{
    return m_outgoingSynapses;
}

void Neuron::RequestProcessingOf(Synapse* source)
{
    if (source->ShouldBeRemoved())
    {
        Debug("%s: removing %s", Name(), source->GetName());
        RemoveSynapse(source);
    }
    if (source->ShouldBeAdded()) 
    {
        Debug("%s: adding %s", Name(), source->GetName());
        AddSynapse(source);
    }
}
void Neuron::AddSynapse(Synapse* toProcess)
{
    if (toProcess->IsAdded() && !toProcess->AreMultipleAdditionsAllowed()) 
    {
        Warn("%s: cannot add already added synapse %s", Name(), toProcess->Identity().c_str());
        return;
    }

#if 1
    const ReturnCode process = toProcess->ProcessAddition();
    if (process != ret::Success() && process != ret::NoErrorNoSuccess()) 
    {
        Warn("%s: did not successfully add %s", Name(), toProcess->Identity().c_str());
        return;
    }
#else
    toProcess->ProcessAddition();
#endif

    if (toProcess->IsAdded()) 
    {
        //TODO: Is this too much overhead to account for rounding errors?
        const AspectIds& synapseEffects = toProcess->GetEffects();
        for (AspectIds::const_iterator asp = synapseEffects.begin(); asp != synapseEffects.end(); ++asp)
        {
            IncrementCountFor(*GetAspectIttr(*asp));
        }
    }

    //TODO: do we care about the return code?
    toProcess->PostAddition();

    //Debug("%s: Unprocessed data from %s processed" ame(), toProcess->GetDatum()->GetOwnerInfo().Name().c_str());
}
void Neuron::RemoveSynapse(Synapse* toProcess)
{
    if (!toProcess->IsAdded() && !toProcess->AreMultipleRemovalsAllowed()) 
    {
        Warn("%s: cannot remove unadded synapse %s", Name(), toProcess->Identity().c_str());
        return;
    }

#if 1
    const ReturnCode process = toProcess->ProcessRemoval();
    if (process != ret::Success() && process != ret::NoErrorNoSuccess()) 
    {
        Warn("%s: did not successfully remove %s", Name(), toProcess->Identity().c_str());
        return;
    }
#else
    toProcess->ProcessRemoval();
#endif

    if (!toProcess->IsAdded()) 
    {
        //TODO: Is this too much overhead to account for rounding errors?
        const AspectIds& synapseEffects = toProcess->GetEffects();
        for (AspectIds::const_iterator asp = synapseEffects.begin(); asp != synapseEffects.end(); ++asp)
        {
            DecrementCountFor(*GetAspectIttr(*asp));
        }
        ResetAllPossibleAspects();
    }

    //TODO: do we care about the return code?
    toProcess->PostRemoval();

    //Debug("%s: Processed data from %s processed" ame(), toProcess->GetDatum()->GetOwnerInfo().Name().c_str());
}
void Neuron::PreSend()
{
    StandardActivate(m_preSend);
}
void Neuron::TransmitCommon(cellular::Cell::Traits selection, bool any)
{
    unsigned int selectionsFound;
    for (Synapses::iterator syn = m_outgoingSynapses.begin(); syn != m_outgoingSynapses.end(); ++syn)
    {
        //Skip past any synapses that are not specified by selection
        selectionsFound = (*syn)->GetNumTraitsMatching(selection);
        if (selection.size() && ((any && selectionsFound == 0) || (!any && selectionsFound!=selection.size())))
        {
            continue;
        }

        ReturnCode errChk = (*syn)->ProcessOutgoing();
        if (errChk != ret::Success() && errChk != ret::NoErrorNoSuccess()) 
        {
            std::string errorCode = ret::CodeTracker::Instance().NameFromId(errChk);
            Inform("%s: Sending data through Synapse %s to %s failed with return code %s", Name(), (*syn)->GetName(), (*syn)->GetTargetName(), errorCode.c_str());
        }
    }
}
void Neuron::Transmit()
{
    PreSend();
    cellular::Cell::Traits selection; //empty
    TransmitCommon(selection, true);
    PostSend();
}
void Neuron::PostSend()
{
    StandardActivate(m_postSend);
}
void Neuron::ExpireSentData()
{
    for (Synapses::iterator syn = m_outgoingSynapses.begin(); syn != m_outgoingSynapses.end(); ++syn)
    {
        (*syn)->ExpireSignal();
    }
}
void Neuron::TransmitSelectively(cellular::Cell::Traits selection, bool any)
{
    PreSend();
    TransmitCommon(selection, any);
    PostSend();
}
void Neuron::TransmitSelectively(molecular::Epitope selection)
{
    cellular::Cell::Traits selectionBuffer;
    selectionBuffer.push_back(selection);
    TransmitSelectively(selectionBuffer, true);
}
Aspects::iterator Neuron::GetAspectIttr(AspectId aspect)
{
    return std::find(m_aspects.begin(), m_aspects.end(), aspect);
}
Aspects::const_iterator Neuron::GetAspectIttr(AspectId aspect) const
{
    return std::find(m_aspects.begin(), m_aspects.end(), aspect);
}
void Neuron::CreateAspect(AspectId aspect, bool shouldReset)
{
    if (aspect == AspectTracker::InvalidId()) 
    {
        Error("%s: Failed to register invalid aspect", GetName());
        return;
    }
    if (GetAspectIttr(aspect) != m_aspects.end()) 
    {
        Error("%s: Aspect %s already exists", GetName(), AspectTracker::Instance().NameFromId(aspect));
        return;
    }
    m_aspects.push_back(Aspect(AspectTracker::Instance().NameFromId(aspect)));
}
void Neuron::SetAspectReset(const AspectId aspect, bool shouldReset)
{
    Aspects::iterator asp = GetAspectIttr(aspect);
    if (asp == m_aspects.end()) 
    {
        Error("%s: Failed to reset aspect %s", GetName(), AspectTracker::Instance().NameFromId(aspect));
        return;
    }
    asp->SetReset(shouldReset);
}
bool Neuron::ShouldResetAspect(const AspectId aspect) const
{
    Aspects::const_iterator asp = GetAspectIttr(aspect);
    if (asp == m_aspects.end()) 
    {
        Error("%s: Failed to reset aspect %s", GetName(), AspectTracker::Instance().NameFromId(aspect));
        return false;
    }
    return asp->ShouldReset();
}
void Neuron::IncrementCountFor(Aspect aspect)
{
    aspect.IncrementCount();
}
void Neuron::DecrementCountFor(Aspect aspect)
{
    aspect.DecrementCount();
}

void Neuron::CheckForReset(const Aspect aspect)
{
    if (aspect.GetCount() != 0 || !aspect.ShouldReset())
    {
        return;
    }

    // Inform("%s: Aspect %s has %u contributors" ame(), AspectIdToString(aspect).c_str(), GetCountFor(aspect)); Reset(aspect.GetId()); }
unsigned int Neuron::GetCountFor(const AspectId aspect) const
{
    Aspects::const_iterator asp = GetAspectIttr(aspect);
    if (asp == m_aspects.end()) 
    {
        Error("%s: Failed to get count for aspect %s", GetName(), AspectTracker::Instance().NameFromId(aspect)); return UINT_MAX;
    }
    return asp->GetCount();
}
void Neuron::ResetAllPossibleAspects()
{
    for (Aspects::iterator asp = m_aspects.begin(); asp != m_aspects.end(); ++asp)
    {
        CheckForReset(*asp);
    }
}
Synapses Neuron::GetSynapsesFor(const AspectId aspect) const
{
    Synapses ret;
    for (Synapses::const_iterator syn = m_incomingSynapses.begin(); syn != m_incomingSynapses.end(); ++syn)
    {
        if ((*syn)->DoesAffect(aspect))
        {
            ret.push_back(*syn);
        }
    }
    return ret;
}
void Neuron::SetLogEngine(log::Engine* logEngine)
{
    //Incoming synapses should be configured by their senders
    for (Synapses::iterator syn = m_outgoingSynapses.begin(); syn != m_outgoingSynapses.end(); ++syn)
    {
        (*syn)->SetLogEngine(logEngine);
    }
    LoggerObject::SetLogEngine(logEngine);
}
void Neuron::ProcessSynapse(Synapse* synapse)
{
    if (synapse->ShouldBeRemoved())
    {
        Debug("%s: removing %s", Name(), synapse->Identity().c_str());
        RemoveSynapse(synapse);
    }
    if (synapse->ShouldBeAdded()) 
    {
        Debug("%s: adding %s", Name(), synapse->Identity().c_str());
        AddSynapse(synapse);
    }
}
void Neuron::ProcessAllSynapses()
{
    for (Synapses::iterator syn = m_incomingSynapses.begin(); syn != m_incomingSynapses.end(); ++syn)
    {
        ProcessSynapse(*syn);
    }
}
bool Neuron::DetermineImpulseTriggers(bool trigger)
{
    bool persistance = IsPersisting(time::GetCurrentTimestamp());
    if (trigger || persistance) 
    {
        if (!trigger) 
        {
            Debug("%s is active, only due to persistence", Name());
        }
        if (m_continuingImpulse) 
        {
            m_firingReason = fc::Continuing();
        }
        else
        {
            m_firingReason = fc::RisingEdge();
            m_continuingImpulse = true;
        }
        return true;
    }
    else if (m_continuingImpulse) 
    {
        m_firingReason = fc::FallingEdge();
        m_continuingImpulse = false;
        return true;
    }
    return false;
}
void Neuron::RetrieveDataSelectively(cellular::Cell::Traits selection, bool any)
{
    unsigned int selectionsFound;
    for (Synapses::iterator syn = m_outgoingSynapses.begin(); syn != m_outgoingSynapses.end(); ++syn)
    {
        //Skip past any synapses that are not specified by selection
        selectionsFound = (*syn)->GetNumTraitsMatching(selection);
        if (selection.size() && ((any && selectionsFound == 0) || (!any && selectionsFound != selection.size())))
        {
            continue;
        }

        ProcessSynapse(*syn);
    }
}
void Neuron::RetrieveDataSelectively(molecular::Epitope selection, bool any)
{
    cellular::Cell::Traits selectionBuffer; //empty
    selectionBuffer.push_back(selection);
    RetrieveDataSelectively(selectionBuffer, any);
}

void Neuron::SetPersonality(Personality* pe)
{
    m_pe = pe;
}
