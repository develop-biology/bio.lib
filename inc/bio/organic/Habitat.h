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

#include "Organism.h"
#include "bio/physical/ThreadedPeriodic.h"

namespace bio {
namespace organic {

/**
 * Habitat: Is a home! <br />
 * Habitat: (say it with me) Is a home! <br />
 *
 * This is where your Organisms live! <br />
 * Each Habitat is it's own thread. Once your Organisms are Adapted to *this, you can Start() the thread and they'll come to life! <br />
 */

class Habitat :
	public cellular::Class< Habitat >,
	public chemical::LinearMotif< Organism* >, 
	public physical::ThreadedPeriodic
{
public:

	/**
	 * Ensure virtual methods point to Class implementations. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(cellular,
		Habitat)

	/**
	 * Standard ctors. <br />
	 */ BIO_DEFAULT_IDENTIFIABLE_CONSTRUCTORS(cellular,
		Habitat,
		&HabitatPerspective::Instance(),
		filter::Organic())

	/**
	 *
	 */
	virtual ~Habitat();

	/**
	 * Causes each Organism to undergo Morphogenesis, after which, they will be ready to live here. <br />
	 */
	virtual Code AdaptInhabitants();
};

} //organic namespace
} //bio namespace
