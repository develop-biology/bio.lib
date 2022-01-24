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

#include "bio/chemical/Atom.h"
#include "bio/chemical/Bond.h"
#include "bio/chemical/PeriodicTable.h"

namespace bio {
namespace chemical {

Atom::Atom()
	:
	m_valence(1)
{
	m_bonds = new Bond[m_valence];
}

Atom::Atom(const Atom& other)
	:
	m_valence(1)
{
	m_bonds = new Bond[m_valence];
}

Atom::~Atom()
{
	//TODO: Make sure contents are always deleted.
	delete m_bonds[];
}

void Atom::Attenuate(const physical::Wave* other)
{
	for (
		Valence val = 0;
		val < m_valence;
		++val
		)
	{
		if (m_bonds[val].IsEmpty())
		{
			continue;
		}
		if (Wave::GetResonanceBetween(
			m_bonds[val].GetBonded(),
			other
		))
		{
			*(m_bonds[val].GetBonded()) += other;
		}
	}
	return val;
}

void Atom::Disattenuate(const physical::Wave* other)
{
	for (
		Valence val = 0;
		val < m_valence;
		++val
		)
	{
		if (m_bonds[val].IsEmpty())
		{
			continue;
		}
		if (Wave::GetResonanceBetween(
			m_bonds[val].GetBonded(),
			other
		))
		{
			*(m_bonds[val].GetBonded()) -= other;
		}
	}
	return val;
}

void Atom::operator+=(const Wave* other)
{
	Attenuate(other);
}

void Atom::operator-=(const Wave* other)
{
	Disattenuate(other);
}

Atom* Atom::Clone() const
{
	return new Atom(*this);
}


bool Atom::FormBondImplementation(
	Wave* toBond,
	AtomicNumber id,
	BondType type)
{
	Valence position = GetBondPosition(id);

	BIO_SANITIZE(toBond && id && position, ,
		return false);

	//position will == m_valence if the id for the given bonded was not found.
	//If that's the case, we need to expand our valence to make room for the new bond.
	//Unless a bond was broken, this should always be the case.
	if (position != m_valence)
	{
		if (!m_bonds[position].IsEmpty())
		{
			//Bond already exists.
			return false;
		}
		else
		{
			return m_bonds[position].Make(
				id,
				toBond,
				type
			);
		}
	}

	++m_valence;

	Bond* tempBonds = new Bond[m_valence];
	memcpy(
		tempBonds,
		m_bonds,
		m_valence * sizeof(Bond));
	delete[] m_bonds;
	m_bonds = tempBonds;

	m_bonds[position] = Bond(
		id,
		toBond,
		type
	);
	return true;
}

bool Atom::BreakBondImplementation(
	Wave* toBreak,
	AtomicNumber id,
	BondType type)
{
	Valence position = GetBondPosition(id);

	BIO_SANITIZE(toBreak && id && position, ,
		return false);
	BIO_SANITIZE(position < m_valence, ,
		return false);

	m_bonds[position].Empty();
	//Let dtor cleanup.

	return true;
}


Valence Atom::GetBondPosition(AtomicNumber bondedId) const
{
	BIO_SANITIZE(bondedId, ,
		return 0);
	Valence val = 0;
	for (
		; val < m_valence;
		++val
		)
	{
		if (m_bonds[val].GetId() == bondedId)
		{
			break;
		}
	}
	return val;
}

Valence Atom::GetBondPosition(Name typeName) const
{
	return GetBondPosition(PeriodicTable::Instance().IdFromName(typeName));
}

BondType Atom::GetBondType(Valence position) const
{
	BIO_SANITIZE(position < m_valence, ,
		return BondTypePerspective::InvalidId());
	return m_bonds[position].GetId();
}


} //chemical namespace
} //bio namespace