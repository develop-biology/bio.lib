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

#include "bio/physical/macro/Macros.h"
#include "bio/common/Types.h"
#include "bio/common/string/String.h"
#include "bio/common/thread/ThreadSafe.h"
#include "bio/common/Cast.h"
#include "bio/physical/string/Brane.h"
#include <sstream>
#include <cstring>

//@formatter:off
#if BIO_CPP_VERSION < 11
	#include <stdint.h>
#else
	#include <cstdint>
#endif
//@formatter:on

namespace bio {
namespace physical {

/**
 * A Perspective keeps track of Names and Ids for a certain set of objects within a DIMENSION and ensures a unique DIMENSION <-> Name pairing for all objects it "observes". <br />
 * A DIMENSION is a numeric space in which objects may be defined. The size of the space determines how many objects may be observed (i.e. kept track of) by a single Perspective. <br />
 * <br />
 * There should only ever be 1 perspective for each DIMENSION. <br />
 * <br />
 * There is little harm in having 2 different classes share the same name, though it should be avoided whenever possible. For example "myThing" can be a Molecule, a Cell, a Neuron, etc. <br />
 * If 2 objects do share a name and a DIMENSION, they will have the same DIMENSION and may be falsely considered equal. A more proper comparison would be if the 2 Waves Resonate() or if the 2 objects share a type. However, cases where 2 objects have the same DIMENSION and are of different types should be almost, if not entirely, avoided by system design (i.e. don't put squares into circular holes, even if they're both shapes).
 * <br />
 * Only objects that share a DIMENSION may be derived from, combined, etc. You cannot have a child class that derives from 2 different Identifiable templates (without solving diamond inheritance and some other caveats). <br />
 * Thus, functionally, you can think of each DIMENSION as a different library, with its source code hidden, such that only objects within that library, that DIMENSION, may inherit from each other. <br />
 * An example DIMENSION would be uint32_t, with up to 4,294,967,295 unique object names. <br />
 * <br />
 * See below for a macro for creating singleton of Perspectives. <br />
 * @tparam DIMENSION an unsigned integer (e.g. uint8_t).
 */
template < typename DIMENSION >
class Perspective :
	virtual public ThreadSafe
{
public:
	/**
	 *
	 */
	Perspective()
		:
		mNextId(1)
	{
		mBranes = new Arrangement< Brane< DIMENSION >* >();
	}

	/**
	 *
	 */
	virtual ~Perspective()
	{
		Brane< DIMENSION >* brane;
		for (
			SmartIterator brn = mBranes->Begin();
			!brn.IsAfterEnd();
			++brn
			)
		{
			brane = brn;
			if (brane)
			{
				delete brane;
				brane = NULL;
			}
		}
		delete mBranes;
		mBranes = NULL;
	}

	/**
	 * When overloading other methods of *this, make sure to check your inputs for invalid Ids. See the functions below for examples. <br />
	 * @return 0.
	 */
	static DIMENSION InvalidId()
	{
		return 0;
	}

	/**
	 * When overloading other methods of *this, make sure to check your inputs for invalid Names. See the functions below for examples. <br />
	 * @return "INVALID_NAME".
	 */
	static Name InvalidName()
	{
		return "INVALID_NAME";
	}

	/**
	 * Gives an iterator fos the given id. <br />
	 * @param id
	 * @return a SmartIterator pointing to the Brane desired, if it IsValid.
	 */
	SmartIterator Find(const DIMENSION& id)
	{
		SmartIterator brn = mBranes->Begin();
		for (
			; !brn.IsAfterEnd();
			++brn
			)
		{
			if (brn.As< Brane< DIMENSION >* >()->mId == id)
			{
				return brn;
			}
		}
		brn.Invalidate();
		return brn;
	}

	/**
	 * Gives an iterator fos the given id. <br />
	 * @param id
	 * @return a SmartIterator pointing to the Brane desired, if it IsValid.
	 */
	SmartIterator Find(const DIMENSION& id) const
	{
		SmartIterator brn = mBranes->Begin();
		for (
			; !brn.IsAfterEnd();
			++brn
			)
		{
			if (brn.As< Brane< DIMENSION >* >()->mId == id)
			{
				return brn;
			}
		}
		brn.Invalidate();
		return brn;
	}


	/**
	 * This will create a new DIMENSION for the given name if one does not exist. <br />
	 * @param name
	 * @return the DIMENSION associated with the given name
	 */
	virtual DIMENSION GetIdFromName(const Name& name)
	{
		if (name == InvalidName())
		{
			return InvalidId();
		}

		DIMENSION ret = GetIdWithoutCreation(name);
		if (ret)
		{
			return ret;
		}

		ret = mNextId++;
		mBranes->Add(CreateBrane(ret, name));

		return ret;
	}


	/**
	 * This requires that the DIMENSION has been previously associated with the name, perhaps from a call to GetIdFromName. <br />
	 * @param id
	 * @return the Name associated with the given DIMENSION
	 */
	virtual Name GetNameFromId(const DIMENSION& id) const
	{
		if (id == InvalidId())
		{
			return InvalidName();
		}

		SmartIterator result = Find(id);
		if (!result.IsValid())
		{
			return InvalidName();
		}
		return result.As< Brane< DIMENSION >* >()->mName;
	}


	/**
	 * There can be up to 256 additional names. <br />
	 * @param name
	 * @return a new DIMENSION for the given Name. However, the Name associated with the returned DIMENSION may not be the one provided. For example, consider: GetNameFromId(GetUniqueIdFor("MyName")); //Returns "MyName" GetNameFromId(GetUniqueIdFor("MyName")); //Returns "MyName_1"
	 */
	virtual DIMENSION GetUniqueIdFor(const Name& name)
	{
		if (name == InvalidName())
		{
			return InvalidId();
		}

		std::ostringstream usedName;
		usedName.str("");
		usedName << name;

		DIMENSION ret = GetIdWithoutCreation(usedName.str().c_str());

		uint8_t nameCount = 0;
		while (!ret)
		{
			usedName.clear();
			usedName.str("");
			usedName << name;
			usedName << "_" << nameCount++;
			ret = GetIdWithoutCreation(usedName.str().c_str());
		}

		//this creates the unique id.
		return GetIdFromName(usedName.str());
	}


	/**
	 * the same as GetIdFromName but will RETURN 0 instead of making a new association, if name is not found. <br />
	 * @param name
	 * @return the DIMENSION associated with name else InvalidId().
	 */
	virtual DIMENSION GetIdWithoutCreation(const Name& name) const
	{
		if (name == InvalidName())
		{
			return InvalidId();
		}

		Brane< DIMENSION >* brane;
		SmartIterator brn = mBranes->Begin();
		for (
			; !brn.IsAfterEnd();
			++brn
			)
		{
			brane = brn;

			if (name == brane->mName)
			{
				return brane->mId;
			}
		}
		return InvalidId();
	}

	/**
	 * @return the number of ids stored in *this.
	 */
	virtual DIMENSION GetNumUsedIds() const
	{
		return this->mNextId - 1;
	}


protected:

	/**
	 * Instead of making Brane a template parameter to *this, we provide this virtual Create method to allow for the creation of custom Branes. <br />
	 * @param id
	 * @param name
	 * @return a new Brane.
	 */
	virtual Brane< DIMENSION >* CreateBrane(DIMENSION id, const Name& name)
	{
		return new Brane(id, name);
	}

	/**
	 * Finds a Brane of the given id and casts it as a T*. <br />
	 * Use this to get the children of Brane created by overrides of CreateBrane. <br />
	 * @tparam T
	 * @param id
	 * @return a typename Brane< DIMENSION >* of the given id as a T* or NULL.
	 */
	template< class T >
	T GetBraneAs(DIMENSION id)
	{
		BIO_STATIC_ASSERT(type::IsPointer< T >())
		BIO_SANITIZE(id,,return NULL)
		SmartIterator brn = Find(id);
		BIO_SANITIZE(brn.IsValid(),,return NULL)
		return Cast< T >(brn.As< Brane< DIMENSION >* >());
	}

	/**
	 * Finds a Brane of the given id and casts it as a T*. <br />
	 * Use this to get the children of Brane created by overrides of CreateBrane. <br />
	 * @tparam T
	 * @param id
	 * @return a typename Brane< DIMENSION >* of the given id as a T* or NULL.
	 */
	template< class T >
	const T GetBraneAs(DIMENSION id) const
	{
		BIO_STATIC_ASSERT(type::IsPointer< T >())
		BIO_SANITIZE(id,,return NULL)
		SmartIterator brn = Find(id);
		BIO_SANITIZE(brn.IsValid(),,return NULL)
		return Cast< T >(brn.As< Brane< DIMENSION >* >());
	}

	mutable Container* mBranes;
	DIMENSION mNextId;
};

} //physical namespace
} //bio namespace
