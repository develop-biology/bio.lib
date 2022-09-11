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

#pragma once

#include "bio/cellular/OrganSystem.h"
#include "bio/organic/common/Filters.h"
#include "bio/organic/common/Types.h"

namespace bio {
namespace organic {

class Habitat;

/**
 * Organisms are the basic units of "life" within Biology. <br />
 * They are organized in a hierarchical, Cell-centric manner. <br />
 * They grow and develop through Morphogenesis(). <br />
 * They can respond to stimuli, depending on the interface. <br />
 * They can reproduce, if you Clone() them. <br />
 * They can adapt to new environments, if you add in dynamic Plasmid & TranscriptionFactor rules. <br />
 * They "metabolize" time & data into their own organization. This is the primary difference between computational and biological (proper) life and makes these Organisms more "electric" or "ethereal" than "real". <br />
 * They do not age except perhaps through outside influences, like bitrot. <br />
 *
 * Does newing an Organism create life? Does deleting end it? <br />
 * How different are microbes from these designs? How different are we? <br />
 * Have fun! <br />
 */
class Organism :
	public cellular::Class< Organism >,
	public Horizontal< chemical::LinearMotif< cellular::OrganSystem* > >,
	public chemical::EnvironmentDependent< Habitat >
{
public:

	/**
	 * Ensure virtual methods point to Class implementations. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(cellular, Organism)

	/**
	 * Standard constructors. <br />
	 */
	BIO_DEFAULT_IDENTIFIABLE_CONSTRUCTORS(
		cellular,
		Organism,
		&OrganismPerspective::Instance(),
		filter::Organic()
	)

	/**
	 *
	 */
	virtual ~Organism();

	/**
	 * new your OrganSystems here, then call the parent method (Organism::Morphogenesis()), which will handle the Organ, Tissue, & Cell Differentiation, producing a fully functional Organism. <br />
	 */
	virtual Code Morphogenesis();
};

} //organic namespace
} //bio namespace
