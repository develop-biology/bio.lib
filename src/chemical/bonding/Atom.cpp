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

#include "bio/chemical/bonding/Atom.h"
#include "bio/chemical/relativity/PeriodicTable.h"
#include "bio/chemical/symmetry/Symmetry.h"

namespace bio {
namespace chemical {

Atom::Atom()
	:
	physical::Class< Atom >(this),
	mBonds(4)
{
}

Atom::Atom(const Atom& other)
	:
	physical::Class< Atom >(this),
	mBonds(other.mBonds.GetCapacity())
{

}

Atom::~Atom()
{
	Bond* bond;
	for (
		SmartIterator bnd = mBonds.Begin();
		!bnd.IsAfterEnd();
		++bnd
		)
	{
		bond = bnd;
		if (bond)
		{
			delete bond;
			bond = NULL;
		}
	}
	mBonds.Clear();
}

Code Atom::Attenuate(const physical::Wave* other)
{
	BIO_SANITIZE(other, , return code::BadArgument1())
	BIO_SANITIZE(mBackflowPreventer.Has(other), , return code::NoErrorNoSucces())

	toPop = mBackflowPreventer.Add(other); //lock to prevent this from being called again.

	const physical::Wave* demodulated = other->Demodulate();
	Code ret = code::Success();

	Bond* bond;
	for (
		SmartIterator bnd = mBonds.End();
		!bnd.IsBeforeBeginning();
		--bnd
		)
	{
		bond = bnd;
		if (bond->IsEmpty())
		{
			continue;
		}
		if (physical::Wave::GetResonanceBetween(
			bond->GetBonded(),
			other
		).Size())
		{
			if (bond->GetBonded()->Attenuate(demodulated) != code::Success())
			{
				ret = code::UnknownError(); //user can debug from logs for now.
			}
		}
	}
	mBackflowPreventer.Erase(toPop);
	return ret;
}

Code Atom::Disattenuate(const physical::Wave* other)
{
	BIO_SANITIZE(other, , return code::BadArgument1())
	BIO_SANITIZE(other == mBackflowPreventer, , return code::NoErrorNoSucces())

	toPop = mBackflowPreventer.Add(other); //lock to prevent this from being called again.

	const physical::Wave* demodulated = other->Demodulate();
	Code ret = code::Success();

	Bond* bond;
	for (
		SmartIterator bnd = mBonds.End();
		!bnd.IsBeforeBeginning();
		--bnd
		)
	{
		bond = bnd;
		if (bond->IsEmpty())
		{
			continue;
		}
		if (physical::Wave::GetResonanceBetween(
			bond->GetBonded(),
			other
		).Size())
		{
			if (bond->GetBonded()->Disattenuate(demodulated) != code::Success())
			{
				ret = code::UnknownError(); //user can debug from logs for now.
			}
		}
	}
	mBackflowPreventer.Erase(toPop);
	return ret;
}

Valence Atom::FormBondImplementation(
	physical::Wave* toBond,
	AtomicNumber id,
	BondType type
)
{
	BIO_SANITIZE(toBond && id, ,
		return InvalidIndex());

	Valence position = GetBondPosition(id);
	Bond* bond;
	if (position && mBonds.IsAllocated(position))
	{
		bond = mBonds.OptimizedAccess(position);
		BIO_SANITIZE(!bond->IsEmpty(), ,
			return InvalidIndex())
		if (bond->Form(
			id,
			toBond,
			type
		))
		{
			return position;
		}
		return InvalidIndex();
	}

	return mBonds.Add(
		new Bond(
			id,
			toBond,
			type
		));
}

bool Atom::BreakBondImplementation(
	AtomicNumber id,
	BondType type
)
{
	Valence position = GetBondPosition(id);

	BIO_SANITIZE(id && position, ,
		return false);
	BIO_SANITIZE(mBonds.IsAllocated(position), ,
		return false);

	mBonds.OptimizedAccess(position)->Break();
	//Let dtor cleanup.

	return true;
}


Valence Atom::GetBondPosition(AtomicNumber bondedId) const
{
	BIO_SANITIZE(bondedId, ,
		return InvalidIndex());

	Bond* bond;
	for (
		SmartIterator bnd = mBonds.End();
		!bnd.IsBeforeBeginning();
		--bnd
		)
	{
		bond = bnd;
		if (bond->GetId() == bondedId)
		{
			return bnd.GetIndex();
		}
	}
	return InvalidIndex();
}

Valence Atom::GetBondPosition(const Name& typeName) const
{
	return GetBondPosition(SafelyAccess<PeriodicTable>()->GetIdWithoutCreation(typeName));
}

BondType Atom::GetBondType(Valence position) const
{
	BIO_SANITIZE(mBonds.IsAllocated(position), ,
		return BondTypePerspective::InvalidId());
	return mBonds.OptimizedAccess(position)->GetId();
}

physical::Symmetry* Atom::Spin() const
{
	//TODO...
	return Wave::Spin();
}

Code Atom::Reify(physical::Symmetry* symmetry)
{
	//TODO...
	return Wave::Reify(symmetry);
}

physical::Wave* Atom::GetBonded(Valence position)
{
	BIO_SANITIZE(mBonds.IsAllocated(position), ,
		return NULL)
	return mBonds.OptimizedAccess(position)->GetBonded();
}

const physical::Wave* Atom::GetBonded(Valence position) const
{
	BIO_SANITIZE(mBonds.IsAllocated(position), ,
		return NULL)
	return mBonds.OptimizedAccess(position)->GetBonded();
}

Bonds* Atom::GetAllBonds()
{
	return &mBonds;
}

const Bonds* Atom::GetAllBonds() const
{
	return &mBonds;
}


} //chemical namespace
} //bio namespace
