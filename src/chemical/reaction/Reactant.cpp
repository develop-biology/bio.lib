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

#include "bio/chemical/reaction/Reactant.h"
#include "bio/common/string/String.h"

namespace bio {
namespace chemical {

Reactant::Reactant() :
	chemical::Class< Reactant >(this),
	Substance(),
	mTypeName(NULL)
{

}

Reactant::Reactant(const Name& typeName)
	:
	chemical::Class< Reactant >(this),
	Substance(),
	mTypeName(typeName)
{

}

Reactant::Reactant(
	const Name& typeName,
	const UnorderedMotif< Property >& properties,
	const UnorderedMotif< State >& states
)
	:
	chemical::Class< Reactant >(this),
	Substance(
		properties,
		states
	),
	mTypeName(typeName)
{

}

Reactant::Reactant(
	const Name& typeName,
	const Substance* substance
)
	:
	chemical::Class< Reactant >(this),
	Substance(*substance),
	mTypeName(typeName)
{

}


Reactant::~Reactant()
{

}

bool Reactant::operator==(const Substance& other) const
{
	return Substance::operator==(other) && other.GetBondPosition(mTypeName) != 0;
}

} //chemical namespace
} //bio namespace
