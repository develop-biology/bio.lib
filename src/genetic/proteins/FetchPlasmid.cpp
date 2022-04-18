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

#include "bio/genetic/proteins/FetchPlasmid.h"
#include "bio/genetic/Plasmid.h"
#include "bio/genetic/common/Types.h"

namespace bio {
namespace genetic {

FetchPlasmid::FetchPlasmid()
	:
	molecular::Protein(chemical::PeriodicTable::Instance().GetNameFromType(*this))
{
	mc_nameSite = Define("Name Binding Site");
	mc_idSite = Define("Id Binding Site");
	mc_returnSite = Define("Return Site");
}

FetchPlasmid::~FetchPlasmid()
{

}

Code FetchPlasmid::Activate()
{
	Code ret = code::BadArgument1();

	RotateTo(mc_returnSite)->ReleaseAll();

	Name boundName = RotateTo< Name >(mc_nameSite);
	StandardDimension boundId = RotateTo< StandardDimension >(mc_idSite);

	if (boundName)
	{
		RotateTo(mc_returnSite)->Bind(PlasmidPerspective::Instance().GetTypeFromIdAs< Plasmid* >(boundId)); 
		ret = code::Success();
	}
	else if (boundId)
	{
		RotateTo(mc_returnSite)->Bind(PlasmidPerspective::Instance().GetTypeFromIdAs< Plasmid* >(boundId)); 
		ret = code::Success();
	}

	RotateTo(mc_nameSite)->ReleaseAll();
	RotateTo(mc_idSite)->ReleaseAll();

	return ret;
}

} //genetic namespace
} //bio namespace
