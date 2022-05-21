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

#include "bio/chemical/Atom.h"
#include "bio/chemical/PeriodicTable.h"
#include "bio/chemical/Symmetry.h"

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
	Bond* bondBuffer;
	for (
		SmartIterator bnd = mBonds.Begin();
		!bnd.IsAfterEnd();
		++bnd
		)
	{
		bondBuffer = bnd;
		if (bondBuffer)
		{
			delete bondBuffer;
			bondBuffer = NULL;
		}
	}
	mBonds.Clear();
}

Code Atom::Attenuate(const physical::Wave* other)
{
	BIO_SANITIZE(other, ,
		return code::BadArgument1())

	const physical::Wave* demodulated = other->Demodulate();
	Code ret = code::Success();

	Bond* bondBuffer;
	for (
		SmartIterator bnd = mBonds.End();
		!bnd.IsBeforeBeginning();
		--bnd
		)
	{
		bondBuffer = bnd;
		if (bondBuffer->IsEmpty())
		{
			continue;
		}
		if (physical::Wave::GetResonanceBetween(
			bondBuffer->GetBonded(),
			other
		).Size())
		{
			if (bondBuffer->GetBonded()->Attenuate(demodulated) != code::Success())
			{
				ret = code::UnknownError(); //user can debug from logs for now.
			}
		}
	}
	return ret;
}

Code Atom::Disattenuate(const physical::Wave* other)
{
	BIO_SANITIZE(other, ,
		return code::BadArgument1())

	const physical::Wave* demodulated = other->Demodulate();
	Code ret = code::Success();

	Bond* bondBuffer;
	for (
		SmartIterator bnd = mBonds.End();
		!bnd.IsBeforeBeginning();
		--bnd
		)
	{
		bondBuffer = bnd;
		if (bondBuffer->IsEmpty())
		{
			continue;
		}
		if (physical::Wave::GetResonanceBetween(
			bondBuffer->GetBonded(),
			other
		).Size())
		{
			if (bondBuffer->GetBonded()->Disattenuate(demodulated) != code::Success())
			{
				ret = code::UnknownError(); //user can debug from logs for now.
			}
		}
	}
	return ret;
}

bool Atom::FormBondImplementation(
	Wave* toBond,
	AtomicNumber id,
	BondType type
)
{
	BIO_SANITIZE(toBond && id, ,
		return false);

	Valence position = GetBondPosition(id);
	Bond* bondBuffer;
	if (position && mBonds.IsAllocated(position))
	{
		bondBuffer = mBonds.OptimizedAccess(position);
		BIO_SANITIZE(!bondBuffer->IsEmpty(), ,
			return false)
		return bondBuffer->Form(
			id,
			toBond,
			type
		);
	}
	//implicitly cast the addition index to a bool.
	return mBonds.Add(
		new Bond(
			id,
			toBond,
			type
		));
}

bool Atom::BreakBondImplementation(
	Wave* toBreak,
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
		return 0);

	Bond* bondBuffer;
	for (
		SmartIterator bnd = mBonds.End();
		!bnd.IsBeforeBeginning();
		--bnd
		)
	{
		bondBuffer = bnd;
		if (bondBuffer->GetId() == bondedId)
		{
			return bnd.GetIndex();
		}
	}
	return 0;
}

Valence Atom::GetBondPosition(Name typeName) const
{
	return GetBondPosition(PeriodicTable::Instance().GetIdWithoutCreation(typeName));
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
