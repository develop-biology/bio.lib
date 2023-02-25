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

#include "Particle.h"

namespace bio {
namespace physical {

/**
 * What a single "point" in space contains. <br />
 * Branes are taken from super string theory and are the multi-dimensional abstraction of membranes. <br />
 * Because multiple types can share names (e.g. int myVar vs float myVar), each Brane must only contain data common to all "points" in the DIMENSION they inhabit (see relativity/Perspective.h for DIMENSION definition). <br />
 * Children of Perspective<> may extend Brane to fill more "complex" DIMENSIONS (all DIMENSIONS are simple numbers but that says nothing about what each number means). <br />
 * <br />
 * For something to exist within a Dimension, as we define it, the thing must: <br />
 * 1. be of lower dimension than the space its found in (this is true by definition and thus ignored). <br />
 * 2. be locatable within that dimension, i.e. take up space. <br />
 * 3. consist of some substance, have some effect, or have the potential to affect and does not simply take up space. <br />
 * Branes must be locatable by a single id for a 1 dimensional array, 2 for a 2 dimensional array and so on. For a Brane to require 2 indices and exist within a 1 dimensional array is a logic error. <br />
 * At a minimum, a Brane must have an id to locate it and something else. We have made that something else a user-friendly(ier) string. All other Branes will have an id and a name and may have other contents as well. <br />
 * @tparam DIMENSION
 */
template< typename DIMENSION >
class Brane : public Particle
{
public:
	Brane(
		const DIMENSION& id,
		const Name& name
	)
		:
		mId(id),
		Particle(name)
	{
	}

	virtual ~Brane()
	{
	}

	DIMENSION mId;
};

} //physical namespace
} //bio namespace
