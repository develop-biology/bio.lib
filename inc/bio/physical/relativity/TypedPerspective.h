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

#include "Perspective.h"
#include "bio/physical/common/Class.h"
#include "bio/physical/string/TypedBrane.h"
#include "bio/physical/wave/Wave.h"
#include "bio/physical/type/IsWave.h"

namespace bio {
namespace physical {

/**
 * A TypedPerspective extends Perspective by adding Wave*s to its Branes. <br />
 * This allows you to (Dis)AssociateType(...) and GetNewObject...(...) from Names and Ids. <br />
 * @tparam DIMENSION an unsigned integer (e.g. uint8_t).
 */
template < typename DIMENSION >
class TypedPerspective :
	public Perspective< DIMENSION >
{
public:
	/**
	 *
	 */
	TypedPerspective()
	{

	}

	/**
	 *
	 */
	virtual ~TypedPerspective()
	{

	}

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
		const DIMENSION& id,
		physical::Wave* type
	)
	{
		BIO_SANITIZE(type, , return false)
		TypedBrane< DIMENSION >* brane = this->template GetBraneAs< TypedBrane< DIMENSION >* >(id);
		BIO_SANITIZE(brane, , return false)
		// BIO_SANITIZE_AT_SAFETY_LEVEL_1(brane->mType, , return false) //it's okay if mType is NULL
		brane->mType = type;
		return true;
	}

	/**
	 * Removes the type association created by AssociateType() and deletes the Associated Wave. <br />
	 * Disassociating a type has no effect on the Recorded Properties. <br />
	 * @param id
	 * @return true if the association was removed else false.
	 */
	virtual bool DisassociateType(const DIMENSION& id)
	{
		TypedBrane< DIMENSION >* brane = this->template GetBraneAs< TypedBrane< DIMENSION >* >(id);
		BIO_SANITIZE(brane, , return false)
		if (brane->mType)
		{
			delete brane->mType;
			brane->mType = NULL;
		}
		return true;
	}

	/**
	 * Only works if AssociateType has been called with the given id. <br />
	 * @param id
	 * @return the pointer to the Wave type associated with the given id else NULL.
	 */
	virtual const Wave* GetTypeFromId(const DIMENSION& id) const
	{
		TypedBrane< DIMENSION >* brane = this->template GetBraneAs< TypedBrane< DIMENSION >* >(id);
		BIO_SANITIZE(brane, , return NULL)
		return brane->mType;
	}

	/**
	 * Only works if AssociateType has been called with the given id. <br />
	 * @param name
	 * @return the pointer to the Wave type associated with the given id else NULL.
	 */
	virtual const Wave* GetTypeFromName(const Name& name) const
	{
		return this->GetTypeFromId(this->GetIdWithoutCreation(name));
	}

	/**
	 * Ease of access method for casting the result of GetTypeFromId(). <br />
	 * @tparam T
	 * @param id
	 * @return a T* associated with the given name id NULL.
	 */
	template < typename T >
	const T GetTypeFromIdAs(const DIMENSION& id) const
	{
		BIO_STATIC_ASSERT(type::IsPointer< T >())
		BIO_SANITIZE_WITH_CACHE(this->GetTypeFromId(id),
			BIO_SINGLE_ARG(return ForceCast< T, const Wave* >(RESULT)),
			return NULL
		)
	}

	/**
	 * Ease of access method for casting the result of GetTypeFromId(). <br />
	 * @tparam T
	 * @param name
	 * @return a T* associated with the given name id NULL.
	 */
	template < typename T >
	const T GetTypeFromNameAs(const Name& name) const
	{
		BIO_STATIC_ASSERT(type::IsPointer< T >())
		BIO_SANITIZE_WITH_CACHE(this->GetTypeFromName(name),
			BIO_SINGLE_ARG(return ForceCast< T, const Wave* >(RESULT)),
			return NULL
		)
	}

	/**
	 * Creates a new object by Clone()ing the associated type. <br />
	 * @param id
	 * @return a Clone() of the Wave* associated with the given id else NULL.
	 */
	virtual Wave* GetNewObjectFromId(const DIMENSION& id) const
	{
		const Wave* type = this->GetTypeFromId(id);
		BIO_SANITIZE(type, , return NULL)
		return type->Clone();
	}

	/**
	 * Creates a new object by Clone()ing the associated type. <br />
	 * @param name
	 * @return a Clone() of the Wave* associated with the given name else NULL.
	 */
	virtual Wave* GetNewObjectFromName(const Name& name)
	{
		const Wave* type = this->GetTypeFromName(name);
		BIO_SANITIZE(type, , return NULL)
		return type->Clone();
	}

	/**
	 * Ease of use method for casting the result of GetNewObjectFromId() <br />
	 * @tparam T
	 * @param id
	 * @return a new T* from Clone()ing the type associated with the given id else NULL.
	 */
	template < typename T >
	T GetNewObjectFromIdAs(const DIMENSION& id)
	{
		BIO_STATIC_ASSERT(type::IsPointer< T >())
		BIO_STATIC_ASSERT(type::IsWave< T >())
		BIO_SANITIZE_WITH_CACHE(this->GetNewObjectFromId(id),
			BIO_SINGLE_ARG(
				return *(ForceCast< physical::Class< T >*, Wave* >(RESULT)->GetWaveObject()) //GetWaveObject gives T*, which, if T is a pointer (as it must be), is T**.
			),
			return NULL
		)
	}

	/**
	 * Ease of access method for casting the result of GetNewObjectFromName() <br />
	 * @tparam T
	 * @param name
	 * @return a new T* from Clone()ing the type associated with the given name else NULL.
	 */
	template < typename T >
	T GetNewObjectFromNameAs(const Name& name)
	{
		return this->GetNewObjectFromIdAs< T >(this->GetIdWithoutCreation(name));
	}


protected:
	
	/**
	 * Create a TypedBrane instead of a standard Brane. <br />
	 * @param id 
	 * @param name 
	 * @return a TypedBrane.
	 */
	virtual TypedBrane< DIMENSION >* CreateBrane(DIMENSION id, const Name& name)
	{
		return new TypedBrane< DIMENSION >(id, name, NULL);
	}
};

} //physical namespace
} //bio namespace
