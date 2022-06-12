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

#include "bio/chemical/solution/Solute.h"
#include "bio/chemical/solution/Fluctuation.h"

namespace bio {
namespace chemical {

void Solute::CommonConstructor()
{
	mConcentration = 0.0f;
	mMin = 0.0f;
	mMax = 1.0f;
}

Concentration Solute::GetConcentration() const
{
	return mConcentration;
}

void Solute::SetConcentration(Concentration newConcentration)
{
	mConcentration = newConcentration;
	Limit();
}

void Solute::Increment(Concentration add)
{
	mConcentration += add;
	Limit();
}

void Solute::Decrement(Concentration subtract)
{
	mConcentration -= subtract;
	Limit();
}

Concentration Solute::GetMin() const
{
	return mMin;
}

Concentration Solute::GetMax() const
{
	return mMax;
}

void Solute::SetMin(Concentration newMin)
{
	mMin = newMin;
	Limit();
}

void Solute::SetMax(Concentration newMax)
{
	mMax = newMax;
	Limit();
}

Concentration Solute::GetConcentrationAtLastPeak() const
{
	return mConcentrationAtLastPeak;
}

void Solute::RecordPeakConcentration()
{
	mConcentrationAtLastPeak = mConcentration;
}

Code Solute::Peak()
{
	Fluctuation* fluctuation;
	for (
		SmartIterator flx = GetAll< Fluctuation* >()->Begin();
		!flx.IsAfterEnd();
		++flx
	)
	{
		fluctuation = flx;
		fluctuation->Affect(this);
		//We don't care about Affect's return value right now.
	}
	return code::Success();
}

void Solute::Limit()
{
	if (mConcentration > mMax)
	{
		mConcentration = mMax;
	}
	else if (mConcentration < mMin)
	{
		mConcentration = mMin;
	}
}

} //chemical namespace
} //bio namespace
