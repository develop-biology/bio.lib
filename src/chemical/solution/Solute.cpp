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

#include "bio/circulatory/Solute.h"
#include "bio/personality/Personality.h"
#include "bio/common/Time.h"
// #include <cstdio>

using namespace bio;

void CommonConstructor()
{
	mConcentration = 0.0f;
	mMin = 0.0f;
	mMax = 1.0f;
}

virtual Concentration GetConcentration() const {return mConcentration;}

/**
 * Rather than activating a Fluctuation, you can just set the mConcentration of *this directly.
 * @param newConcentration
 */
virtual void SetConcentration(Concentration newConcentration);

/**
 * Get the mMin of *this
 * @return the mMin of *this.
 */
virtual Concentration GetMin() const {return mMin;}

/**
 * Get the mMax of *this.
 * @return the mMax of *this.
 */
virtual Concentration GetMax() const {return mMax;}

/**
 * Set the mMin of *this.
 * @param newMin
 */
virtual void SetMin(Concentration newMin);

/**
 * Set the mMax of *this.
 * @param newMax
 */
virtual void SetMax(Concentration newMax);

/**
 * @return the Concentration of *this last time *this Peak()ed.
 */
virtual Concentration GetConcentrationAtLastPeak() const {return mConcentrationAtLastPeak;}

/**
 * Mark the given Fluctuation as active.
 * Active Fluctuations will be applied on a per second basis; meaning that a change of +1 will be interpreted as +1 / s. If ApplyAnyActiveChanges is called every 200ms it will add 0.2 between clock ticks.
 * @param fluctuation
 * @param concentration
 * @return true on success, false otherwise
 */
virtual bool EnableFluctuation(const Id fluctuation, const Concentration concentration);

/**
 * Marks the given Fluctuation as inactive.
 * @param fluctuation
 * @return true on success, false otherwise
 */
virtual bool DisableFluctuation(const Id fluctuation);

/**
 * Applies active changes. See EnableFluctuation, above.
 * For more info see physical::Periodic.
 */
virtual Code Peak();

protected:
Concentration mConcentration;
Concentration mConcentrationAtLastPeak;
Concentration mMin, mMax;
UnchangableChangeMap mActiveChanges;

/**
 * Make sure the Concentration of *this does not exceed mMin ~ mMax.
 */
virtual void Limit()
{
	if (mConcentration > mMax) mConcentration = mMax;
	else if (mConcentration < mMin) mConcentration = mMin;
}

};





Hormone::PersonalityVar(Name name, NameTracker<PersonalityVarId>* tracker, PersonalityValue init)
    : PollObject(), Named(name, tracker), LoggerObject(NULL, log::Filt::PERSONALITY), m_value(init), m_valueAtLastPoll(0), m_min(0.0f), m_max(1.0f)
{

}
PersonalityVar::~PersonalityVar()
{
}

void PersonalityVar::SetValue(PersonalityValue newValue)
{
    m_value = newValue;
    Limit();
    Debug("%s = %f. Value is now %f", GetName(), newValue, m_value);
}
void PersonalityVar::SetMin(PersonalityValue newMin)
{
    m_min = newMin;
    Limit();
}
void PersonalityVar::SetMax(PersonalityValue newMax)
{
    m_max = newMax;
    Limit();
}

bool PersonalityVar::EnableChangeOverTime(const PersonalityChangeId change, const PersonalityValue value)
{
    if (m_activeChanges.find(change) != m_activeChanges.end())
        return false;
    m_activeChanges.insert(UnchangablePersonalityChangeMapPair(change, value));
    return true;
}

bool PersonalityVar::DisableChangeOverTime(PersonalityChangeId change)
{
    UnchangablePersonalityChangeMap::iterator atv = 
        m_activeChanges.find(change);
    if (atv == m_activeChanges.end())
        return false;
    m_activeChanges.erase(atv);
    return true;
}

void PersonalityVar::Poll()
{

}
void PersonalityVar::Display()
{
    printf("  %s: %f (min: %.2g max: %.2g) Active Changes:\n", GetName(), m_value, m_min, m_max);

    Name changeName; //buffer
    for (UnchangablePersonalityChangeMap::const_iterator cng = 
        m_activeChanges.begin(); cng != m_activeChanges.end(); ++cng)
    {
        changeName = 
            Personality::ChangeTracker::Instance().NameFromId(cng->first);
        printf("    %s rate: %f", changeName, cng->second);
    }
}
