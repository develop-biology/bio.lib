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

#include "bio/chemical/common/Class.h"
#include "bio/chemical/Substance.h"

namespace bio {
namespace chemical {

/**
 * A Reactant is a Substance that has a type. <br />
 * Reactants are intended to be used in Reactions. <br />
 * In order to ensure the Substances provided to a Reaction are the proper types, we override the equality operator. This ensures that the compared value can be cast as the type recorded in *this. <br />
 */
class Reactant :
	public chemical::Class< Reactant >,
	public Substance
{
public:

	/**
	 * Ensure virtual methods point to Class implementations. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(chemical, Reactant)

	/**
	 * Don't use this
	 */
	Reactant();

	/**
	 * @param typeName
	 */
	Reactant(const Name& typeName);

	/**
	 * @param typeName
	 * @param substance
	 */
	Reactant(
		const Name& typeName,
		const Substance* substance
	);

	/**
	 * @param typeName
	 * @param properties
	 * @param states
	 */
	Reactant(
		const Name& typeName,
		const UnorderedMotif< Property >& properties,
		const UnorderedMotif< State >& states
	);

	/**
	 *
	 */
	virtual ~Reactant();

	/**
	 * @param other
	 * @return whether *this is the same as other and other can be cast to the appropriate type.
	 */
	virtual bool operator==(const Substance& other) const;

protected:
	Name mTypeName;

};

} //chemical namespace
} //bio namespace

