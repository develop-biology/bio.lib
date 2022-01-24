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

#include "bio/organism/reactions/GrowOrgan.h"
#include "bio/visceral/Organ.h"
#include "bio/visceral/OrganSystem.h"
#include "bio/visceral/Properties.h"
#include "bio/chemical/PeriodicTable.h"
#include "bio/chemical/SymmetryTypes.h"

namespace bio {
namespace visceral {

GrowOrgan::GrowOrgan() :
	chemical::Reaction(PeriodicTable::Instance().GetNameFromType(*this)),
	physical::Class(this, new physical::Symmetry(PeriodicTable::Instance().GetNameFromType(*this),symmetry_type::Operation()))
{
	Require<Organ>();
	Require<OrganSystem>();
}

GrowOrgan::~GrowOrgan()
{
}

Products GrowOrgan::Process(chemical::Substances& reactants)
{
	Organ* organ = reactants[0];
	organ->SetEnvironment(reactants[1]);
	organ->BuildMobilome();
	organ->GrowTissues();
	organ->SpecializeTissues();
	return reactants;
}

} //visceral namespace
} //bio namespace