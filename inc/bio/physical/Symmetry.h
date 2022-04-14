/*
 * This file is a part of the Biology project by eons LLC.
 * Biology (aka Develop Biology) is a framework for approaching software
 * development from a natural sciences perspective.
 *
 * Copyright (C) 2021 Séon O'Shannon & eons LLC
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

#include "bio/physical/common/Types.h"
#include "Identifiable.h"
#include "bio/common/ByteStream.h"

namespace bio {
namespace physical {

/**
 * physical::Symmetry only contains a Name, a type, and a value.
 * See SymmetryTypes.h for what types of Symmetry are available on a physical level.
 * Both Symmetry and SymmetryTypes are intended to grow with each namespace, as the complexity of what is being Rotated grows.
 */
class Symmetry :
	virtual public Identifiable< StandardDimension >,
	Class< Symmetry >
{
public:

	/**
	 * Ensure virtual methods point to Class implementations.
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(physical,
		Symmetry)

	/**
	 *
	 */
	Symmetry();

	/**
	 * @param name
	 * @param type
	 */
	Symmetry(
		Name name,
		Name type
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
		StandardDimension id,
		Name type
	);

	/**
	 * @param id
	 * @param type
	 */
	Symmetry(
		StandardDimension id,
		SymmetryType type
	);

	/**
	 *
	 */
	virtual ~Symmetry();

	/**
	 * With Symmetry, we make "Type" what is Identifiable and Name ancillary.
	 * @return The Type of *this, akan Identifiable<SymmetryType>::GetName().
	 */
	const Identifiable< SymmetryType >& GetType() const;

	/**
	 * Set the type of *this.
	 * @param type
	 */
	void SetType(SymmetryType type);

	/**
	 * Set the type of *this.
	 * @param type
	 */
	void SetType(Name type);

	/**
	 * Set the m_value of *this.
	 * @param bytes
	 */
	virtual void SetValue(const ByteStream& bytes);

	/**
	 * Get the m_value of *this.
	 * @return m_value for read only.
	 */
	virtual const ByteStream& GetValue() const;

	/**
	 * Get the m_value of *this for direct editing.
	 * @return m_value for writing.
	 */
	virtual ByteStream* AccessValue();

protected:
	Name m_name;
	ByteStream m_value;
	Identifiable< SymmetryType > m_type;
};

} //physical namespace
} //bio namespace
