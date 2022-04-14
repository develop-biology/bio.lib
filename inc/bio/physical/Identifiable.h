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

#include "bio/common/VirtualBase.h"
#include "bio/common/String.h"
#include "bio/common/macros/OSMacros.h"
#include "Perspective.h"
#include "Observer.h"
#include "Wave.h"
#include "bio/physical/common/Class.h"
#include <cstring>

namespace bio {
namespace physical {

/**
 * An Identifiable class has a name and a numeric identifier (Id).
 * Names are used for debugging and human interaction, while Ids are used for run-time processing.
 * Identifiable classes require a Perspective<DIMENSION> to maintain a mapping of Id <-> Name pairs.
 * The Id exists within a DIMENSION, i.e. an int type (almost always unsigned), like uint32_t.
 * For more on DIMENSIONs, see Perspective.h
 *
 *The reason this class is templated is so that less numerous classes can use a smaller ID type, which decreases memory footprint and increases processing speed (less bits to check).
 * Unfortunately, inheritance reveals a problem with this design: Identifiable classes cannot change their ID type.
 * For example, if you have one class that you expect a small number of and then derive from that class, expecting a larger number of children, you either must derive from Identifiable twice, indulging in diamond inheritance, or increase the size of Id to encompass all possible uses.
 * For this reason, the default DIMENSION (StandardDimension, from Types.h) should be used in nearly all cases, unless you want to ensure either your class is not derived from or that it remains separated from other code.
 * An example of using a non-StandardDimension can be found in Codes. Codes have their own DIMENSION, as they should not be inherited from but may still be expanded upon through user-defined values (simply additional name <-> id definitions).
*/
template < typename DIMENSION >
class Identifiable :
	virtual public Observer< Perspective< DIMENSION > >, //includes VirtualBase, so no need to re-inherit.
	public physical::Class< Identifiable< DIMENSION > >
{
public:
	typedef DIMENSION Id;
	typedef std::vector< Id > Ids;

	/**
	 * Ensure virtual methods point to Class implementations.
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(physical,
		Identifiable< DIMENSION >)


	/**
	 * @param perspective
	 */
	explicit Identifiable(Perspective< DIMENSION >* perspective = NULL)
		:
		physical::Class< Identifiable< DIMENSION > >(this),
		#if BIO_MEMORY_OPTIMIZE_LEVEL == 0
		m_name(NULL),
		#endif
		m_id(Perspective< DIMENSION >::InvalidId())
	{
		CloneIntoName(Perspective< DIMENSION >::InvalidName());
		if (perspective)
		{
			Observer< Perspective< DIMENSION > >::Initialize(perspective);
		}
	}

	/**
	 * @param name
	 * @param perspective
	 */
	explicit Identifiable(
		Name name,
		Perspective< DIMENSION >* perspective = NULL
	)
		:
		physical::Class< Identifiable< DIMENSION > >(this),
		#if BIO_MEMORY_OPTIMIZE_LEVEL == 0
		m_name(NULL),
		#endif
		m_id(Perspective< DIMENSION >::InvalidId())
	{
		CloneIntoName(name);
		if (perspective)
		{
			Observer< Perspective< DIMENSION > >::Initialize(perspective);
			m_id = this->GetPerspective()->GetIdFromName(m_name);
			this->MakeWave();
		}
		else
		{
			m_id = Perspective< DIMENSION >::InvalidId();
		}
	}

	/**
	 * @param id
	 * @param perspective
	 */
	explicit Identifiable(
		Id id,
		Perspective< DIMENSION >* perspective = NULL
	)
		:
		physical::Class< Identifiable< DIMENSION > >(this),
		#if BIO_MEMORY_OPTIMIZE_LEVEL == 0
		m_name(NULL),
		#endif
		m_id(Perspective< DIMENSION >::InvalidId())
	{
		if (perspective)
		{
			Observer< Perspective< DIMENSION > >::Initialize(perspective);
			CloneIntoName(perspective->GetNameFromId(id));
			this->MakeWave();
		}
		else
		{
			m_id = Perspective< DIMENSION >::InvalidId();
			CloneIntoName(Perspective< DIMENSION >::InvalidName());
		}
	}

	/**
	 * @param other
	 */
	Identifiable(const Identifiable& other)
		:
		physical::Class< Identifiable< DIMENSION > >(this),
		#if BIO_MEMORY_OPTIMIZE_LEVEL == 0
		m_name(NULL),
		#endif
		m_id(other.m_id)
	{
		Observer< Perspective< DIMENSION > >::Initialize(other.GetPerspective());
		CloneIntoName(other.GetName());
	}

	/**
	 *
	 */
	virtual ~Identifiable()
	{
		#if BIO_MEMORY_OPTIMIZE_LEVEL == 0
		delete[] m_name;
		#endif
	}

	/**
	 * @return *this as its Id.
	 */
	virtual operator DIMENSION() const
	{
		return m_id;
	}

	/**
	 * @param id
	 * @return whether or not the id of *this matches id provided and double checks with the Perspective used by *this.
	 */
	virtual bool operator==(const Id id) const
	{
		if (!this->GetId())
		{
			return false;
		}
		if (!this->GetId() == id.m_t)
		{
			return false;
		}
		if (this->GetPerspective() && !this->IsNameInsensitive(this->GetPerspective()->GetNameFromId(id)))
		{
			return false;
		}
		return true;
	}

	/**
	 *
	 * @param name
	 * @return whether or not the given name matches that of *this and double checks with the Perspective used by *this.
	 */
	virtual bool operator==(Name name) const
	{
		if (!GetName())
		{
			if (!name)
			{
				return true;
			}
			return false;
		}
		if (!name)
		{
			return false;
		}
		if (this->GetPerspective() && !this->IsId(this->GetPerspective()->GetIdWithoutCreation(name)))
		{
			return false;
		}
		return true;
	}

	/**
	 * @param other
	 * @return whether or not the Ids of other and *this match and were given by the same Perspective.
	 */
	virtual bool operator==(const Identifiable< DIMENSION >& other) const
	{
		return this->IsId(other.GetId()) && this->GetPerspective() == other.GetPerspective();
	}

	/**
	 * @return the name of *this.
	 */
	virtual Name GetName() const
	{
		#if BIO_MEMORY_OPTIMIZE_LEVEL == 0
		return m_name;
		#else
		return BIO_SANITIZE_WITH_CACHE(this->GetPerspective(), RESULT->GetNameFromId(m_id), return "INVALID_NAME");
		#endif
	}

	/**
	 * @return the id of *this.
	 */
	virtual Id GetId() const
	{
		return m_id;
	}

	/**
	 * Sets the name and updates the type to the given name.
	 * Has no effect if perspective is null.
	 * @param name
	 */
	virtual void SetName(Name name)
	{
		//TODO: should this check be after setting the name?
		//	if so, the constructors can be simplified.
		if (!this->GetPerspective())
		{
			return;
		}
		CloneIntoName(name);

		m_id = this->GetPerspective()->GetIdFromName(m_name);
	}

	/**
	 * Sets the id and updates the name to the given id.
	 * Has no effect if perspective is null.
	 * @param id
	 */
	virtual void SetId(Id id)
	{
		if (!this->GetPerspective())
		{
			return;
		}
		m_id = id;

		CloneIntoName(this->GetPerspective()->GetNameFromId(m_id));
	}

	/**
	 * This is preferred to GetName() == ... because == may not exist for the Name type and/or the Name type may change in a later release.
	 * Case Sensitive.
	 * @param name
	 * @return a comparison operation on the given name with the name of *this.
	 */
	virtual bool IsName(Name name) const
	{
		return !strcmp(
			name,
			this->GetName());
	}

	/**
	 * @param name
	 * @return whether or not the given name is offensive. JK, it's just a case insensitive version of IsName.
	 */
	virtual bool IsNameInsensitive(Name name) const
	{
		return !strcasecmp(
			name,
			this->GetName());
	}

	/**
	 * @param id
	 * @return whether or not the given id matches that of *this.
	 */
	virtual bool IsId(Id id) const
	{
		return id == m_id;
	}

	/**
	 * Sets the perspective for *this.
	 * @param perspective
	 */
	virtual void SetPerspective(Perspective< DIMENSION >* perspective)
	{
		this->SetPerspective(perspective);

		if (IsName(Perspective< DIMENSION >::InvalidName()) && !IsId(Perspective< DIMENSION >::InvalidId()))
		{
			CloneIntoName(this->GetPerspective()->GetNameFromId(m_id));
		}
		else if (!IsName(Perspective< DIMENSION >::InvalidName()) && IsId(Perspective< DIMENSION >::InvalidId()))
		{
			m_id = this->GetPerspective()->GetIdFromName(GetName());
		}
	}

	/**
	 * Records *this as the archetypal Wave for the id of *this.
	 * @param force
	 */
	virtual void MakeWave(bool force = false)
	{
		if (!this->GetPerspective())
		{
			return;
		}
		if (force)
		{
			this->GetPerspective()->DisassociateType(m_id);
		}
		this->GetPerspective()->AssociateType(
			m_id,
			this
		);
	}

	#if 0
	//No need to compile these nops.

	/**
	 * Required method from Wave. See that class for details.
	 * Because Symmetry is Identifiable, it cannot be #included here. We rely on children of Identifiable (primarily chemical:: objects to implement Spin()).
	 * @return a Symmetrical image of *this
	 */
	virtual Symmetry* Spin() const
	{
		return NULL;
	}

	/**
	 * Required method from Wave. See that class for details.
	 * Reconstruct *this from the given Symmetry.
	 * Because Symmetry is Identifiable, it cannot be #included here. We rely on children of Identifiable (primarily chemical:: objects to implement Reify()).
	 * @param symmetry
	 */
	virtual Code Reify(Symmetry* symmetry)
	{
		//nop
	}
	#endif

protected:
	/**
	 * VirtualBase required method. See that class for details (in common/)
	 * @param args
	 */
	virtual void InitializeImplementation(ByteStreams args)
	{
		if (args.size() == 2)
		{
			if (args[1].Is< Perspective< DIMENSION >* >())
			{
				Observer< Perspective< DIMENSION > >::Initialize(args[1]);
			}
			args.pop_back();
		}
		if (args.size() == 1)
		{
			if (args[0].Is(m_id))
			{
				m_id = args[0];
			}
			else if (args[0].Is(m_name))
			{
				CloneIntoName(args[0]);
			}
		}
	}

private:
	#if BIO_MEMORY_OPTIMIZE_LEVEL == 0
	Name m_name;
	#endif

	Id m_id;

	void CloneIntoName(Name name)
	{
		#if BIO_MEMORY_OPTIMIZE_LEVEL == 0
		if (m_name)
		{
			delete[] m_name;
		}
		string::CloneInto(
			name,
			m_name
		);
		#endif
	}
};

} //physical namespace
} //bio namespace
