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
		filter::Molecular(),
		symmetry_type::Value())
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
		filter::Molecular(),
		symmetry_type::Value())
{

}

OpenChannel::~OpenChannel()
{

}

void OpenChannel::IngressSolute(chemical::Solute* external)
{
	mTransMembraneDomain.IngressSolute(external);
}

void OpenChannel::IngressSolution(chemical::Solution* external)
{
	mTransMembraneDomain.IngressSolution(external);
}

chemical::Solute OpenChannel::Egress(const Name& soluteName)
{
	return mTransMembraneDomain.Egress(soluteName);
}

chemical::Solute OpenChannel::Egress(const Id& soluteId)
{
	return mTransMembraneDomain.Egress(soluteId);
}

chemical::Solute* OpenChannel::Secrete(const Name& soluteName)
{
	return mTransMembraneDomain.Secrete(soluteName);
}


chemical::Solute* OpenChannel::Secrete(const Id& soluteId)
{
	return mTransMembraneDomain.Secrete(soluteId);
}

} //molecular namespace
} //bio namespace
