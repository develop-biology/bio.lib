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
	m_id(PeriodicTable::InvalidId()),
	m_bonded(NULL),
	m_type(bond_type::Empty())
{
}

Bond::Bond(
	AtomicNumber id,
	physical::Wave* bonded, <br />
	BondType type
)
	:
	m_id(id),
	m_bonded(bonded),
	m_type(type)
{
}

Bond::~Bond()
{

}

bool Bond::Form(
	AtomicNumber id,
	physical::Wave* bonded, <br />
	BondType type
)
{
	BIO_SANITIZE(bonded, ,
		return false);
	m_id = id;
	m_bonded = bonded;
	m_type = type;
	return true;
}

AtomicNumber Bond::GetId() const
{
	return m_id;
}

physical::Wave* Bond::GetBonded() <br />
{
	return m_bonded;
}

const physical::Wave* Bond::GetBonded() const <br />
{
	return m_bonded;
}

BondType Bond::GetType() const
{
	return m_type;
}

bool Bond::IsEmpty() const
{
	return m_bonded == NULL && m_type == bond_type::Empty();
}

void Bond::Break()
{
	//leave m_id intact.
	m_bonded = NULL;
	m_type = bond_type::Empty();
}

bool Bond::operator==(const AtomicNumber id) const
{
	return m_id == id;
}

bool Bond::operator==(const Bond& other) const
{
	return m_id == other.m_id;
}

} //chemical namespace
} //bio namespace
