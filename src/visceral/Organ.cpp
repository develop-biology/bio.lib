/*
 * This file is a part of the Biology project by eons LLC.
 * Biology (aka Develop Biology) is a framework for approaching software
 * development from a natural sciences perspective.
 *
 * Copyright (C) 2021 Séon O'Shannon & eons LLC
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

#include "bio/visceral/Organ.h"
#include "bio/visceral/common/Properties.h"
#include "bio/visceral/common/Filters.h"
#include "bio/visceral/common/Types.h"
#include "bio/visceral/reactions/DifferentiateTissue.h"

namespace bio {
namespace visceral {

Organ::Organ() :
	Class(this),
	chemical::LinearStructuralComponent< genetic::Plasmid >(&genetic::PlasmidPerspective::Instance()),
	chemical::LinearStructuralComponent< Tissue >(&TissuePerspective::Instance()),
	molecular::EnvironmentDependent<OrganSystem>()
{

}

Organ::Organ(StandardDimension id) :
	Class(this, id, &OrganPerspective::Instance(), filter::Visceral()),
	chemical::LinearStructuralComponent< genetic::Plasmid >(&genetic::PlasmidPerspective::Instance()),
	chemical::LinearStructuralComponent< Tissue >(&TissuePerspective::Instance()),
	molecular::EnvironmentDependent<OrganSystem>()
{

}

Organ::Organ(Name name) :
	Class(this, name, &OrganPerspective::Instance(), filter::Visceral()),
	chemical::LinearStructuralComponent< genetic::Plasmid >(&genetic::PlasmidPerspective::Instance()),
	chemical::LinearStructuralComponent< Tissue >(&TissuePerspective::Instance()),
	molecular::EnvironmentDependent<OrganSystem>()
{

}

Organ::Organ(const Organ& toCopy) :
	Class(this, toCopy.GetId(), toCopy.GetPerspective(), toCopy.GetFilter()),
	chemical::LinearStructuralComponent< genetic::Plasmid >(toCopy),
	chemical::LinearStructuralComponent< Tissue >(toCopy),
	molecular::EnvironmentDependent<OrganSystem>(toCopy)
{

}

Organ::~Organ()
{

}

void Organ::SpecializeTissues()
{
	ForEach<Tissue*>(chemical::Reaction::Initiate<DifferentiateTissue>());
}

} //visceral namespace
} //bio namespace
