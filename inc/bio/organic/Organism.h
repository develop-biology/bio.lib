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

#include "bio/chemical/structure/LinearStructuralComponent.h"
#include "bio/molecular/EnvironmentDependent.h"
#include "bio/visceral/common/Class.h"

namespace bio {

namespace visceral {
class OrganSystem;
}

class Habitat;

/**
 * Organisms are the basic units of "life" within Biology.
 * They are organized in a hierarchical, Cell-centric manner.
 * They grow and develop through Morphogenesis().
 * They can respond to stimuli, depending on the Organism.
 * They can reproduce, if you Clone() them.
 * They can adapt to new environments, if you add in dynamic Plasmid & TranscriptionFactor rules.
 * They "metabolize" time & data into their own organization. This is the primary difference between computational and biological (proper) life and makes these Organisms more "electric" or "ethereal" than "real".
 * They do not age except perhaps through outside influences, like bitrot.
 *
 * Does newing an Organism create life? Does deleting end it?
 * How different are microbes from these designs? How different are we?
 * Have fun!
 */
class Organism :
	public visceral::Class<Organism>,
	public chemical::LinearStructuralComponent< visceral::OrganSystem* >,
	public molecular::EnvironmentDependent<Habitat>,
{
public:

	/**
	 * @param name what's our new friend's name???
	 */
	Organism(Name name);

	/**
	 * @param id Does our new friend have an Id already?
	 */
	Organism(StandardDimension id);

	/**
	 *
	 */
	Organism();

	/**
	 * Copies all contents of the given Organism into *this.
	 * NOTE: Cells are not functionally when copied. To make the new Cells in *this work, you must also call DifferentiateCells().
	 * @param toCopy
	 */
	Organism(const Organism& toCopy);

	/**
	 *
	 */
	virtual ~Organism();

	/**
	 * new your OrganSystems here, then call the parent method (Organism::Morphogenesis()), which will handle the Organ, Tissue, & Cell Differentiation, producing a fully functional Organism.
	 */
	virtual void Morphogenesis();
};

} //bio namespace
