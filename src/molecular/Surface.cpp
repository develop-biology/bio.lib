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

#include "bio/molecular/Surface.h"
#include "bio/molecular/Molecule.h"

namespace bio {
namespace molecular {

void Surface::CtorCommon()
{
	//Required chemical::Atom method
	//Ensures *this can be accessed from the <template> interface.
	Bond(
		this,
		bond_type::Virtual());
}

Surface::Surface(
	Name name,
	Molecule* environment)
	:
	chemical::Substance(),
	chemical::LinearStructuralComponent<Molecule*>(&MoleculePerspective::Instance()),
	EnvironmentDependent<Molecule>(environment),
	physical::Identifiable<StandardDimension>(
		name,
		environment
	)
{
	CtorCommon();
}

Surface::Surface(const Surface& toCopy)
	:
	chemical::Substance(toCopy),
	chemical::LinearStructuralComponent<Molecule*>(toCopy),
	EnvironmentDependent<Molecule>(toCopy)
{
	CtorCommon();
	for (
		chemical::Valence val = 0;
		val < toCopy.m_valence;
		++val
		)
	{
		if (toCopy.m_bonds[val].GetType() == Manage())
		{
			//Calling FormBondImplementation directly saves us some work and should be safer than trying to do auto-template type determination from Clone().
			FormBondImplementation(
				toCopy.m_bonds[val].GetBonded()->Clone(),
				toCopy.m_bonds[val].GetId(),
				toCopy.m_bonds[val].GetType());
		}
	}
}

Surface::~Surface()
{
	for (
		chemical::Valence val = 0;
		val < m_valence;
		++val
		)
	{
		if (m_bonds[val].GetType() == Manage())
		{
			delete m_bonds[val].GetBonded();
			m_bonds[val].Break();
		}
	}
}

bool Surface::FormBondImplementation(
	Wave* toBond,
	AtomicNumber id,
	BondType type)
{
	return Atom::FormBondImplementation(
		toBond,
		id,
		type
	)
}

bool Surface::BreakBondImplementation(
	Wave* toDisassociate,
	AtomicNumber id,
	BondType type)
{
	return Atom::BreakBondImplementation(
		toBond,
		id,
		type
	);
}


void Surface::SetEnvironment(Molecule* environment)
{
	m_environment = environment;
	m_id = 0;
	Identifiable<StandardDimension>::SetPerspective(environment);
}

void Surface::SetPerspective(Molecule* perspective)
{
	SetEnvironment(perspective);
}

} //molecular namespace
} //bio namespace
