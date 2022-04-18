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

#include "bio/chemical/common/Types.h"
#include "bio/chemical/common/BondTypes.h"
#include "bio/physical/arrangement/Arrangement.h"

namespace bio {

namespace physical {
class Wave;
}

namespace chemical {

/**
 * Bonds are used by Atom to form relationships to Waves. <br />
 * These can be thought of as edges in a directed graph. <br />
 * See Atom.h for more info. <br />
 *
 * NOTE: Bonds may currently only be of a single BondType. This may change in a future release. <br />
 */
class Bond
{
public:
	/**
	 *
	 */
	Bond();

	/**
	 * @param id
	 * @param bonded
	 * @param type
	 */
	Bond(
		AtomicNumber id,
		physical::Wave* bonded, 
		BondType type = bond_type::Unknown());

	/**
	 *
	 */
	~Bond();

	/**
	 * @param id
	 * @return whether or not the given id matches that of *this.
	 */
	bool operator==(const AtomicNumber id) const;

	/**
	 * @param other
	 * @return whether or not id of other matches that of *this.
	 */
	bool operator==(const Bond& other) const;

	/**
	 * Update the contents of *this. <br />
	 * Only works if *this IsEmpty(). <br />
	 * @param id
	 * @param bonded
	 * @param type
	 * @return true if *this was updated; false otherwise (e.g. if *this is already pointing to something)
	 */
	bool Form(
		AtomicNumber id,
		physical::Wave* bonded, 
		BondType type = bond_type::Unknown());

	/**
	 * @return the m_id of *this.
	 */
	AtomicNumber GetId() const;

	/**
	 * @return the m_bonded of *this.
	 */
	physical::Wave* GetBonded(); 

	/**
	 * @return the m_bonded of *this.
	 */
	const physical::Wave* GetBonded() const; 

	/**
	 * @return the m_type of *this.
	 */
	BondType GetType() const;

	/**
	 * @return whether or not *this points to anything.
	 */
	bool IsEmpty() const;

	/**
	 * Empties the contents of *this. <br />
	 */
	void Break();

private:
	AtomicNumber m_id;
	physical::Wave* m_bonded; 
	BondType m_type;
};

typedef physical::Arrangement< Bond* > Bonds; 

} //chemical namespace
} //bio namespace
