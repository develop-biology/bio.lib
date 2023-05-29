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

#include "bio/genetic/affinity/Affinity.h"
#include "bio/genetic/Expressor.h"

namespace bio {
namespace genetic {

Affinity::~Affinity()
{

}

Affinity::Strength Affinity::GetStrengthOfAttractionTo(const physical::Wave* wave) const
{
	Strength ret = chemical::Affinity::GetStrengthOfAttractionTo(wave);
	const Expressor* expressor = ChemicalCast< const Expressor* >(wave);
	BIO_SANITIZE(expressor,,return ret)
	ret += MeasureAttractionAlong< TranscriptionFactor >(expressor);
	return ret;
}

} //genetic namespace
} //bio namespace
