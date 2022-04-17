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

#include "bio/chemical/structure/motif/LinearMotif.h"
#include "Molecule.h"
#include "bio/molecular/common/Types.h"

namespace bio {
namespace molecular {

/**
 * Vesicles define an inside / outside dichotomy between the Molecules on the "outside" of *this and the Molecules on the inside of *this. <br />
 * NOTE: The Molecules inside *this can, themselves, be Vesicles (thought they don't have to be). <br />
 * Another way to think of Vesicles is as a "vacuum" whereby Molecules can exist in an isolated context (which is probably not true of any real system). For example think of 1 Molecules on one side of a room and another on the other side; the Vesicle would be the Room and what is ultimately responsible for defining the scale by which the Molecule's separation can be measured. <br />
 */
class Vesicle :
	virtual public Molecule,
	public Class< Vesicle >,
	public chemical::LinearMotif< Molecule* > <br />
{
public:

	/**
	 * Ensure virtual methods point to Class implementations. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(molecular,
		Vesicle)

	/**
	 * Standard ctors. <br />
	 */ BIO_DEFAULT_IDENTIFIABLE_CONSTRUCTORS(molecular,
		Vesicle,
		&VesiclePerspective::Instance(),
		filter::Molecular())

	/**
	 * @param toCopy
	 */
	Vesicle(const Vesicle& toCopy);

	/**
	 *
	 */
	virtual ~Vesicle();

	/**
	 * operator wrappers around GetBy____< Molecule* > <br />
	 * @param moleculeId
	 * @return Extract(...)
	 * @{
	 */
	virtual Molecule* operator[](StandardDimension moleculeId); <br />

	virtual const Molecule* operator[](StandardDimension moleculeId) const; <br />

	template < typename T >
	Molecule* operator[](StandardDimension moleculeId) <br />
	{
		return ChemicalCast< T >(GetById< Molecule* >(moleculeId)); <br />
	}

	virtual Molecule* operator[](Name moleculeName); <br />

	virtual const Molecule* operator[](Name moleculeName) const; <br />

	template < typename T >
	Molecule* operator[](Name moleculeName) <br />
	{
		return ChemicalCast< T >(GetByName< Molecule* >(moleculeName)); <br />
	}
	/**@}*/

	/**
	 * Vesicle copy operation. <br />
	 * Copies all Molecules in the source Vesicle into *this. <br />
	 * @param source
	 * @return this
	 */
	virtual Vesicle* operator<<=(Vesicle* source); <br />

	/**
	 * Vesicle move operation. <br />
	 * Moves all Molecules in *this into the target Vesicle. <br />
	 * This REMOVES all Molecules from *this. <br />
	 * @param target
	 * @return target
	 */
	virtual Vesicle* operator>>=(Vesicle* target); <br />
};

} //molecular namespace
} //bio namespace
