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
 * Vesicles define an inside / outside dichotomy between the Surfaces on the "outside" of *this and the Molecules on the inside of *this. <br />
 * NOTE: The Molecules inside *this can, themselves, be Vesicles (thought they don't have to be). <br />
 * Another way to think of Vesicles is as a "vacuum" whereby Molecules can exist in an isolated context. <br />
 * For example think of one Molecule on one side of a room and another on the other side; the Vesicle would be the room and what is ultimately responsible for defining the scale by which the Molecule's separation can be measured. <br />
 * Functionally, what Vesicle does is implement the "protected" keyword. <br />
 * Thus, Vesicles may only interact with each other through what they expose on their Surfaces. <br />
 */
class Vesicle :
	public Class< Vesicle >,
	protected chemical::LinearMotif< Molecule* >,
	virtual public Molecule
{
	friend class TransMembraneDomain;
	friend BIO_EXCITATION_CLASS(chemical::LinearMotif< Molecule* >, Molecule*, Name); //For genetic Localization and Insertion.

public:

	/**
	 * Ensure virtual methods point to Class implementations. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(molecular, Vesicle)

	/**
	 * Standard ctors. <br />
	 */
	BIO_DEFAULT_IDENTIFIABLE_CONSTRUCTORS(molecular,
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

	
protected:
	
	/**
	 * operator wrappers around GetBy____< Molecule* > 
	 * @param moleculeId
	 * @return GetBy____< Molecule* >
	 */
	virtual Molecule* operator[](const Id& moleculeId);

	/**
	 * operator wrappers around GetBy____< Molecule* > 
	 * @param moleculeId
	 * @return GetBy____< Molecule* >
	 */
	virtual const Molecule* operator[](const Id& moleculeId) const;

	/**
	 * operator wrappers around GetBy____< Molecule* > 
	 * @tparam T
	 * @param moleculeId
	 * @return GetBy____< Molecule* >
	 */
	template < typename T >
	Molecule* operator[](const Id& moleculeId)
	{
		return ChemicalCast< T >(GetById< Molecule* >(moleculeId));
	}

	/**
	 * operator wrappers around GetBy____< Molecule* > 
	 * @param moleculeName
	 * @return GetBy____< Molecule* >
	 */
	virtual Molecule* operator[](const Name& moleculeName);

	/**
	 * operator wrappers around GetBy____< Molecule* > 
	 * @param moleculeName
	 * @return GetBy____< Molecule* >
	 */
	virtual const Molecule* operator[](const Name& moleculeName) const;

	/**
	 * operator wrappers around GetBy____< Molecule* >
	 * @tparam T
	 * @param moleculeName
	 * @return GetBy____< Molecule* >
	 */
	template < typename T >
	Molecule* operator[](const Name& moleculeName)
	{
		return ChemicalCast< T >(GetByName< Molecule* >(moleculeName));
	}

};

} //molecular namespace
} //bio namespace
