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

#include "bio/chemical/Bond.h"
#include "bio/chemical/PeriodicTable.h"
#include "bio/physical/Wave.h"
#include "bio/common/macros/Macros.h"

namespace bio {
namespace chemical {

Bond::Bond()
	:
	mId(PeriodicTable::InvalidId()),
	mBonded(NULL),
	mType(bond_type::Empty())
{
}

Bond::Bond(
	AtomicNumber id,
	physical::Wave* bonded,
	BondType type
)
	:
	mId(id),
	mBonded(bonded),
	mType(type)
{
}

Bond::~Bond()
{

}

bool Bond::Form(
	AtomicNumber id,
	physical::Wave* bonded,
	BondType type
)
{
	BIO_SANITIZE(bonded, ,
		return false);
	mId = id;
	mBonded = bonded;
	mType = type;
	return true;
}

AtomicNumber Bond::GetId() const
{
	return mId;
}

physical::Wave* Bond::GetBonded()
{
	return mBonded;
}

const physical::Wave* Bond::GetBonded() const
{
	return mBonded;
}

BondType Bond::GetType() const
{
	return mType;
}

bool Bond::IsEmpty() const
{
	return mBonded == NULL && mType == bond_type::Empty();
}

void Bond::Break()
{
	//leave mId intact.
	mBonded = NULL;
	mType = bond_type::Empty();
}

bool Bond::operator==(const AtomicNumber id) const
{
	return mId == id;
}

bool Bond::operator==(const Bond& other) const
{
	return mId == other.mId;
}

} //chemical namespace
} //bio namespace
