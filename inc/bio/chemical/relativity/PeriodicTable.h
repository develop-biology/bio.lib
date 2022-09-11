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

#include "Element.h"
#include "bio/common/type/NakedTypeName.h"
#include "bio/chemical/common/Types.h"
#include "bio/physical/relativity/TypedPerspective.h"

namespace bio {
namespace chemical {

/**
 * The PeriodicTable is a stand-in for runtime type information (RTTI). <br />
 * Instead of having to rely on code compiled with RTTI enabled, we implement our own type tracking mechanism. <br />
 *
 * The PeriodicTable tracks Properties of types in addition to assigning them an AtomicNumber. <br />
 * This is done because neither static nor virtual methods will allow for the inverted inheritance necessary to make methods like Atom::Attenuate work properly. <br />
 * See Elementary.h for a more detailed description of this system. <br />
 */
class PeriodicTableImplementation :
	public physical::TypedPerspective< AtomicNumber >
{
public:

	/**
	 *
	 */
	PeriodicTableImplementation();

	/**
	 *
	 */
	virtual ~PeriodicTableImplementation();

	/**
	 * @tparam T
	 * @return the Name associated with the given type.
	 */
	template < typename T >
	Name GetNameFromType() const
	{
		return type::NakedTypeName< T >();
	}

	/**
	 * For auto template determination. <br />
	 * @tparam T
	 * @param t
	 * @return the Name associated with the given type.
	 */
	template < typename T >
	Name GetNameFromType(const T t) const
	{
		return GetNameFromType< T >();
	}

	/**
	 * @tparam T
	 * @return the AtomicNumber associated with the given type.
	 */
	template < typename T >
	AtomicNumber GetIdFromType()
	{
		return GetIdFromName(GetNameFromType< T >());
	}

	/**
	 * For auto template determination. <br />
	 * @tparam T
	 * @param t
	 * @return the AtomicNumber associated with the given type.
	 */
	template < typename T >
	AtomicNumber GetIdFromType(const T t)
	{
		return GetIdFromType< T >();
	}

	/**
	 * @param id
	 * @return whatever properties have been Recorded for the given type.
	 */
	const Properties& GetPropertiesOf(AtomicNumber id) const;

	/**
	 * @param name
	 * @return whatever properties have been Recorded for the given type.
	 */
	const Properties& GetPropertiesOf(const Name& name) const;

	/**
	 * @tparam T
	 * @return whatever properties have been Recorded for the given type.
	 */
	template < typename T >
	const Properties& GetPropertiesOf() const
	{
		return GetPropertiesOf(type::NakedTypeName< T >());
	}

	/**
	 * Add a Property to the given type's record in *this. <br />
	 * @param id
	 * @param property
	 * @return the id given.
	 */
	AtomicNumber RecordPropertyOf(
		AtomicNumber id,
		const Property& property
	);

	/**
	 * Add a Property to the given type's record in *this. <br />
	 * @param name
	 * @param property
	 * @return the id of the given name.
	 */
	AtomicNumber RecordPropertyOf(
		const Name& name,
		const Property& property
	);

	/**
	 * Add a Property to the given type's record in *this. <br />
	 * Strips any qualifiers (*, &, or const) from T <br />
	 * @tparam T
	 * @param property
	 * @return the id of the given typo.
	 */
	template < typename T >
	AtomicNumber RecordPropertyOf(const Property& property)
	{
		return RecordPropertyOf(
			type::NakedTypeName< T >(),
			property
		);
	}

	/**
	 * Add Properties to the given type's record in *this. <br />
	 * @param id
	 * @param properties
	 * @return the given id.
	 */
	AtomicNumber RecordPropertiesOf(
		AtomicNumber id,
		const Properties& properties
	);

	/**
	 * Add Properties to the given type's record in *this. <br />
	 * @param name
	 * @param properties
	 * @return the id of the given name.
	 */
	AtomicNumber RecordPropertiesOf(
		const Name& name,
		const Properties& properties
	);

	/**
	 * Add Properties to the given type's record in *this. <br />
	 * @tparam T
	 * @param properties
	 * @return the id of the given type.
	 */
	template < typename T >
	AtomicNumber RecordPropertiesOf(const Properties& properties)
	{		
		return RecordPropertiesOf(
			type::NakedTypeName< T >(),
			properties
		);
	}

	/**
	 * Associates the given Wave type with the given id. <br />
	 * This is only necessary if you want to use GetTypeFromId later on. <br />
	 * @tparam T
	 * @param type
	 * @return true if the association completed successfully else false
	 */
	template < typename T >
	bool AssociateType(Wave* type)
	{
		AssociateType(GetIdFromType< T >(), type)
	}

	/**
	 * Removes the type association created by AssociateType(). <br />
	 * @tparam T
	 * @return true if the association was removed else false.
	 */
	template < typename T >
	bool DisassociateType()
	{
		DisassociateType(GetIdFromType< T >())
	}

	/**
	 * Get a previously AssociatedType. <br />
	 * Requires that T be a child of physical::Class<>. <br />
	 * It is the caller's responsibility to know if T should be wrapped by Quantum<>; see how Atom handles AsBonded() & AsBondedQuantum() for an example. <br />
	 * @tparam T
	 * @return a new T* from that stored in *this or NULL.
	 */
	template < typename T >
	const T* GetInstance() const
	{
		BIO_STATIC_ASSERT(type::IsWave< T >())
		const physical::Wave* storedInstance = GetTypeFromId(GetIdWithoutCreation(GetNameFromType< T >()));
		BIO_SANITIZE(storedInstance, , return NULL)

		const physical::Class< T >* type = ForceCast< const physical::Class< T >* >(storedInstance);
		return type->GetWaveObject()->Clone();
	}

protected:
	/**
	 * Create an Element instead of any other kind of Brane. <br />
	 * @param id
	 * @param name
	 * @return a new Element.
	 */
	virtual Brane* CreateBrane(AtomicNumber id, const Name& name)
	{
		return new Element(id, name);
	}
};

BIO_SINGLETON(PeriodicTable, PeriodicTableImplementation)

} //chemical namespace
} //bio namespace
