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

#include "bio/organism/Habitat.h"
#include "bio/organism/Filters.h"
#include "bio/organism/reactions/Morphogenesis.h"

namespace bio {

Habitat::Habitat()
	:
	visceral::Class(this),
	chemical::LinearStructuralComponent<Organism*>(&OrganismPerspective::Instance()),
	physical::ThreadedPeriodic()
{

}

Habitat::Habitat(StandardDimension id)
	:
	visceral::Class(this, id, &HabitatPerspective::Instance(), filter::Habitat()),
	chemical::LinearStructuralComponent<Organism*>(&OrganismPerspective::Instance()),
	physical::ThreadedPeriodic()
{

}

Habitat::Habitat(Name name)
	:
	visceral::Class(this, name, &HabitatPerspective::Instance(), filter::Habitat()),
	chemical::LinearStructuralComponent<Organism*>(&OrganismPerspective::Instance()),
	physical::ThreadedPeriodic()
{

}

Habitat::Habitat(const Habitat& toCopy)
	:
	visceral::Class(this, toCopy.GetId(), toCopy.GetPerspective(), toCopy.GetFilter()),
	chemical::LinearStructuralComponent<Organism*>(toCopy),
	physical::ThreadedPeriodic(toCopy)
{

}

Habitat::~Habitat()
{

}

void Habitat::AdaptInhabitants()
{
	ForEach<Organism*>(
		chemical::Reaction::Initiate<Morphogenesis>(),
		this
	);
}

} //bio namespace