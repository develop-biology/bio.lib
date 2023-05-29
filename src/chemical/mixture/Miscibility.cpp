/*
 * This file is a part of the Biology project by eons LLC.
 * Biology (aka Develop Biology) is a framework for approaching software
 * development from a natural sciences perspective.
 *
 * Copyright (C) 2023 Séon O'Shannon & eons LLC
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

#include "bio/chemical/mixture/Miscibility.h"
#include "bio/physical/wave/Interference.h"

namespace bio {
namespace chemical {

Miscibility::Miscibility() :
	physical::Class< Miscibility >(this, NULL)
{

}

Miscibility::Miscibility(const Property& property):
	physical::Class< Miscibility >(this, NULL)
{

}

Miscibility::Miscibility(const Name& name):
	physical::Class< Miscibility >(this, NULL)
{

}

Miscibility::~Miscibility()
{
	if (mInterference)
	{
		delete mInterference;
		mInterference = NULL;
	}
}

const physical::Wave* Miscibility::GetDisplacement(const Substance* substance) const
{
	return NULL;
}

physical::Interference* Miscibility::GetInterference() const
{
	return ForceCast< physical::Class< physical::Interference >* >(mInterference->Clone()->AsWave())->GetWaveObject();
}

void Miscibility::SetInterference(physical::Interference* interference)
{

}

} //chemical namespace
} //bio namespace
