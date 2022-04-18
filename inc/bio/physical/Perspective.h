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
#include "bio/common/String.h"
#include "bio/common/ThreadSafe.h"
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
	typedef std::vector< Id > Ids;

	/**
	 * What a single point in space contains. <br />
	 * Dimensions are Nuit ∴ ∴ <br />
	 */
	class Hadit
	{
	public:
		Hadit(
			Id id,
			Name name,
			Wave* type 
		)
			:
			m_id(id),
			m_name(name),
			m_type(type)
		{
		}

		Id m_id;
		Name m_name;
		Wave* m_type; 
	};

	typedef std::vector< Hadit > Hadits;

	/**
	 *
	 */
	Perspective()
		:
		m_nextId(1)
	{
	}

	/**
	 *
	 */
	virtual ~Perspective()
	{
		LockThread();
		for (
			typename Hadits::iterator itt = m_hadits.begin();
			itt != m_hadits.end();
			++itt
			)
		{
			if (itt->m_name)
			{
				delete[] itt->m_name;
				itt->m_name = NULL;
			}
			if (itt->m_type)
			{
				PerspectiveUtilities::Delete(itt->m_type);
				itt->m_type = NULL;
			}
		}
		m_hadits.clear();
		UnlockThread();
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
	 * @return a Hadits::iterator or m_hadits.end();
	 */
	typename Hadits::iterator Find(Id id)
	{

		LockThread();
		typename Hadits::iterator hdt = m_hadits.begin();
		for (
			; hdt != m_hadits.end();
			++hdt
			)
		{
			if (hdt->m_id == id)
			{
				UnlockThread();
				return hdt;
			}
		}
		UnlockThread();
		return hdt;
	}

	/**
	 * Gives an iterator fos the given id. <br />
	 * @param id
	 * @return a Hadits::const_iterator or m_hadits.end();
	 */
	typename Hadits::const_iterator Find(Id id) const
	{
		LockThread();
		typename Hadits::const_iterator hdt = m_hadits.begin();
		for (
			; hdt != m_hadits.end();
			++hdt
			)
		{
			if (hdt->m_id == id)
			{
				UnlockThread();
				return hdt;
			}
		}
		UnlockThread();
		return hdt;
	}


	/**
	 * This will create a new Id for the given name if one does not exist. <br />
	 * @param name
	 * @return the Id associated with the given name
	 */
	virtual Id GetIdFromName(Name name)
	{
		if (strcmp(
			InvalidName(),
			name
		))
		{
			return InvalidId();
		}

		Id ret = GetIdWithoutCreation(name);
		if (ret)
		{
			return ret;
		}

		Name usedName;
		string::CloneInto(
			name,
			usedName
		);

		LockThread();
		ret = m_nextId++;
		m_hadits.push_back(
			Hadit(
				ret,
				usedName,
				NULL
			));
		UnlockThread();

		return ret;
	}


	/**
	 * This requires that the Id has been previously associated with the name, perhaps from a call to GetIdFromName. <br />
	 * @param id
	 * @return the Name associated with the given Id
	 */
	virtual Name GetNameFromId(Id id) const
	{
		if (id == InvalidId())
		{
			return InvalidName();
		}

		typename Hadits::const_iterator result = Find(id);
		if (result == m_hadits.end())
		{
			return InvalidName();
		}
		return result->m_name;
	}


	/**
	 * There can be up to 256 additional names. <br />
	 * @param name
	 * @return a new Id for the given Name. However, the Name associated with the returned Id may not be the one provided. For example, consider: GetNameFromId(GetUniqueIdFor("MyName")); //Returns "MyName" GetNameFromId(GetUniqueIdFor("MyName")); //Returns "MyName_1"
	 */
	virtual Id GetUniqueIdFor(Name name)
	{
		if (strcmp(
			InvalidName(),
			name
		))
		{
			return InvalidId();
		}

		std::ostringstream usedName;
		usedName.str(""); //TODO: can we do this all in the ctor?
		usedName << name;

		Id ret = GetIdWithoutCreation(usedName.str().c_str());

		uint8_t nameCount = 0;
		while (!ret)
		{
			usedName.clear();
			usedName.str(""); //TODO: is this right?
			usedName << name;
			usedName << "_" << nameCount++;
			ret = GetIdWithoutCreation(usedName.str().c_str());
		}

		//this creates the unique id.
		return GetIdFromName(usedName.str().c_str());
	}


	/**
	 * the same as GetIdFromName but will RETURN 0 instead of making a new association, if name is not found. <br />
	 * @param name
	 * @return the Id associated with name else InvalidId().
	 */
	virtual Id GetIdWithoutCreation(Name name) const
	{
		if (strcmp(
			InvalidName(),
			name
		))
		{
			return InvalidId();
		}

		typename Hadits::const_iterator itt = m_hadits.begin();
		for (
			; itt != m_hadits.end();
			++itt
			)
		{
			if (!strcmp(
				itt->m_name,
				name
			))
			{
				return itt->m_id;
			}
		}
		return InvalidId();
	}

	/**
	 * @return the number of ids stored in *this.
	 */
	virtual Id GetNumUsedIds() const
	{
		return this->m_nextId - 1;
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
		typename Hadits::iterator hdt = Find(id);
		if (hdt == m_hadits.end() || hdt->m_type)
		{
			return false;
		}

		LockThread();
		BIO_SANITIZE(type,
			hdt->m_type = PerspectiveUtilities::Clone(type),
			hdt->m_type = type);
		UnlockThread();

		return true;
	}

	/**
	 * Removes the type association created by AssociateType(). <br />
	 * @param id
	 * @return true if the association was removed else false.
	 */
	virtual bool DisassociateType(Id id)
	{
		typename Hadits::iterator hdt = Find(id);
		if (hdt == m_hadits.end())
		{
			return false;
		}

		LockThread();
		BIO_SANITIZE_AT_SAFETY_LEVEL_2(hdt->m_type,
			PerspectiveUtilities::Delete(hdt->m_type),);
		hdt->m_type = NULL;
		UnlockThread();

		return true;
	}


	/**
	 * Only works if AssociateType has been called with the given id. <br />
	 * @param id
	 * @return the pointer to the Wave type associated with the given id else NULL.
	 */
	virtual const Wave* GetTypeFromId(Id id) const 
	{
		BIO_SANITIZE(id == InvalidId(), ,
			return NULL)

		typename Hadits::const_iterator result = Find(id);
		if (result == m_hadits.end())
		{
			return NULL;
		}
		return result->m_type;
	}

	/**
	 * Only works if AssociateType has been called with the given id. <br />
	 * @param name
	 * @return the pointer to the Wave type associated with the given id else NULL.
	 */
	virtual const Wave* GetTypeFromName(Name name) const 
	{
		return GetTypeFromId(GetIdWithoutCreation(name));
	}

	/**
	 * Creates a new object by Clone()ing the associated type. <br />
	 * @param id
	 * @return a Clone() of the Wave* associated with the given id else NULL.
	 */
	virtual Wave* GetNewObjectFromId(Id id) const 
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
	virtual Wave* GetNewObjectFromName(Name name) 
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
	const T GetTypeFromIdAs(Id id) const
	{
		BIO_SANITIZE_WITH_CACHE(GetTypeFromId(id),
			BIO_SINGLE_ARG(return ForceCast< T, const Wave* >(RESULT)), 
			return NULL);
	}

	/**
	 * Ease of access method for casting the result of GetTypeFromId(). <br />
	 * @tparam T
	 * @param name
	 * @return a T* associated with the given name id NULL.
	 */
	template < typename T >
	const T GetTypeFromNameAs(Name name) const
	{
		BIO_SANITIZE_WITH_CACHE(GetTypeFromName(name),
			BIO_SINGLE_ARG(return ForceCast< T, const Wave* >(RESULT)), 
			return NULL);
	}

	/**
	 * Ease of use method for casting the result of GetNewObjectFromId() <br />
	 * @tparam T
	 * @param id
	 * @return a new T* from Clone()ing the type associated with the given id else NULL.
	 */
	template < typename T >
	T GetNewObjectFromIdAs(Id id)
	{
		BIO_SANITIZE_WITH_CACHE(GetNewObjectFromId(id),
			BIO_SINGLE_ARG(return ForceCast< T, Wave* >(RESULT)), 
			return NULL);
	}

	/**
	 * Ease of access method for casting the result of GetNewObjectFromName() <br />
	 * @tparam T
	 * @param name
	 * @return a new T* from Clone()ing the type associated with the given name else NULL.
	 */
	template < typename T >
	T GetNewObjectFromNameAs(Name name)
	{
		BIO_SANITIZE_WITH_CACHE(GetNewObjectFromName(name),
			BIO_SINGLE_ARG(return ForceCast< T, Wave* >(RESULT)), 
			return NULL);
	}


protected:
	Hadits m_hadits;
	Id m_nextId;
};

} //physical namespace
} //bio namespace
