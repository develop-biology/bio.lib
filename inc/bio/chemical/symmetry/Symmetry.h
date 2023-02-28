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

#include "bio/physical/symmetry/Symmetry.h"
#include "bio/physical/common/Class.h"
#include "bio/chemical/structure/motif/LinearMotif.h"
#include "bio/chemical/structure/Structure.h"
#include "bio/chemical/common/Class.h"
#include "bio/chemical/common/Filters.h"
#include "bio/chemical/bonding/Covalent.h"
#include "bio/chemical/Substance.h"

namespace bio {
namespace chemical {

/**
 * chemical::Symmetry adds a recursive structure to physical::Symmetry. This enables nesting and complex structures to form. <br />
 * Primarily, chemical::Symmetry allows for lists (chemical::Structures) and basic operations (chemical::Reactions) to be Rotated. <br />
 * <br />
 * All chemical::Symmetries have a Miscibility which determines how multiple Symmetries are Mixed. <br />
 * Mixing operates on Symmetries in order to provide a standard means of combining Biological objects. <br />
 * The default Miscibility is Passthrough(), which will cause all nested Symmetries to be Realized. <br />
 * To avoid superfluous updates when Realizing a highly-nested object, Symmetries will, by default, be non-recursive. This means that Reifying a Wave with *this should only update the Wave's initial value, and not check the nested Symmetries within this (e.g. not update member variables or sub-objects of the Wave*). <br />
 * There are 3 ways to enable recursive behavior: <br />
 * 1. Set the Miscibility to Passthrough(), this will cause all nested Symmetries (which are not themselves Passthrough Miscible) to be updated, but no more than that. <br />
 * 2. Set the mRealization of *this to NULL. This will enable full Reification on the first call only. After *this has been Reified, it will have a mRealization and will not seek to be fully Reified again unless otherwise specified. <br />
 * 3. Add the state::Recursive(). This can be done with the ease-of-use method: Recurse(). <br />
 */
class Symmetry :
	public chemical::Class< Symmetry >,
	public Substance,
	public physical::Symmetry,
	public Covalent< LinearMotif< physical::Symmetry* > >
{
public:

	/**
	 * Ensure virtual methods point to Class implementations. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(chemical, Symmetry)

	BIO_DEFAULT_IDENTIFIABLE_CONSTRUCTORS(
		chemical,
		Symmetry,
		filter::Chemical()
	)

	/**
	 * @param name
	 * @param type
	 */
	Symmetry(
		const Name& name,
		const Name& type
	);

	/**
	 * @param name
	 * @param type
	 */
	Symmetry(
		const Name& name,
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

	/**
	 * Set the mMiscibility of *this.
	 * @param miscibility
	 */
	virtual void SetMiscibility(const Miscibility& miscibility);

	/**
	 * @return the mMiscibility of *this.
	 */
	virtual const Miscibility& GetMiscibility() const;

	/**
	 * Helper method for setting the Recursive() State. <br />
	 * May be overridden to add custom behavior. <br />
	 */
	virtual void Recurse();

	/**
	 * Helper method for removing the Recursive() State. <br />
	 * May be overridden to add custom behavior. <br />
	 */
	virtual void DoNotRecurse();

	/**
	 * Helper method for checking the Recursive() State. <br />
	 * May be overridden to add custom checks. <br />
	 * @return whether or not *this has the Recursive() State.
	 */
	virtual bool IsRecursive() const;

protected:
	Miscibility mMiscibility;
};

} //chemical namespace
} //bio namespace
