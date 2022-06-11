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
Personality: Holds all the circulatory variables
API Personality variables are created by the the circulatory file. See documentation below in the ctor. GetValue(string) Returns current value of circulatory variable "string". SetValue(string) Sets current value of circulatory variable "string". ChangeViaStimulus() Call to increment circulatory variable by a stimulus defined in the circulatory JSON file. StartChangeViaTimeRate() Call to start changing circulatory variable by   time rate defined in the circulatory JSON file.  StopChangeViaTimeRate()     Call to stop changing circulatory variable by the     time rate defined in StartChangeViaTimeRate(). Poll() is to be called by Brain or other fast clock
*/
#include <bio/common/PollObject.h>
#include <bio/physical/Identified.h>
#include <bio/log/LoggerObject.h>
#include "Types.h"

namespace bio{

class PersonalityVar;

class Personality : public PollObject, public Identified<DefaultRealm>, public log::LoggerObject {
public:

    BIO_DEFINE_MAP(PVMap, PersonalityVarId, PersonalityVar *)

/**
 poses. However, the default ID for a Personality is not used Additionally, the NameTracker is left NULL. If you would like to manage more than one Brain, feel free to:
*/
    /** 1. BIO_NAME_TRACKER_SINGLETON(PersonalityTracker, PersonalityId)
    // 2. call SetTracker(PersonalityTracker) NOTE: THIS CAN ONLY BE DONE ONCE

    /**
    The name of the circulatory can be
    */
    Personality(Name name = "plain");
    virtual ~Personality();

    BIO_NAME_TRACKER_SINGLETON(PVTracker, PersonalityVarId)
    BIO_NAME_TRACKER_SINGLETON(ChangeTracker, PersonalityChangeId)

    /**
    Creates a circulatory variable and adds it to *this.
    RETURNS: new PersonalityVar on success ULL otherwise
    New circulatory variable's attributes are all set to zero.
    */
    PersonalityVar* CreatePV(Name name);

    /**
    Stores a PersonalityVar in *this
    */
    bool AddPV(PersonalityVar* pv);

    /**
    RETURNS: a pointer to the indicated PVar; NULL if not found.
    */
    PersonalityVar* GetPV(PersonalityVarId id);
    const PersonalityVar* GetPV(PersonalityVarId id) const;
    PersonalityVar* GetPV(Name name) 
    {
        return GetPV(ChangeTracker::Instance().IdFromName(name));
    }
    const PersonalityVar* GetPV(Name name) const 
    {
        return GetPV(ChangeTracker::Instance().IdFromName(name));
    }

    /**
    RETURNS: the PVar with the highest value; NULL if all PVars are 0, and the first eligible if 2 PVars are tied.
    */
    const PersonalityVar* GetHighestPV() const;

    PersonalityValue GetValue(PersonalityVarId id) const;

    bool SetValue(PersonalityVarId id, PersonalityValue newValue);

    /**
    Create a change that can be used later.
    Changes are used to increment or decrement PersonalityVars.
    They can be applied in bursts or over time.
    For more info on how to use changes, see below and PersonalityVar.h
    */
    PersonalityChangeId CreateChange(Name name, PersonalityValue value);

    /**
    TODO: there is no way to change or delete Changes; this is mainly due to changes being copied to PVars.

    /**
    RETURNS: the value stored with CreateChange, 0 if changeId isn't found in *this.
    */
    PersonalityValue GetValueOfChange(PersonalityChangeId changeId) const;

    /**
    To change a circulatory variable once and not over time, use these methods. They will call PersonalityVar::ApplyChange
    */
    void ChangeBurst(PersonalityVar* pVar, PersonalityChangeId changeId, unsigned int multiples);

    /**
    Like the above, but uses all PersonalityVars.
    */
    void ChangeBurst(PersonalityChangeId changeId, unsigned int multiples);

    /**
    To change circulatory variables on the clock, use these methods.
    See PersonalityVar.h for more information on Enabling and Disabling
    */
    void EnableChangeOverTime(PersonalityVar* pVar, PersonalityChangeId changeId);
    void DisableChangeOverTime(PersonalityVar* pVar, PersonalityChangeId changeId);

//START: PollObject required methods. See that class for documentation

    /**
    Poll the circulatory engine. This MUST be called on a schedule so
    that this engine updates its state.
    DO NOT ADD PVARS TO A CL0CK! pVars should only be polled by *this.
    */
    void Poll();

//END: PollObject required methods.

//START: LoggerObject overrides

    virtual void SetLogEngine(log::Engine* logEngine);

//END: LoggerObject overrides

protected:
    PVMap m_varMap;

    PersonalityChangeMap m_changeMap;

    /**
    Calls ApplyAnyActiveChanges for all PVars.
    RETURNS: true on success, false otherwise
    */
    void ChangeAllVarsViaTime();

};
} //bio namespace
