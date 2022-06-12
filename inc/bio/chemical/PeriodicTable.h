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

#include "bio/common/type/NakedTypeName.h"
#include "bio/chemical/common/Types.h"
#include "bio/physical/common/Class.h"
#include "bio/physical/type/IsWave.h"
#include "bio/physical/Perspective.h"


namespace bio {
namespace chemical {

/**
 * The PeriodicTable is a stand-in for runtime type information (RTTI). <br />
 * Instead of having to rely on code compiled with RTTI enabled, we implement our own type tracking mechanism. <br />
 *
 * The PeriodicTable tracks Properties of types in addition to assigning them an AtomicNumber.. <br />
 * This is done because neither static nor virtual methods will allow for the inverted inheritance necessary to make inverted-inheritance methods like Atom::CallForAll work properly. <br />
 * See Elementary.h for a more detailed description of this system. <br />
 */
class PeriodicTableImplementation :
	public physical::Perspective< AtomicNumber >
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
	const Properties GetPropertiesOf(AtomicNumber id) const;

	/**
	 * @param name
	 * @return whatever properties have been Recorded for the given type.
	 */
	const Properties GetPropertiesOf(const Name& name) const;

	/**
	 * @tparam T
	 * @return whatever properties have been Recorded for the given type.
	 */
	template < typename T >
	const Properties GetPropertiesOf() const
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
		Property property
	);

	/**
	 * Add a Property to the given type's record in *this. <br />
	 * @param name
	 * @param property
	 * @return the id of the given name.
	 */
	AtomicNumber RecordPropertyOf(
		const Name& name,
		Property property
	);

	/**
	 * Add a Property to the given type's record in *this. <br />
	 * Strips any qualifiers (*, &, or const) from T <br />
	 * @tparam T
	 * @param property
	 * @return the id of the given typo.
	 */
	template < typename T >
	AtomicNumber RecordPropertyOf(Property property)
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
		Properties properties
	);

	/**
	 * Add Properties to the given type's record in *this. <br />
	 * @param name
	 * @param properties
	 * @return the id of the given name.
	 */
	AtomicNumber RecordPropertiesOf(
		const Name& name,
		Properties properties
	);

	/**
	 * Add Properties to the given type's record in *this. <br />
	 * @tparam T
	 * @param properties
	 * @return the id of the given type.
	 */
	template < typename T >
	AtomicNumber RecordPropertiesOf(Properties properties)
	{		
		return RecordPropertiesOf(
			type::NakedTypeName< T >(),
			properties
		);
	}

	/**
	 * Only works if AssociateType has been called with the given id. <br />
	 * @param id
	 * @return the pointer to the Wave type associated with the given id else NULL.
	 */
	virtual const physical::Wave* GetTypeFromId(AtomicNumber id) const;

	/**
	 * Associates the given Wave type with the given id. <br />
	 * Nop if a type is already Associated. In that case, you must Disassociate the type before calling this method. <br />
	 * <br />
	 * NOTE: THE ASSOCIATED TYPE WILL BE DELETED BY *this AND SHOULD LAST THE LIFETIME OF THE PROGRAM! <br />
	 * In other words, don't delete whatever you provide here. <br />
	 * <br />
	 * This is only necessary if you want to use GetTypeFromId later on. <br />
	 * Associating a type with an id has no effect on the Recorded Properties. <br />
	 * You should only use this with classes that derive from physical::Class<>, as this is the only accepted means of retrieving the type later (see GetInstance, below). <br />
	 * @param id
	 * @param type
	 * @return true if the association completed successfully else false
	 */
	virtual bool AssociateType(
		AtomicNumber id,
		physical::Wave* type
	);

	/**
	 * Removes the type association created by AssociateType() and deletes the Associated Wave. <br />
	 * Disassociating a type has no effect on the Recorded Properties. <br />
	 * @param id
	 * @return true if the association was removed else false.
	 */
	virtual bool DisassociateType(AtomicNumber id);

	/**
	 * Get a previously AssociatedType. <br />
	 * Requires that T be a child of physical::Class<>. <br />
	 * It is the caller's responsibility to know if T should be wrapped by Quantum<>; see how Atom handles AsBonded() & AsBondedQuantum() for an example. <br />
	 * @tparam T
	 * @return a T* from that stored in *this or NULL.
	 */
	template < typename T >
	const T* GetInstance() const
	{
		BIO_STATIC_ASSERT(type::IsWave< T >())

		const physical::Wave* storedInstance = GetTypeFromId(GetIdWithoutCreation(GetNameFromType< T >()));
		BIO_SANITIZE(storedInstance, , return NULL)

		const physical::Class< T >* type = ForceCast< const physical::Class< T >* >(storedInstance);
		return type->GetWaveObject();
	}
};

BIO_SINGLETON(PeriodicTable, PeriodicTableImplementation)

} //chemical namespace
} //bio namespace
