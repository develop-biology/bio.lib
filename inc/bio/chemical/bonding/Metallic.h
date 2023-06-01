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

#include "Atom.h"
#include "Bond.h"
#include "bio/chemical/common/BondTypes.h"

namespace bio {
//not chemical.

/**
 * Metallic bonds create a "horizontal inheritance" similar to Covalent bonds. <br />
 * With Metallic bonds, all Bonds are shared between the Bonding objects. <br />
 * This solves the problem of Bonding a child class but trying to use As< ParentClass >(). <br />
 * However, this introduces the limitation of not being able to Bond more than one object of the same type. <br />
 * If both of the objects being Bonded have already Bonded to an object of the same type, the behavior is undefined (ideally neither class is affected & the result of As< SharedClass >() depends on which object you start with. <br />
 * When instantiating a Metallic class, a new T will be created and Metallicly bonded to the child class of *this (through the shared Atom). <br />
 * Each Metallic Bond has a distinct Type based on the template it is constructed with. This type helps to identify which Bonds have been shared. <br />
 * You may get the BondType *this uses with GetBondType(). <br />
 * IMPORTANT NOTE: Metallic Bonds are only updated when a new Metallic Bond is created. <br />
 * NOTE: atm, there is no way to provide arguments to the new Ts. <br />
 * See Atom.h for more on Bonds.<br />
 * @tparam T
 */
template < class T >
class Metallic :
	virtual public chemical::Atom
{
public:
	/**
	 *
	 */
	Metallic() :
		mT(new T())
	{
		DonateBonds(mT, this);
		DonateBonds(this, mT);
	}

	/**
	 *
	 */
	virtual ~Metallic()
	{
		if (mT)
		{
			BreakDonatedBonds(this);
			BreakDonatedBonds(mT);
			delete mT;
			mT = NULL;
		}
	}

	/**
	 * @return what *this was created for.
	 */
	T* Object()
	{
		return mT;
	}

	/**
	 * @return what *this was created for.
	 */
	const T* Object() const
	{
		return mT;
	}

	/**
	 * Pass all Bonds in donor to the receiver. <br />
	 * The BondType used will be that of GetBondType(). <br />
	 * @param donor
	 * @param receiver
	 */
	static void DonateBonds(
		chemical::Atom* donor,
		chemical::Atom* receiver
	)
	{
		chemical::Bond* bond;

		for (
			SmartIterator bnd = donor->GetAllBonds()->Begin();
			!bnd.IsAfterEnd();
			++bnd
		)
		{
			bond = bnd;

			//Skip already Bonded objects.
			if (receiver->GetBondPosition(bond->GetId()))
			{
				continue;
			}

			//Nuke the BondType, since it could lead to deleting the same pointer twice.
			//We also want to keep track of where the Bonds came from.
			receiver->FormBondImplementation(
				bond->GetBonded(),
				bond->GetId(),
				GetBondType()
			);
		}
	}

	/**
	 * Break all Bonds in receiver that were donated by donor. <br />
	 * @param receiver
	 */
	static void BreakDonatedBonds(chemical::Atom* receiver)
	{
		chemical::Bond* bond;

		for (
			SmartIterator bnd = receiver->GetAllBonds()->Begin();
			!bnd.IsAfterEnd();
			++bnd
		)
		{
			bond = bnd;

			if (bond->GetType() != GetBondType() || !receiver->GetBondPosition(bond->GetId()))
			{
				continue;
			}

			receiver->BreakBondImplementation(
				bond->GetId(),
				GetBondType()
			);
		}
	}

	/**
	 * @return the unique BondType used by *this.
	 */
	static BondType GetBondType()
	{
		static const BondType BondId = BondTypePerspective::Instance().GetIdFromName(type::TypeName< Metallic< T > >());
		return BondId;
	}

protected:
	T* mT;
};

} //bio namespace
