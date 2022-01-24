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

#include "bio/organism/Organism.h"
#include "bio/organism/Filters.h"
#include "bio/organism/reactions/Organogenesis.h"

namespace bio {

Organism::Organism()
	:
	visceral::Class(this),
	chemical::LinearStructuralComponent<OrganSystem*>(&visceral::OrganSystemPerspective::Instance()),
	molecular::EnvironmentDependent<Habitat>()
{

}

Organism::Organism(StandardDimension id)
	:
	visceral::Class(this, id &OrganismPerspective::Instance(), filter::Organism()),
	chemical::LinearStructuralComponent<OrganSystem*>(&visceral::OrganSystemPerspective::Instance()),
	molecular::EnvironmentDependent<Habitat>()
{

}


Organism::Organism(Name name)
	:
	visceral::Class(this, name &OrganismPerspective::Instance(), filter::Organism()),
	chemical::LinearStructuralComponent<OrganSystem*>(&visceral::OrganSystemPerspective::Instance()),
	molecular::EnvironmentDependent<Habitat>()
{

}

Organism::Organism(const Organism& toCopy) :
	visceral::Class(this, toCopy.GetId(), toCopy.GetPerspective(), toCopy.GetFilter()),
	chemical::LinearStructuralComponent<visceral::OrganSystem*>(toCopy),
	molecular::EnvironmentDependent<Habitat>(toCopy)
{

}

Organism::~Organism()
{
}

void Organism::Morphogenesis()
{
	ForEach<visceral::OrganSystem*>(chemical::Reaction::Initiate<Organogenesis>(), this);
}

} //bio namespace