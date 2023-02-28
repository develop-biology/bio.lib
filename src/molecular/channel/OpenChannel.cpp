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

#include "bio/molecular/channel/OpenChannel.h"
#include "bio/molecular/common/SymmetryTypes.h"

namespace bio {
namespace molecular {

OpenChannel::OpenChannel() :
	molecular::Class< OpenChannel >(
		this,
		NULL,
		filter::Molecular(),
		symmetry_type::Variable())
{

}


OpenChannel::OpenChannel(
	const Name& name,
	Vesicle* environment
) :
	Pore(name, environment),
	molecular::Class< OpenChannel >(
		this,
		name,
		environment,
		filter::Molecular(),
		symmetry_type::Variable())
{

}

OpenChannel::~OpenChannel()
{

}

void OpenChannel::Influx(Molecule* outer)
{
	mTransMembraneDomain.Influx(outer);
}


void OpenChannel::Influx(Vesicle* outer)
{
	mTransMembraneDomain.Influx(outer);
}


Molecule* OpenChannel::Efflux(const Name& moleculeName)
{
	return mTransMembraneDomain.Efflux(moleculeName);
}


const Molecule* OpenChannel::Efflux(const Name& moleculeName) const
{
	return mTransMembraneDomain.Efflux(moleculeName);
}


Molecule* OpenChannel::Efflux(const Id& moleculeId)
{
	return mTransMembraneDomain.Efflux(moleculeId);
}


const Molecule* OpenChannel::Efflux(const Id& moleculeId) const
{
	return mTransMembraneDomain.Efflux(moleculeId);
}


Molecule* OpenChannel::Secrete(const Name& moleculeName)
{
	return mTransMembraneDomain.Secrete(moleculeName);
}


Molecule* OpenChannel::Secrete(const Id& moleculeId)
{
	return mTransMembraneDomain.Secrete(moleculeId);
}

} //molecular namespace
} //bio namespace
