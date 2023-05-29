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

#include "bio/physical/affinity/Affinity.h"
#include "bio/chemical/Substance.h"

namespace bio {
namespace chemical {

class Substance;

/**
 * Affinities are stored sets of Filters, Properties, States. <br />
 * They allow you to build a selector that can be compared against Substances later. <br />
 */
class Affinity :
	public chemical::Class< Affinity >,
	public ::bio::Affinity,
	public Covalent< UnorderedMotif< physical::Attraction< Filter > > >,
	public Covalent< UnorderedMotif< physical::Attraction< Property > > >,
	public Covalent< UnorderedMotif< physical::Attraction< State > > >,
	virtual public Structure
{
public:

	BIO_DEFAULT_IDENTIFIABLE_CONSTRUCTORS(
		chemical,
		Affinity,
		filter::Chemical()
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
	 * @param waves
	 * @return the sum of all Attractions between *this and the substance.
	 */
	virtual Strength GetStrengthOfAttractionTo(const physical::Wave* wave) const;

	/**
	 * Because UnorderedMotifs do not provide a Get method, we have to do some hacks to get the Attraction::Force stored in *this. <br />
	 * @tparam T the DIMENSION for which to measure the physical::Attraction in *this.
	 * @param t a point in the T DIMENSION that we will measure.
	 * @return the Attraction::Force *this has for the given t.
	 */
	template < typename T >
	physical::Attraction< T >::Force GetAttractionFor(T t) const
	{
		const Container* container = GetAll< physical::Attraction< T > >();
		BIO_SANITIZE(container,,return 0.0f;)
		Index index = container->SeekTo(physical::Attraction< T >(t));
		if (container->IsAllocated(index)) {
			return 0.0f;
		}
		return container->Access(index).template As< physical::Attraction< T > >().GetForce();
	}
	
	template < typename T >
	Strength MeasureAttractionAlong(const Substance* substance) const
	{
		Strength ret = 0.0f;
		for (
			SmartIterator itt = substance->template GetAll< T >()->End();
			!itt.IsBeforeBeginning();
			--itt
		) {
			ret += GetAttractionFor< T >(itt.template As< T >());
		}
		return ret;
	}

};

} //chemical namespace
} //bio namespace
