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

#include "bio/neural/Synapse.h"
#include "bio/neural/Neuron.h"
#include "bio/common/String.h"

using namespace bio;

Synapse::Synapse(Name name): 
    Named(name, &SynapseTracker::Instance()),
    cellular::Cell("Synapse"),
    m_source(NULL),
    m_target(NULL),
    m_targetID(SynapseTracker::InvalidId()),
    m_id(SynapseTracker::InvalidId()),
    m_isReady(false),
    m_isAdded(false),
    m_areZerosAllowed(false),
    m_doesExpire(false),
    m_allowMultipleAdditions(false),
    m_allowMultipleRemovals(false),
    m_expiration(0),
    m_addAt(0),
    m_addedAt(0),
    m_lastUpdated(0),
    m_callback(NULL),
    m_processOutgoing(NULL),
    m_processAddition(NULL),
    m_postAddition(NULL),
    m_processRemoval(NULL),
    m_postRemoval(NULL),
    m_expireSignal(NULL)
{
    SetLogFilter(log::Filt::CONNECTION);

    AddTrait("Synapse");
    if (m_target)
    {
        m_targetID = m_target->GetId();
    }
    //The sender that owns *this will initialize it.
    OrganismShouldNotFold();

    CreateTopLevelProtein("Callback");
    CreateTopLevelProtein("ProcessOutgoing");
    CreateTopLevelProtein("ProcessAddition");
    CreateTopLevelProtein("PostAddition");
    CreateTopLevelProtein("ProcessRemoval");
    CreateTopLevelProtein("PostRemoval");
    CreateTopLevelProtein("ExpireSignal");
}

Synapse::Synapse(const Synapse& rhs): cellular::Cell(rhs), m_source(NULL), m_target(NULL), m_targetID(0), m_id(rhs.m_id), m_isReady(rhs.m_isReady), m_isAdded(rhs.m_isAdded), m_areZerosAllowed(rhs.m_areZerosAllowed), m_doesExpire(rhs.m_doesExpire), m_allowMultipleAdditions(rhs.m_allowMultipleAdditions), m_allowMultipleRemovals(rhs.m_allowMultipleRemovals), m_expiration(rhs.m_expiration), m_addAt(rhs.m_addAt), m_addedAt(rhs.m_addedAt), m_lastUpdated(rhs.m_lastUpdated)
{
    SetLogEngine(const_cast<log::Engine*>(rhs.GetLogEngine()));

    //The sender that owns *this will initialize it.
    OrganismShouldNotFold();
}

Synapse::~Synapse()
{
}

std::string Synapse::Identity() const
{
    std::string ret = GetSourceName();
    ret += " to ";
    ret += GetTargetName();
    ret += ": ";
    ret += GetName();
    return ret;
}

void Synapse::TranscribeTopLevelProteins()
{
    m_callback = GetProtein("Callback");
    m_processOutgoing = GetProtein("ProcessOutgoing");
    m_processAddition = GetProtein("ProcessAddition");
    m_postAddition = GetProtein("PostAddition");
    m_processRemoval = GetProtein("ProcessRemoval");
    m_postRemoval = GetProtein("PostRemoval");
    m_expireSignal = GetProtein("ExpireSignal");

    cellular::Cell::TranscribeTopLevelProteins();
}

void Synapse::FoldAllProteins(void* arg)
{
    if (!GetTargetNeuron())
    {
        return;
    }
    //Inform("Initializing %s", Identity().c_str()); FoldProtein(m_callback); FoldProtein(m_processAddition); FoldProtein(m_postAddition); FoldProtein(m_processRemoval); FoldProtein(m_postRemoval);

    //TODO: Test this code!
    if (m_processOutgoing->NameOfPlasmidUsedIn() == "NO_MODULE" && m_processOutgoing->GetProteins()->size() == 0)
    {
        if (RemoveProtein(m_processOutgoing) == ret::Success())
        {
            m_processOutgoing = NULL;
        }
        else
        {
            Warn("Could not remove ProcessOutgoing from %s", Identity().c_str());
        }
    }
    else
    {
        FoldProtein(m_processOutgoing);
    }
    if (m_expireSignal->NameOfPlasmidUsedIn() == "NO_MODULE" && m_expireSignal->GetProteins()->size() == 0)
    {
        if (RemoveProtein(m_expireSignal) == ret::Success())
        {
            m_expireSignal = NULL;
        }
        else
        {
            Warn("Could not remove ExpireSignal from %s", Identity().c_str());
        }
    }
    else
    {
        FoldProtein(m_expireSignal);
    }
}
const Neuron* Synapse::GetSourceNeuron() const
{
    return m_source;
}
Name Synapse::GetSourceName() const
{
    if (!GetSourceNeuron())
    {
        return "NO SOURCE NEURON";
    }
    return GetSourceNeuron()->GetName();
}
NeuronId Synapse::GetSourceId() const
{
    if (!GetSourceNeuron())
    {
        return SynapseTracker::InvalidId();
    }
    return GetSourceNeuron()->GetId();
}

Neuron* Synapse::GetTargetNeuron()
{
    return m_target;
}
const Neuron* Synapse::GetTargetNeuron() const
{
    return m_target;
}
Name Synapse::GetTargetName() const
{
    if (!GetTargetNeuron())
    {
        return "NO TARGET NEURON";
    }
    return GetTargetNeuron()->GetName();
}

NeuronId Synapse::GetTargetId() const
{
    if (!GetTargetNeuron())
    {
        return m_targetID;
    }
    return GetTargetNeuron()->GetId();
}
void Synapse::ExpireSignal()
{
    if (m_expireSignal)
    {
        StandardActivate(m_expireSignal);
    }
    else
    {
        if (IsAdded()) 
        {
            GetTargetNeuron()->RemoveSynapse(this);
        }
        NoLongerReady(); //Must be after *this is removed.
        MakeZero();
    }
}

