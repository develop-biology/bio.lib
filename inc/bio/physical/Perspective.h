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

#include "bio/physical/macros/Macros.h"
#include "bio/common/Types.h"
#include "bio/common/string/String.h"
#include "bio/common/thread/ThreadSafe.h"
#include "bio/common/Cast.h"
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

class Wave;

/**
 * Wave is incomplete here due to circular inheritance. <br />
 * To provide the complete type, we provide this Utilities class which is defined in a non-templated compilation unit. <br />
 */
struct PerspectiveUtilities
{
	static Wave* Clone(const Wave* toClone);

	static void Delete(Wave* toDelete);
};

/**
 * A Perspective keeps track of Names and Ids for a certain set of objects within a DIMENSION and ensures a unique Id <-> Name pairing for all objects it "observes". <br />
 * A DIMENSION is a numeric space in which objects may be defined. The size of the space determines how many objects may be "observed" (i.e. kept track of) by a single Perspective. <br />
 * Together, a Perspective and DIMENSION can be thought to define the "size" of the "lens" used to "observe" "objects", if you'll permit the physical analogy of what is purely imaginative. <br />
 * Only objects that share a DIMENSION may be derived from, combined, etc. You cannot have a child class that derives from 2 different Identifiable templates (without solving diamond inheritance and some other caveats). <br />
 * Thus, functionally, you can think of each DIMENSION as a different library, with its source code hidden, such that only objects within that library, that DIMENSION, may inherit from each other. <br />
 * An example DIMENSION would be uint32_t, with up to 4,294,967,295 unique object names per perspective. <br />
 *
 * An example use case can be seen with Neurons and Synapses. Both are distinct objects and both can be tracked through different Perspectives. This means a Neuron of Id 1 can have the name "MyNeuron" and a Synapse of Id 1 can have the name "MySynapse". <br />
 * However, Neurons and Synapses share a lot of code and should exist within the same DIMENSION (e.g. in case you wanted to make some strange Neuron/Synapse hybrid). If your DIMENSION is a uint8_t, you could have 255 Neurons and 255 Synapses using a different Perspective for each. Using a single Respective, you could only have 255 uniquely identified Neurons OR Connections, total. <br />
 * Therefore, you'd likely want multiple Perspectives and a much larger DIMENSION (uint32_t, for instance) in order to accommodate a more total objects. <br />
 * See below for a macro for creating singleton of Perspectives. <br />
 */
