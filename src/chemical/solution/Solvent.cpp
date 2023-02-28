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

#include "bio/chemical/solution/Solvent.h"
#include "bio/chemical/solution/Solute.h"

namespace bio {
namespace chemical {

Id Solvent::Dissolve(Substance* toDissolve)
{

}

Substance* Solvent::Separate(const Id& id)
{

}

Id Solvent::Ingress(const Solute& toIngress)
{

}

Solute& Solvent::Egress(const Id& soluteId)
{

}

const Solute& Solvent::Egress(const Id& soluteId) const
{
	return const_cast< Solvent* >(this)->Egress(soluteId);
}

Solute& Solvent::Egress(const Name& substanceName)
{
	
}

const Solute& Solvent::Egress(const Name& substanceName) const
{
	return const_cast< Solvent* >(this)->Egress(substanceName);
}

Solute& Solvent::operator[](const Id& soluteId)
{
	return Egress(soluteId);
}

const Solute& Solvent::operator[](const Id& soluteId) const
{
	return Egress(soluteId);
}

Solute& Solvent::operator[](const Name& substanceName)
{
	return Egress(substanceName);
}

const Solute& Solvent::operator[](const Name& substanceName) const
{
	return Egress(substanceName);
}

} //chemical namespace
} //bio namespace
