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

#include "bio/chemical/solution/fluctuation/ChangeOverTime.h"
#include "bio/chemical/solution/Solute.h"

namespace bio {
namespace chemical {

void ChangeOverTime::CommonConstructor()
{
	mRate = 0.0f;
}

Concentration ChangeOverTime::GetRate() const
{
	return mRate;
}

void ChangeOverTime::SetRate(Concentration rate)
{
	mRate = rate;
}

Code ChangeOverTime::Affect(Solute* solute)
{
	BIO_SANITIZE(solute,,return code::BadArgument1())

	Timestamp now = time::GetCurrentTimestamp();
	Timestamp msSinceLast = now - solute->GetTimeLastPolled();
	float deltaSec = msSinceLast / 1000.0f;
	float add = 0.0f;
	for (UnchangablePersonalityChangeMap::const_iterator cng =
		m_activeChanges.begin(); cng != m_activeChanges.end(); ++cng)
	{
		add = cng->second * deltaSec;
		m_value += add;
		// Debug("%s += %f (dT = %f) via %s. Value is now %f", m_id.c_str(), add, deltaSec, cng->first.c_str(), m_value);
	}
	Limit();
	m_lastPollTimestamp = now;
	m_valueAtLastPoll = m_value;
}

} //chemical namespace
} //bio namespace
