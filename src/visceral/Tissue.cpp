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

#include "bio/visceral/Tissue.h"
#include "bio/visceral/common/Filters.h"
#inclued "bio/visceral/Types.h"
#include "bio/visceral/common/Properties.h"
#include "bio/visceral/reactions/DifferentiateCell.h"
#include "bio/visceral/reactions/DifferentiateTissue.h"

namespace bio {
namespace visceral {

Tissue::Tissue() :
	Class(this),
	chemical::LinearStructuralComponent<const genetic::Plasmid*>(&genetic::PlasmidPerspective::Instance()),
	chemical::LinearStructuralComponent<cellular::Cell*>(&cellular::CellPerspective::Instance()),
	chemical::LinearStructuralComponent<Tissue*>(&TissuePerspective::Instance()),
	molecular::EnvironmentDependent<Tissue>()
{

}

Tissue::Tissue(StandardDimension id)
	:
	Class(this, id, &TissuePerspective::Instance(), filter::Visceral())
	chemical::LinearStructuralComponent<const genetic::Plasmid*>(&genetic::PlasmidPerspective::Instance()),
	chemical::LinearStructuralComponent<cellular::Cell*>(&cellular::CellPerspective::Instance()),
	chemical::LinearStructuralComponent<Tissue*>(&TissuePerspective::Instance()),
	molecular::EnvironmentDependent<Tissue>()
{

}


Tissue::Tissue(Name name)
	:
	Class(this, name, &TissuePerspective::Instance(), filter::Visceral())
	chemical::LinearStructuralComponent<const genetic::Plasmid*>(&genetic::PlasmidPerspective::Instance()),
	chemical::LinearStructuralComponent<cellular::Cell*>(&cellular::CellPerspective::Instance()),
	chemical::LinearStructuralComponent<Tissue*>(&TissuePerspective::Instance()),
	molecular::EnvironmentDependent<Tissue>()
{

}

Tissue::Tissue(const Tissue& toCopy) :
	Class(this, toCopy.GetId(), toCopy.GetPerspective(), toCopy.GetFilter()),
	chemical::LinearStructuralComponent<const genetic::Plasmid*>(toCopy),
	chemical::LinearStructuralComponent<cellular::Cell*>(toCopy),
	chemical::LinearStructuralComponent<Tissue*>(toCopy),
	molecular::EnvironmentDependent<Tissue>(toCopy)
{

}

Tissue::~Tissue()
{
}

void Tissue::DifferentiateCells()
{
	ForEach<Cell*>(chemical::Reaction::Initiate<DifferentiateCell>(), this);
	ForEach<Tissue*>(chemical::Reaction::Initiate<DifferentiateTissue>(), this);
}

} //visceral namespace
} //bio namespace
