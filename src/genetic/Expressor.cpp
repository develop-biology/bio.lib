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

#include "bio/genetic/Expressor.h"
#include "bio/genetic/common/Properties.h"
#include "bio/genetic/reactions/ExpressPlasmid.h"

namespace bio {
namespace genetic {

Expressor::Expressor() :
	chemical::StructuralComponent<TranscriptionFactor>(),
	chemical::LinearStructuralComponent< const Plasmid* >(),
	chemical::LinearStructuralComponent< Protein* >(),
	molecular::Vesicle()
{
}

Expressor::Expressor(Name name) :
	chemical::StructuralComponent<TranscriptionFactor>(),
	chemical::LinearStructuralComponent< const Plasmid* >(),
	chemical::LinearStructuralComponent< Protein* >(),
	molecular::Vesicle(),
	physical::Identifiable<StandardDimension>(name, &molecular::VesiclePerspective::Instance())
{
}

Expressor::Expressor(const Expressor& other) :
	chemical::StructuralComponent<TranscriptionFactor>(other),
	chemical::LinearStructuralComponent< const Plasmid* >(other),
	chemical::LinearStructuralComponent< Protein* >(other),
	molecular::Vesicle(other)
{

}

Expressor::~Expressor()
{

}

Expressor* Expressor::Clone() const
{
	return new Expressor(*this);
}


bool Expressor::operator==(const Expressor& other) const
{
	return chemical::StructuralComponent<TranscriptionFactor>::operator==(other) && molecular::Molecule::operator==(other);
}


void Expressor::ImportAll(const Expressor& other)
{
	chemical::Substance::ImportAll(other);
	Import<TranscriptionFactor>(other);
	Import<Plasmid*>(other);
}

Code Expressor::Activate(StandardDimension proteinId)
{
	ret = code::Success();
	chemical::Reaction::Attempt<ActivateProtein>(GetById<Protein*>(proteinId, false));
	return ret; //TODO: Get Code from children?
}

Code Expressor::Activate(Name proteinName)
{
	return Activate(ProteinPerspective::Instance().IdFromName(proteinName));
}

Code Expressor::BeginExpressing()
{
	ForEach<Plasmid*>(chemical::Reaction::Initiate<ExpressPlasmid>(), this);
}

} //molecular namespace
} //bio namespace
