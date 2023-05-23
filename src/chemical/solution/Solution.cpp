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

#include "bio/chemical/solution/Solution.h"
#include "bio/chemical/solution/Solute.h"

namespace bio {
namespace chemical {

Id Solution::Dissolve(Substance* toDissolve)
{

}

Substance* Solution::Separate(const Id& id)
{

}

Id Solution::Influx(const Solute& toInflux)
{

}

void Solution::DecrementConcentration(const Index soluteIndex)
{
	BIO_SANITIZE(this->IsAllocated(soluteIndex), , return)

	//We need to access the Solute we want to change directly, rather than make a copy as OptimizedAccess would do.
	Solute* solute = ForceCast< Solute* >(&this->mStore[soluteIndex * sizeof(Solute)]);
	BIO_SANITIZE(solute, , return)
	solute->DecrementConcentration();
}

Solute& Solution::Efflux(const Id& soluteId)
{

}

const Solute& Solution::Efflux(const Id& soluteId) const
{
	return const_cast< Solution* >(this)->Efflux(soluteId);
}

Solute& Solution::Efflux(const Name& substanceName)
{
	
}

const Solute& Solution::Efflux(const Name& substanceName) const
{
	return const_cast< Solution* >(this)->Efflux(substanceName);
}

Solute& Solution::operator[](const Id& soluteId)
{
	return Efflux(soluteId);
}

const Solute& Solution::operator[](const Id& soluteId) const
{
	return Efflux(soluteId);
}

Solute& Solution::operator[](const Name& substanceName)
{
	return Efflux(substanceName);
}

const Solute& Solution::operator[](const Name& substanceName) const
{
	return Efflux(substanceName);
}

} //chemical namespace
} //bio namespace
