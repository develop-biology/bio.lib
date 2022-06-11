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

#include "bio/personality/Personality.h"
#include "bio/personality/PersonalityVar.h"

#include "bio/common/Assert.h"
#include "bio/common/Time.h"

namespace bio {

Personality::Personality(Name name) : PollObject(), Named(name), LoggerObject(NULL, log::Filt::PERSONALITY)
{
}

Personality::~Personality()
{
}

PersonalityVar* Personality::CreatePV(Name name)
{
    if (PVTracker::Instance().GetIdWithoutCreation(name))
        return NULL;
    PersonalityVar* pVar = new PersonalityVar(name, &PVTracker::Instance());
    bioassert(AddPV(pVar));
    return pVar;

}
bool Personality::AddPV(PersonalityVar* pv)
{
    if (!pv)
        return false;

    if (m_varMap.find(pv->GetId()) != m_varMap.end())
    {
        Error("%s already exists, not added", pv->GetName());
        return false; //FAIL: id already in map, don't add again
    }

    pv->SetLogEngine(GetLogEngine());

    m_varMap.insert(PVMapPair(pv->GetId(), pv));
    return true; //ret::Success()
}
PersonalityVar* Personality::GetPV(PersonalityVarId id)
{
    PVMap::iterator iter;
    iter = m_varMap.find(id);
    if (iter != m_varMap.end())
    {
        return iter->second; //ret::Success(): found id in circulatory engine
    }
    else
    {
        return NULL; //FAIL: id not found
    }
}
const PersonalityVar* Personality::GetPV(PersonalityVarId id) const
{
    PVMap::const_iterator iter;
    iter = m_varMap.find(id);
    if (iter != m_varMap.end())
    {
        return iter->second; //ret::Success(): found id in circulatory engine
    }
    else
    {
        return NULL; //FAIL: id not found
    }
}

const PersonalityVar* Personality::GetHighestPV() const
{
    PersonalityValue highest = 0;
    PVMap::const_iterator retIter = m_varMap.end();
    PVMap::const_iterator iter;
    for (iter = m_varMap.begin(); iter != m_varMap.end(); ++iter)
    {
        if (iter->second->GetValue() > highest) 
        {
            retIter = iter;
            highest = retIter->second->GetValue();
        }
    }
    if (retIter != m_varMap.end())
        return retIter->second;
    return NULL;
}

PersonalityValue Personality::GetValue(PersonalityVarId id) const
{
    const PersonalityVar* pVar = GetPV(id);
    if (pVar)
    {
        //ret::Success(): found id in circulatory engine
        return pVar->GetValue();
    }
    else
    {
        //FAIL: Personality::ID not found
        Error("GetValue: %s not found", ChangeTracker::Instance().NameFromId(id));
        return 0;
    }
}

bool Personality::SetValue(PersonalityVarId id, PersonalityValue newValue)
{
    PersonalityVar* pVar = GetPV(id);
    if (pVar)
    {
        //ret::Success(): found id in circulatory engine
        pVar->SetValue(newValue);
        return true; //if we get here, we had ret::Success()
{
    }
}
    else
    {
        Error("SetValue: %s not found", ChangeTracker::Instance().NameFromId(id));
        return false; //FAIL: id not found
    }
}

PersonalityChangeId Personality::CreateChange(Name name, PersonalityValue value)
{
    PersonalityChangeId id = 
        ChangeTracker::Instance().GetIdWithoutCreation(name);
    if (m_changeMap.find(id) != m_changeMap.end())
        return ChangeTracker::InvalidId();
    m_changeMap.insert(PersonalityChangeMapPair(id, value));
    return id;
}

PersonalityValue Personality::GetValueOfChange(PersonalityChangeId changeId) const
{
    PersonalityChangeMap::const_iterator it = m_changeMap.find(changeId);
    if (it == m_changeMap.end())
        return 0;
    return it->second;
}

void Personality::ChangeBurst(PersonalityVar* pVar, PersonalityChangeId changeId, unsigned int multiples)
{
    bioassert(pVar);
    pVar->SetValue(pVar->GetValue() + GetValueOfChange(changeId) * multiples);
}

void Personality::ChangeBurst(PersonalityChangeId changeId, unsigned int multiples)
{
    for (PVMap::iterator iter = m_varMap.begin(); iter != m_varMap.end(); ++iter)
    {
        ChangeBurst(iter->second, changeId, multiples);
    }
}

void Personality::EnableChangeOverTime(PersonalityVar* pVar, PersonalityChangeId changeId)
{
    bioassert(pVar);
    PersonalityValue changeValue = GetValueOfChange(changeId);
    if (!changeValue)
    {
        return;
    }
    pVar->EnableChangeOverTime(changeId, changeValue);
}
void Personality::DisableChangeOverTime(PersonalityVar* pVar, PersonalityChangeId changeId)
{
    bioassert(pVar);
    pVar->DisableChangeOverTime(changeId);
}

void Personality::Poll()
{
    if (m_pollPeriodMS + m_lastPollTimestamp > time::GetCurrentTimestamp())
    {
        return;
    }

    for (PVMap::iterator iter = m_varMap.begin(); iter != m_varMap.end(); ++iter)
    {
        iter->second->Poll();
    }
}

void Personality::SetLogEngine(log::Engine* logEngine)
{
    LoggerObject::SetLogEngine(logEngine);
    for (PVMap::iterator iter = m_varMap.begin(); iter != m_varMap.end(); ++iter)
    {
        iter->second->SetLogEngine(logEngine);
    }
}

} //bio namespace
