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
#include "bio/common/String.h"

namespace bio {
namespace chemical {

void Reactant::CtorCommon(Name typeName)
{
	string::CloneInto(
		typeName,
		m_typeName
	);
}

Reactant::Reactant(Name typeName)
	:
	Class< Reactant >(this),
	Substance()
{
	CtorCommon(typeName);
}

Reactant::Reactant(
	Name typeName,
	const typename UnorderedMotif< Property >::Contents* properties, <br />
	const typename UnorderedMotif< State >::Contents* states <br />
)
	:
	Class< Reactant >(this),
	Substance(
		properties,
		states
	)
{
	CtorCommon(typeName);
}

Reactant::Reactant(
	Name typeName,
	const Substance* substance <br />
)
	:
	Class< Reactant >(this),
	Substance(*substance)
{
	CtorCommon(typeName);
}


Reactant::~Reactant()
{
	delete[] m_typeName;
}

bool Reactant::operator==(const Substance& other) const
{
	return Substance::operator==(other) && other.GetBondPosition(m_typeName) != 0;
}

} //chemical namespace
} //bio namespace