template < typename DIMENSION >
class Perspective :
	virtual public ThreadSafe
{
public:
	typedef DIMENSION Id;
	typedef Arrangement< Id > Ids;

	/**
	 * What a single "point" in space contains. <br />
	 * Branes are taken from super string theory and are the multi-dimensional abstraction of membranes. <br />
	 */
	class Brane
	{
	public:
		Brane(
			Id id,
			const Name& name,
			Wave* type
		)
			:
			mId(id),
			mName(name),
			mType(type)
		{
		}

		Id mId;
		Name mName;
		Wave* mType;
	};

	typedef Arrangement< Brane* > Branes;

	/**
	 *
	 */
	Perspective()
		:
		mNextId(1)
	{
	}

	/**
	 *
	 */
	virtual ~Perspective()
	{
		Brane* brane;
		for (
			SmartIterator brn = mBranes.Begin();
			!brn.IsAfterEnd();
			++brn
			)
		{
			brane = brn;
			if (brane->mType)
			{
				PerspectiveUtilities::Delete(brane->mType);
				brane->mType = NULL;
			}
			delete brane;
			brane = NULL;
		}
		mBranes.Clear();
	}

	/**
	 * When overloading other methods of *this, make sure to check your inputs for invalid Ids. See the functions below for examples. <br />
	 * @return 0.
	 */
	static Id InvalidId()
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
	SmartIterator Find(const Id& id)
	{
		SmartIterator brn = mBranes.Begin();
		for (
			; !brn.IsAfterEnd();
			++brn
			)
		{
			if (brn.As< Brane* >()->mId == id)
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
	SmartIterator Find(const Id& id) const
	{
		SmartIterator brn = mBranes.Begin();
		for (
			; !brn.IsAfterEnd();
			++brn
			)
		{
			if (brn.As< Brane* >()->mId == id)
			{
				return brn;
			}
		}
		brn.Invalidate();
		return brn;
	}


	/**
	 * This will create a new Id for the given name if one does not exist. <br />
	 * @param name
	 * @return the Id associated with the given name
	 */
	virtual Id GetIdFromName(const Name& name)
	{
		if (name == InvalidName())
		{
			return InvalidId();
		}

		Id ret = GetIdWithoutCreation(name);
		if (ret)
		{
			return ret;
		}

		ret = mNextId++;
		mBranes.Add(
			new Brane(
				ret,
				name,
				NULL
			));

		return ret;
	}


	/**
	 * This requires that the Id has been previously associated with the name, perhaps from a call to GetIdFromName. <br />
	 * @param id
	 * @return the Name associated with the given Id
	 */
	virtual Name GetNameFromId(const Id& id) const
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
		return result.As< Brane* >()->mName;
	}


	/**
	 * There can be up to 256 additional names. <br />
	 * @param name
	 * @return a new Id for the given Name. However, the Name associated with the returned Id may not be the one provided. For example, consider: GetNameFromId(GetUniqueIdFor("MyName")); //Returns "MyName" GetNameFromId(GetUniqueIdFor("MyName")); //Returns "MyName_1"
	 */
	virtual Id GetUniqueIdFor(const Name& name)
	{
		if (name == InvalidName())
		{
			return InvalidId();
		}

		std::ostringstream usedName;
		usedName.str("");
		usedName << name;

		Id ret = GetIdWithoutCreation(usedName.str().c_str());

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
	 * @return the Id associated with name else InvalidId().
	 */
	virtual Id GetIdWithoutCreation(const Name& name) const
	{
		if (name == InvalidName())
		{
			return InvalidId();
		}

		Brane* brane;
		SmartIterator brn = mBranes.Begin();
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
	virtual Id GetNumUsedIds() const
	{
		return this->mNextId - 1;
	}


	/**
	 * Associates the given Wave type with the given id. <br />
	 * This is only necessary if you want to use GetTypeFromId later on. <br />
	 * NOTE: There is no GetIdFromType(). For that behavior, see chemical::PeriodicTable and chemical::Atom. <br />
	 * @param id
	 * @param type
	 * @return true if the association completed successfully else false
	 */
	virtual bool AssociateType(
		Id id,
		Wave* type
	)
	{
		SmartIterator brn = Find(id);
		if (!brn.IsValid())
		{
			return false;
		}
		Brane* brane = brn;

		BIO_SANITIZE(type,
			brane->mType = PerspectiveUtilities::Clone(type),
			brane->mType = type)

		return true;
	}

	/**
	 * Removes the type association created by AssociateType(). <br />
	 * @param id
	 * @return true if the association was removed else false.
	 */
	virtual bool DisassociateType(const Id& id)
	{
		SmartIterator brn = Find(id);
		if (!brn.IsValid())
		{
			return false;
		}

		Brane* brane = brn;

		BIO_SANITIZE_AT_SAFETY_LEVEL_1(brane->mType,
			PerspectiveUtilities::Delete(brane->mType),
		)
		brane->mType = NULL;

		return true;
	}


	/**
	 * Only works if AssociateType has been called with the given id. <br />
	 * @param id
	 * @return the pointer to the Wave type associated with the given id else NULL.
	 */
	virtual const Wave* GetTypeFromId(const Id& id) const
	{
		BIO_SANITIZE(id == InvalidId(),
			,
			return NULL
		)

		SmartIterator result = Find(id);
		if (!result.IsValid())
		{
			return NULL;
		}
		return result.As< Brane* >()->mType;
	}

	/**
	 * Only works if AssociateType has been called with the given id. <br />
	 * @param name
	 * @return the pointer to the Wave type associated with the given id else NULL.
	 */
	virtual const Wave* GetTypeFromName(const Name& name) const
	{
		return GetTypeFromId(GetIdWithoutCreation(name));
	}

	/**
	 * Creates a new object by Clone()ing the associated type. <br />
	 * @param id
	 * @return a Clone() of the Wave* associated with the given id else NULL.
	 */
	virtual Wave* GetNewObjectFromId(const Id& id) const
	{
		const Wave* ret = GetTypeFromId(id);
		if (ret)
		{
			return PerspectiveUtilities::Clone(ret);
		}
		return NULL;
	}

	/**
	 * Creates a new object by Clone()ing the associated type. <br />
	 * @param name
	 * @return a Clone() of the Wave* associated with the given name else NULL.
	 */
	virtual Wave* GetNewObjectFromName(const Name& name)
	{
		return this->GetNewObjectFromId(this->GetIdFromName(name));
	}

	/**
	 * Ease of access method for casting the result of GetTypeFromId(). <br />
	 * @tparam T
	 * @param id
	 * @return a T* associated with the given name id NULL.
	 */
	template < typename T >
	const T GetTypeFromIdAs(const Id& id) const
	{
		BIO_SANITIZE_WITH_CACHE(GetTypeFromId(id),
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
		BIO_SANITIZE_WITH_CACHE(GetTypeFromName(name),
			BIO_SINGLE_ARG(return ForceCast< T, const Wave* >(RESULT)),
			return NULL
		)
	}

	/**
	 * Ease of use method for casting the result of GetNewObjectFromId() <br />
	 * @tparam T
	 * @param id
	 * @return a new T* from Clone()ing the type associated with the given id else NULL.
	 */
	template < typename T >
	T GetNewObjectFromIdAs(const Id& id)
	{
		BIO_SANITIZE_WITH_CACHE(GetNewObjectFromId(id),
			BIO_SINGLE_ARG(return ForceCast< T, Wave* >(RESULT)),
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
		BIO_SANITIZE_WITH_CACHE(GetNewObjectFromName(name),
			BIO_SINGLE_ARG(return ForceCast< T, Wave* >(RESULT)),
			return NULL
		)
	}


protected:
	Branes mBranes;
	Id mNextId;
};

} //physical namespace
} //bio namespace
