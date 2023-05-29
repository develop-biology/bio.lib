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

#include "bio/physical/common/Class.h"
#include "bio/physical/common/Filters.h"
#include "bio/physical/macro/Macros.h"
#include "bio/physical/relativity/Identifiable.h"
#include "bio/chemical/Substance.h"

namespace bio {
namespace chemical {

/**
 * Miscibilities map the Properties of a Substance to an Interference pattern that can be used to Superpose one Substance on another. <br />
 * This is accomplished by the Id of a Miscibility mapping directly to a Property. To get the Miscibility for a given Property, you can call MiscibilityPerspective::Instance()->GetTypeFromIdAs< Miscibility* >(Cast< Protperty::Type >(PROPERTY));<br />
 * The Superposing of Substances is handled by the Mix() Reaction. <br />
 */
class Miscibility :
	public physical::Class< Miscibility >,
	public physical::Identifiable< Property >
{
public:

	/**
	 * Ensure virtual methods point to Class implementations. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(physical, Miscibility)

	/**
	 * Id should be set later. <br />
	 */
	Miscibility();

	/**
	 * @param property what *this applies to.
	 */
	Miscibility(const Property& property);

	/**
	 * @param name the name of the Property that *this applies to.
	 */
	Miscibility(const Name& name);

	/**
	 * Will delete mInterference if it is not NULL. <br />
	 */
	virtual ~Miscibility();

	/**
	 * Get the right sub-object (or bonded object) of the given Substance for use in Wave::Superpose. <br />
	 * The Wave returned should be able to be ForceCasted to the expected object per the given Interference. <br />
	 * For example, if the
	 * @param substance
	 * @return
	 */
	const Wave* GetDisplacement(const Substance* substance) const;

	/**
	 * Get the Interference pattern that can be used to Superpose one Substance on another. <br />
	 * @return a Clone() of mInterference
	 */
	physical::Interference* GetInterference() const;

	/**
	 * Set the Interference pattern that can be used to Superpose one Substance on another. <br />
	 * @param interference
	 */
	void SetInterference(physical::Interference* interference);

protected:
	physical::Interference* mInterference;
};

} //chemical namespace
} //bio namespace
