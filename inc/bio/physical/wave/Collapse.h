/*
 * This file is a part of the Biology project by eons LLC.
 * Biology (aka Develop Biology) is a framework for approaching software
 * development from a natural sciences perspective.
 *
 * Copyright (C) 2023 Séon O'Shannon & eons LLC
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

#include "bio/physical/common/Class.h"
#include "bio/physical/common/Superpositions.h"
#include "bio/physical/relativity//Identifiable.h"

namespace bio {
namespace physical {

/**
 * Collapse handles Interference by providing a function which can be associated with each Superposition. <br />
 * Thus, instead of implementing the behavior each Superposition has on your complex Wave, you can simply call Collapse::Measure(Superposition, ...) .<br />
 * Collapses automatically register themselves with the SuperpositionPerspective. <br />
 */
class Collapse :
	public physical::Class< Collapse >,
	public physical::Identifiable< Superposition >
{
public:

	/**
	 * Ensure virtual methods point to Class implementations. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(physical, Collapse)

	/**
	 * @param applyTo
	 */
	Collapse(const Superposition& applyTo);

	/**
	 *
	 */
	virtual ~Collapse();

	/**
	 * Perform some operation and return the result, whatever that is. <br />
	 * It's generally expected that the result will be assigned to the first Wave's value, whatever that is. <br />
	 * This in intentionally very generic. Use it to implement any behavior you want! <br />
	 * @param waves
	 * @return like, whatever. man.
	 */
	virtual ByteStream operator()(const ConstWaves& waves) const;

	/**
	 * This is a convenience function which calls the operator() function of the appropriate Collapse object. <br />
	 * @param superposition
	 * @param waves
	 * @return the result of Collapsing the given superposition.
	 */
	static ByteStream Measure(const Superposition& superposition, const ConstWaves& waves);
};

} // namespace physical
} // namespace bio
