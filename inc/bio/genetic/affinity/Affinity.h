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

#pragma once

#include "bio/chemical/affinity/Affinity.h"
#include "bio/genetic/common/Filters.h"
#include "bio/genetic/common/Types.h"

namespace bio {
namespace genetic {

/**
 * genetic::Affinities are stored sets of TranscriptionFactors, Filters, Properties, and States. <br />
 * They allow you to build a selector that can be compared against Expressors later. <br />
 */
class Affinity :
	public chemical::Class< Affinity >,
	public chemical::Affinity,
	public Covalent< chemical::UnorderedMotif< physical::Attraction< TranscriptionFactor > > >
{
public:

	BIO_DEFAULT_IDENTIFIABLE_CONSTRUCTORS(
		chemical,
		Affinity,
		filter::Genetic()
	)

	/**
	 * Ensure virtual methods point to Class implementations. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(chemical, Affinity)

	/**
	 *
	 */
	virtual ~Affinity();

	/**
	 * To get the Affinity::Strength of an interaction, we add all Attraction::Forces together. <br />
	 * Here, negative Attractions (i.e. repulsions) will cause the Affinity to be weaker (i.e. smaller) or negative. <br />
	 * @param wave
	 * @return the sum of all Attractions between *this and the substance.
	 */
	virtual Strength GetStrengthOfAttractionTo(const physical::Wave* wave) const;

};

} //genetic namespace
} //bio namespace
