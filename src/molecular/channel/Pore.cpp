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

#include "bio/molecular/channel/Pore.h"
#include "bio/molecular/common/SymmetryTypes.h"

namespace bio {
namespace molecular {

Pore::Pore() :
	molecular::Class< Pore >(
	this,
	NULL,
	filter::Molecular(),
	symmetry_type::Variable())
{

}

Pore::Pore(
	const Name& name,
	Vesicle* environment
) :
	Surface(name, environment),
	molecular::Class< Pore >(
		this,
		name,
		environment,
		filter::Molecular(),
		symmetry_type::Variable())
{

}

Pore::~Pore()
{

}

void Pore::SetEnvironment(Vesicle* environment)
{
	mTransMembraneDomain.SetVesicle(environment);
	Surface::SetEnvironment(environment);
}


} //molecular namespace
} //bio namespace
