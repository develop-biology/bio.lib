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

#include "bio/common/VirtualBase.h"
#include "bio/common/string/String.h"
#include "bio/common/macro/OSMacros.h"
#include "Perspective.h"
#include "Observer.h"
#include "bio/physical/Wave.h"
#include "bio/physical/common/Class.h"
#include <cstring>

namespace bio {
namespace physical {

/**
 * An Identifiable class has a name and a numeric identifier (Identifier). <br />
 * Names are used for debugging and human interaction, while Ids are used for run-time processing. <br />
 * Identifiable classes require a Perspective<DIMENSION> to maintain a mapping of Identifier <-> Name pairs. <br />
 * The Identifier exists within a DIMENSION, i.e. an int type (almost always unsigned), like uint32_t. <br />
 * For more on DIMENSIONs, see Perspective.h <br />
 *
 *The reason this class is templated is so that less numerous classes can use a smaller ID type, which decreases memory footprint and increases processing speed (less bits to check).
 * Unfortunately, inheritance reveals a problem with this design: Identifiable classes cannot change their ID type. <br />
 * For example, if you have one class that you expect a small number of and then derive from that class, expecting a larger number of children, you either must derive from Identifiable twice, indulging in diamond inheritance, or increase the size of Identifier to encompass all possible uses. <br />
 * For this reason, the default DIMENSION (Id, from Types.h) should be used in nearly all cases, unless you want to ensure either your class is not derived from or that it remains separated from other code. <br />
 * An example of using a non-Id can be found in Codes. Codes have their own DIMENSION, as they should not be inherited from but may still be expanded upon through user-defined values (simply additional name <-> id definitions). <br />
*/
template < typename DIMENSION >
class Identifiable :
	public physical::Class< Identifiable< DIMENSION > >,
	public Observer< Perspective< DIMENSION > >,
	protected VirtualBase
{
public:
	typedef DIMENSION Identifier;
	typedef Arrangement< Identifier > Ids;

	/**
	 * Ensure virtual methods point to Class implementations. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(physical, Identifiable< DIMENSION >)


	/**
	 * @param perspective
	 */
	explicit Identifiable(Perspective< DIMENSION >* perspective = NULL)
		:
		physical::Class< Identifiable< DIMENSION > >(this),
		#if BIO_MEMORY_OPTIMIZE_LEVEL < 1
		mName(Perspective< DIMENSION >::InvalidName()),
		#endif
		mId(Perspective< DIMENSION >::InvalidId())
	{
		if (perspective)
		{
			this->Observer< Perspective< DIMENSION > >::SetPerspective(perspective);
		}
	}

	/**
	 * @param name
	 * @param perspective
	 */
	explicit Identifiable(
		const Name& name,
		Perspective< DIMENSION >* perspective = NULL
	)
		:
		physical::Class< Identifiable< DIMENSION > >(this),
		#if BIO_MEMORY_OPTIMIZE_LEVEL < 1
		mName(name),
		#endif
		mId(Perspective< DIMENSION >::InvalidId())
	{
		if (perspective)
		{
			this->Observer< Perspective< DIMENSION > >::SetPerspective(perspective);
			this->mId = this->GetPerspective()->GetIdFromName(mName);
			this->MakeWave();
		}
		else
		{
			this->mId = Perspective< DIMENSION >::InvalidId();
		}
	}

	/**
	 * @param id
	 * @param perspective
	 */
	explicit Identifiable(
		Identifier id,
		Perspective< DIMENSION >* perspective = NULL
	)
		:
		physical::Class< Identifiable< DIMENSION > >(this),
		mId(Perspective< DIMENSION >::InvalidId())
	{
		if (perspective)
		{
			this->Observer< Perspective< DIMENSION > >::SetPerspective(perspective);
			#if BIO_MEMORY_OPTIMIZE_LEVEL < 1
			this->mName = perspective->GetNameFromId(id);
			#endif
			this->MakeWave();
		}
		else
		{
			this->mId = Perspective< DIMENSION >::InvalidId();
			this->mName = Perspective< DIMENSION >::InvalidName();
		}
	}

	/**
	 * @param other
	 */
	Identifiable(const Identifiable& other)
		:
		physical::Class< Identifiable< DIMENSION > >(this),
		#if BIO_MEMORY_OPTIMIZE_LEVEL < 1
		mName(other.GetName()),
		#endif
		mId(other.mId)
	{
		this->Observer< Perspective< DIMENSION > >::SetPerspective(other.GetPerspective());
	}

	/**
	 *
	 */
	virtual ~Identifiable()
	{

	}

	/**
	 * @return *this as its Identifier.
	 */
	virtual operator DIMENSION() const
	{
		return this->mId;
	}

	/**
	 * @param id
	 * @return whether or not the id of *this matches id provided and double checks with the Perspective used by *this.
	 */
	virtual bool operator==(const Identifier id) const
	{
		if (!this->GetId())
		{
			return false;
		}
		if (!this->GetId() == id.mT)
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
	virtual bool operator==(const Name& name) const
	{
		if (!this->GetName())
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
		#if BIO_MEMORY_OPTIMIZE_LEVEL < 1
		return this->mName;
		#else
		return BIO_SANITIZE_WITH_CACHE(this->GetPerspective(), RESULT->GetNameFromId(mId), return "INVALID_NAME");
		#endif
	}

	/**
	 * @return the id of *this.
	 */
	virtual Identifier GetId() const
	{
		return this->mId;
	}

	/**
	 * Sets the name and updates the type to the given name. <br />
	 * Has no effect if perspective is null. <br />
	 * @param name
	 * @return whether or not the Name was updated.
	 */
	virtual bool SetName(const Name& name)
	{
		BIO_SANITIZE(this->GetPerspective(),
			,
			return false
		)

		#if BIO_MEMORY_OPTIMIZE_LEVEL < 1
		this->mName = name;
		#endif

		this->mId = this->GetPerspective()->GetIdFromName(name);
		return true;
	}

	/**
	 * Sets the id and updates the name to the given id. <br />
	 * Has no effect if perspective is null. <br />
	 * @param id
	 * @return whether or not the id was updated.
	 */
	virtual bool SetId(Identifier id)
	{
		BIO_SANITIZE(this->GetPerspective(),
			,
			return false
		)

		mId = id;

		#if BIO_MEMORY_OPTIMIZE_LEVEL < 1
		mName = this->GetPerspective()->GetNameFromId(mId);
		#endif

		return true;
	}

	/**
	 * This is preferred to GetName() == ... because == may not exist for the Name type and/or the Name type may change in a later release. <br />
	 * Case Sensitive. <br />
	 * @param name
	 * @return a comparison operation on the given name with the name of *this.
	 */
	virtual bool IsName(const Name& name) const
	{
		return name == this->GetName();
	}

	/**
	 * @param name
	 * @return whether or not the given name is offensive. JK, it's just a case insensitive version of IsName.
	 */
	virtual bool IsNameInsensitive(const Name& name) const
	{
		return !strcasecmp(
			name.AsCharString(),
			this->GetName().AsCharString());
	}

	/**
	 * @param id
	 * @return whether or not the given id matches that of *this.
	 */
	virtual bool IsId(Identifier id) const
	{
		return id == this->mId;
	}

	/**
	 * Sets the perspective for *this. <br />
	 * @param perspective
	 */
	virtual void SetPerspective(Perspective< DIMENSION >* perspective)
	{
		this->Observer< Perspective< DIMENSION > >::SetPerspective(perspective);

		if (this->IsName(Perspective< DIMENSION >::InvalidName()) && !this->IsId(Perspective< DIMENSION >::InvalidId()))
		{
			#if BIO_MEMORY_OPTIMIZE_LEVEL < 1
			mName = this->GetPerspective()->GetNameFromId(mId);
			#endif
		}
		else if (!this->IsName(Perspective< DIMENSION >::InvalidName()) && this->IsId(Perspective< DIMENSION >::InvalidId()))
		{
			mId = this->GetPerspective()->GetIdFromName(GetName());
		}
	}


	#if 0
	//No need to compile these nops.

	/**
	 * Required method from Wave. See that class for details. <br />
	 * Because Symmetry is Identifiable, it cannot be #included here. We rely on children of Identifiable (primarily chemical:: objects to implement Spin()). <br />
	 * @return a Symmetrical image of *this
	 */
	virtual Symmetry* Spin() const 
	{
		return NULL;
	}

	/**
	 * Required method from Wave. See that class for details. <br />
	 * Reconstruct *this from the given Symmetry. <br />
	 * Because Symmetry is Identifiable, it cannot be #included here. We rely on children of Identifiable (primarily chemical:: objects to implement Reify()). <br />
	 * @param symmetry
	 */
	virtual Code Reify(Symmetry* symmetry) 
	{
		//nop
	}
	#endif

protected:
	/**
	 * VirtualBase required method. See that class for details (in common/) <br />
	 * @param args
	 */
	virtual void InitializeImplementation(ByteStreams& args)
	{

		if (args.Size() == 2)
		{
			if (args[args.GetEndIndex()].Is< Perspective< DIMENSION >* >())
			{
				this->Observer< Perspective< DIMENSION > >::SetPerspective(args[args.GetEndIndex()]);
			}
			args.Erase(args.GetEndIndex());
		}
		if (args.Size() == 1)
		{
			if (args[args.GetEndIndex()].Is(mId))
			{
				this->mId = args[args.GetEndIndex()].As< Identifier >();

				#if BIO_MEMORY_OPTIMIZE_LEVEL < 1
				if (this->mId && this->GetPerspective())
				{
					this->mName = this->GetPerspective()->GetNameFromId(this->mId);
				}
				#endif
			}
			#if BIO_MEMORY_OPTIMIZE_LEVEL < 1
			else if (args[args.GetEndIndex()].Is(mName))
			{
				//mName keeps being deleted when this temp goes out of scope.
				//If we can find a way to reliably use move constructors here, we can make the Mode READ_ONLY and potentially save ourselves a bunch of memory.
				//Or we can leave as is and let people use BIO_MEMORY_OPTIMIZE_LEVEL to remove storing names altogether.
				this->mName = String::SetMode(args[args.GetEndIndex()].As< String >(), String::READ_WRITE);

				if (this->GetPerspective())
				{
					this->mId = this->GetPerspective()->GetIdFromName(this->mName);
				}
			}
			#endif
		}
	}

private:
	#if BIO_MEMORY_OPTIMIZE_LEVEL < 1
	Name mName;
	#endif

	Identifier mId;
};

} //physical namespace
} //bio namespace
