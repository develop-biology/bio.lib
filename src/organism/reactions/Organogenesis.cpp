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

#pragma once

#include "bio/organism/reactions/Organogenesis.h"
#include "bio/organism/Properties.h"
#include "bio/visceral/Organ.h"
#include "bio/visceral/OrganSystem.h"
#include "bio/chemical/PeriodicTable.h"
#include "bio/chemical/SymmetryTypes.h"

namespace bio {

Organogenesis::Organogenesis() :
	chemical::Reaction(PeriodicTable::Instance().GetNameFromType(*this)),
	physical::Class(this, new physical::Symmetry(PeriodicTable::Instance().GetNameFromType(*this),symmetry_type::Operation()))
{
	Require<Visceral::OrganSystem>();
	Require<Organism>();
}

Organogenesis::~Organogenesis()
{
}

Products Organogenesis::Process(chemical::Substances& reactants)
{
	visceral::OrganSystem* organSystem = reactants[0];
	organSystem->SetEnvironment(reactants[1]);
	organSystem->Organogenesis();
	return reactants;
}

} //bio namespace