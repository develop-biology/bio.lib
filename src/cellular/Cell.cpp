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

#include "bio/cellular/Cell.h"
#include "bio/cellular/Organelle.h"
#include "bio/cellular/Tissue.h"

namespace bio {
namespace cellular {

Cell::~Cell()
{

}

Code Cell::ExpressGenes()
{
	Code ret = code::Success();
	Organelle* organelle;
	for (
		SmartIterator org = GetAll< Organelle* >()->Begin();
		!org.IsAfterEnd();
		++org
	) {
		organelle = org;
		organelle->Import< genetic::Plasmid* >(As< chemical::LinearMotif< genetic::Plasmid* >* >());
		if (organelle->ExpressGenes() != code::Success() && ret == code::Success())
		{
			ret = code::UnknownError();
		}
	}
	if (genetic::Expressor::ExpressGenes() != code::Success() && ret == code::Success())
	{
		ret = code::UnknownError();
	}
	return ret;
}

bool Cell::IsWithinTissue(const Id& tissueId) const
{
	if (!GetEnvironment< Tissue* >())
	{
		return false;
	}
	return GetEnvironment< Tissue* >()->IsWithinTissue(tissueId);
}

bool Cell::IsWithinTissue(const Name& name) const
{
	return IsWithinTissue(IdPerspective::Instance().GetIdFromName(name));
}

} //cellular
} //bio