void Synapse::Callback(void* arg)
{
    StandardActivate(m_callback, arg);
}
bool Synapse::IsReady() const
{
    return m_isReady;
}
bool Synapse::IsAdded() const
{
    return m_isAdded;
}
void Synapse::Added()
{
    if (IsAdded() && !AreMultipleAdditionsAllowed())
    {
        Warn("%s readded", Identity().c_str());
    }
    else
    {
        Debug("%s added", Identity().c_str());
        PrepareForRemoval();
    }
    m_isAdded = true;
    m_addedAt = time::GetCurrentTimestamp();
    NoLongerReady();
}
void Synapse::Removed()
{
    if (!IsAdded() && !AreMultipleRemovalsAllowed())
    {
        Warn("%s removed while not added", Identity().c_str());
    }
    else
    {
        Debug("%s removed", Identity().c_str());
    }
    m_isAdded = false;
}

Synapse* Synapse::Clone() const
{
    return new Synapse(*this);
}

Synapse* Synapse::ConfigureFor(const Neuron* source, Neuron* target) const
{
    Synapse* ret = Clone();
    ret->m_source = source;
    if (m_target)
    {
        ret->m_target = target;
        ret->m_targetID = m_target->GetId();
    }
    ret->AddtionalConfiguration(source, target);
    if (target)
    {
        target->AddIncomingSynapse(ret);
    }
    return ret;
}

ReturnCode Synapse::ProcessAddition()
{
    return StandardActivate(m_processAddition);
}
ReturnCode Synapse::ProcessRemoval()
{
    return StandardActivate(m_processRemoval);
}

ReturnCode Synapse::PostAddition()
{
    return StandardActivate(m_postAddition);
}
ReturnCode Synapse::PostRemoval()
{
    return StandardActivate(m_postRemoval);
}
void Synapse::NoLongerReady()
{
    m_isReady = false;
    m_addAt = 0; //FIXME: this could be incrementing the timestamp.
}
void Synapse::Update(Timestamp addDatumAt)
{
    m_isReady = true;
    m_addAt = addDatumAt;
    m_lastUpdated = time::GetCurrentTimestamp();

    //NOTE: Expiration time may be extended later or other data edits may occur
    //This is better than GetTimeToAdd == GetCurrentTimestamp.
    //FIXME: bug when GetCurrentTimestamp = UINT_MAX
    if (GetTimeToAdd() <= GetTimeLastUpdated())
    {
        GetTargetNeuron()->RequestProcessingOf(this);
    }
}
Timestamp Synapse::GetTimeLastUpdated() const
{
    return m_lastUpdated;
}
bool Synapse::ShouldBeAdded() const
{
    // Debug("%s...", Identity().c_str());
    if (!m_isReady)
    {
        // Debug(" not added as it is not ready");
        return false;
    }

    if (m_isAdded)
    {
        // Debug(" not added as it is already added");
        return false;
    }

    if (IsZero() && !AreZerosAllowed())
    {
        // Debug(" not added as its value is zero");
        return false;
    }

    Timestamp now = time::GetCurrentTimestamp();

    //FIXME: bug when GetCurrentTimestamp = UINT_MAX
    if (now < m_addAt)
    {
        // Debug(" will not be added until %lums from now", (m_addAt - now));
        return false;
    }

    if (m_doesExpire && now >= m_addAt + m_expiration)
    {
        // Debug(" not added as it expired %lums ago", (now - (m_lastUpdated + m_expiration)));
        return false;
    }
    Debug("    will be added!");
    return true;
}
bool Synapse::ShouldBeRemoved() const
{
    if (!m_isAdded)
        return false;

    //New data are available.
    if (m_isReady)
        return true;

    if (m_doesExpire)
    {
        Timestamp now = time::GetCurrentTimestamp();
        return now >= m_addedAt + m_expiration;
    }
    else
    {
        return false;
    }
}
Timestamp Synapse::GetTimeToAdd() const
{
    return m_addAt;
}

TimeMS Synapse::GetExpiration() const
{
    return m_expiration;
}
void Synapse::SetExpiration(TimeMS lastFor)
{
    m_expiration = lastFor;
}
void Synapse::ExtendExpirationUntil(Timestamp timeToExpire)
{
    SetExpiration(timeToExpire - m_addAt);
}
void Synapse::AllowMultipleAdditions(bool allow)
{
    m_allowMultipleAdditions = allow;
}
bool Synapse::AreMultipleAdditionsAllowed() const
{
    return m_allowMultipleAdditions;
}
void Synapse::AllowMultipleRemovals(bool allow)
{
    m_allowMultipleRemovals = allow;
}
bool Synapse::AreMultipleRemovalsAllowed() const
{
    return m_allowMultipleRemovals;
}
bool Synapse::AreZerosAllowed() const
{
    return m_areZerosAllowed;
}
void Synapse::SetZerosAllowed(bool allowed)
{
    m_areZerosAllowed = allowed;
}

bool Synapse::DoesExpire() const
{
    return m_doesExpire;
}
void Synapse::SetDoesExpire(const bool does)
{
    m_doesExpire = does;
}

void Synapse::AddEffect(const AspectId effect)
{
    if (!DoesAffect(effect))
    {
        m_effects.push_back(effect);
    }
}
bool Synapse::DoesAffect(const AspectId aspect)
{
    return std::find(m_effects.begin(), m_effects.end(), aspect) != m_effects.end();
}
AspectIds Synapse::GetEffects() const
{
    return m_effects;
}
