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

#include "Atom.h"
#include "bio/chemical/common/BondTypes.h"

namespace bio {
//not chemical.

/**
 * Covalent classes can be used to create "horizontal inheritance". <br />
 * When instantiating a Covalent class, a new T will be created and Covalently bonded to the child class of *this (through the shared Atom). <br />
 * NOTE: atm, there is no way to provide arguments to the new Ts. <br />
 * NOTE: Bonding a child class will not bond that class's parents. Virtually inherited Atoms are still distinct objects. If you would like access to a Bonded object's parents, use a Metallic Bond. <br />
 * Covalent inheritance allows distinct objects to be treated as a one. See Atom.h for more on Bonds.<br />
 * @tparam T
 */
template < class T >
class Covalent :
	virtual public chemical::Atom
{
public:
	/**
	 *
	 */
	Covalent() :
		mT(new T())
	{
		FormBond(mT, bond_type::Covalent());
	}

	/**
	 *
	 */
	virtual ~Covalent()
	{
		delete mT;
	}

	/**
	 * @return what *this was created for.
	 */
	T* Object()
	{
		return mT;
	}

	/**
	 * @return what *this was created for.
	 */
	const T* Object() const
	{
		return mT;
	}

protected:
	T* mT;
};

} //bio namespace
