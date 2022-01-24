/*
 * This file is a part of the Biology project by eons LLC.
 * Biology (aka Develop Biology) is a framework for approaching software
 * development from a natural sciences perspective.
 *
 * Copyright (C) 2021 Séon O'Shannon & eons LLC
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

#include "Substance.h"

namespace bio {
namespace chemical {

/**
 * A Reactant is a Substance that has a type.
 * Reactants are intended to be used in Reactions.
 * In order to ensure the Substances provided to a Reaction are the proper types needed by the Reaction, the equality operator ensures that the compared value can be cast as the type recorded in *this.
 */
class Reactant : public Substance
{
public:
	/**
	 * @param typeName
	 */
	Reactant(Name typeName);

	/**
	 * @param typeName
	 * @param substance
	 */
	Reactant(Name typeName, const Substance& substance);

	/**
	 * @param typeName
	 * @param properties
	 * @param states
	 */
	Reactant(
		Name typeName,
		typename const StructuralComponent<Property>::Contents& properties,
		typename const StructuralComponent<State>::Contents& states);

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
	Name m_typeName;
};

} //chemical namespace
} //bio namespace

