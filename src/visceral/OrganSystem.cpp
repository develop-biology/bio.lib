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

#include "bio/visceral/OrganSystem.h"
#include "bio/visceral/Types.h"
#include "bio/visceral/Filters.h"
#include "bio/visceral/Properties.h"
#include "bio/visceral/reactions/GrowOrgan.h"

namespace bio {
namespace visceral {

OrganSystem::OrganSystem()
	:
	Class(this),
	chemical::LinearArrangment<Organ*>(&OrganPerspective::Instance()),
	molecular::EnvironmentDependent<Organism>()
{
}

OrganSystem::OrganSystem(StandardDimension id)
	:
	Class(this, id, &OrganSystemPerspective::Instance(), filter::Visceral()),
	chemical::LinearArrangement<Organ*>(&OrganPerspective::Instance()),
	molecular::EnvironmentDependent<Organism>()
{

}

OrganSystem::OrganSystem(Name name)
	:
	Class(this, name, &OrganSystemPerspective::Instance(), filter::Visceral()),
	chemical::LinearArrangement<Organ*>(&OrganPerspective::Instance()),
	molecular::EnvironmentDependent<Organism>()
{

}

OrganSystem::OrganSystem(const OrganSystem& toCopy)
	:
	Class(this, toCopy.GetId(), toCopy.GetPerspective(), toCopy.GetFilter()),
	chemical::LinearArrangement<Organ*>(toCopy),
	molecular::EnvironmentDependent<Organism>(toCopy)
{

}

OrganSystem::~OrganSystem()
{

}

void OrganSystem::Organogenesis()
{
	ForEach<Organ*>(
		chemical::Reaction::Initiate<GrowOrgan>(),
		this
	);
}

} //visceral namespace
} //bio namespace