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

#include "bio/physical/common/Types.h"
#include "bio/physical/relativity/Identifiable.h"
#include "bio/common/ByteStream.h"

namespace bio {
namespace physical {

/**
 * physical::Symmetry contains a Name, a type, and a value. It also contains some meta information, like when it was created, last updated, and the Wave it represents. <br />
 * You can think of Symmetry as the meta-information of Biology. It's used for serialization and other quality-of-life features. <br />
 * See SymmetryTypes.h for what types of Symmetry are available on a physical level. <br />
 * Both Symmetry and SymmetryTypes are intended to grow with each namespace, as the complexity of what is being Rotated grows. <br />
 */
class Symmetry :
	physical::Class< Symmetry >,
	virtual public Identifiable< Id >
{
public:

	/**
	 * Ensure virtual methods point to Class implementations. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(physical, Symmetry)

	/**
	 *
	 */
	Symmetry();

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
	 * With Symmetry, we make "Type" what is Identifiable and Name ancillary. <br />
	 * @return The Type of *this, aka an Identifiable<SymmetryType>.
	 */
	const Identifiable< SymmetryType >& GetType() const;

	/**
	 * Set the type of *this. <br />
	 * @param type
	 */
	void SetType(SymmetryType type);

	/**
	 * Set the type of *this. <br />
	 * @param type
	 */
	void SetType(const Name& type);

	/**
	 * Set the mValue of *this. <br />
	 * Also sets the TimeUpdated. <br />
	 * @param bytes
	 */
	virtual void SetValue(const ByteStream& bytes);

	/**
	 * Get the mValue of *this. <br />
	 * @return mValue for read only.
	 */
	virtual const ByteStream& GetValue() const;

	/**
	 * Get the mValue of *this for direct editing. <br />
	 * Also sets the TimeUpdated. <br />
	 * @return mValue for writing.
	 */
	virtual ByteStream* AccessValue();

	/**
	 * @return the time *this was first instantiated. <br />
	 */
	virtual const Timestamp& GetTimeCreated() const;

	/**
	 * @return the time *this was last written to. <br />
	 */
	virtual const Timestamp& GetTimeUpdated() const;

	/**
	 * Set the "owner" of *this. <br />
	 * Having a Realization allows a Symmetry to be Realized(). <br />
	 * @param realization
	 */
	virtual void SetRealization(Wave* realization);

	/**
	 * Realizing a Symmetry copies its mValue into its mRealization. <br />
	 * This allows Waves to be updated with values outside the Biology framework, creating a basic Object Relational Mapping (ORM) system. <br />
	 */
	virtual void Realize();

protected:
	ByteStream mValue;
	Identifiable< SymmetryType > mType;
	Timestamp mTimeCreated;
	Timestamp mTimeUpdated;
	Wave* mRealization;
};

} //physical namespace
} //bio namespace
