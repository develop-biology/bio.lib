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

#include "bio/genetic/protein/FetchPlasmid.h"
#include "bio/genetic/Plasmid.h"
#include "bio/genetic/common/Types.h"

namespace bio {
namespace genetic {

FetchPlasmid::FetchPlasmid()
	:
	genetic::Class< FetchPlasmid >(
		this,
		"FetchPlasmid",
		filter::Genetic()
	),
	molecular::Protein(SafelyAccess<chemical::PeriodicTable>()->GetNameFromType(*this))
{
	mcNameSite = Define("Name Binding Site");
	mcIdSite = Define("Id Binding Site");
	mcReturnSite = Define("Return Site");
}

FetchPlasmid::~FetchPlasmid()
{

}

Code FetchPlasmid::Activate()
{
	Code ret = code::BadArgument1();

	RotateTo(mcReturnSite)->Release();

	Name& boundName = RotateTo(mcNameSite)->Probe< Name >();
	Id& boundId = RotateTo(mcIdSite)->Probe< Id >();

	Plasmid* boundPlasmid = NULL;
	if (boundName)
	{
		boundPlasmid = SafelyAccess<PlasmidPerspective>()->GetTypeFromNameAs< Plasmid* >(boundName);
	}
	else if (boundId)
	{
		boundPlasmid = SafelyAccess<PlasmidPerspective>()->GetTypeFromIdAs< Plasmid* >(boundId);
	}

	if (boundPlasmid)
	{
		RotateTo(mcReturnSite)->Bind(*boundPlasmid);
		ret = code::Success();
	}

	RotateTo(mcNameSite)->Release();
	RotateTo(mcIdSite)->Release();

	return ret;
}

} //genetic namespace
} //bio namespace
