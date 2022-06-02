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

#include "bio/physical/Symmetry.h"
#include "bio/physical/common/Class.h"
#include "bio/chemical/structure/motif/LinearMotif.h"
#include "bio/chemical/structure/Structure.h"
#include "bio/chemical/common/Class.h"

namespace bio {
namespace chemical {

/**
 * chemical::Symmetry adds a recursive structure to physical::Symmetry. This enables nesting and complex structures to form. <br />
 * Primarily, chemical::Symmetry allows for lists (chemical::Structures) and basic operations (chemical::Reactions) to be Rotated. <br />
 */
class Symmetry :
	public physical::Symmetry,
	public LinearMotif< Symmetry* >,
	public chemical::Class< Symmetry >,
	virtual public Structure
{
public:

	/**
	 * Ensure virtual methods point to Class implementations. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(chemical,
		Symmetry)

	BIO_DEFAULT_IDENTIFIABLE_CONSTRUCTORS(chemical,
		Symmetry,
		&physical::SymmetryPerspective::Instance())

	/**
	 * @param name
	 * @param type
	 */
	Symmetry(
		Name name,
		const Name& type
	);

	/**
	 * @param name
	 * @param type
	 */
	Symmetry(
		Name name,
		SymmetryType type
	);

	/**
	 * @param id
	 * @param type
	 */
	Symmetry(
		Id id,
		const Name& type
	);

	/**
	 * @param id
	 * @param type
	 */
	Symmetry(
		Id id,
		SymmetryType type
	);

	/**
	 *
	 */
	virtual ~Symmetry();
};

} //chemical namespace
} //bio namespace
