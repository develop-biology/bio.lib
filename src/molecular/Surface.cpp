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

#include "bio/molecular/Surface.h"
#include "bio/molecular/Molecule.h"
#include "bio/molecular/common/Filters.h"
#include "bio/molecular/common/SymmetryTypes.h"

namespace bio {
namespace molecular {

Surface::Surface(
	Name name,
	Molecule* environment
) :
	molecular::Class< Surface >(
		this,
		name,
		environment,
		filter::Molecular(),
		symmetry_type::Variable()),
	EnvironmentDependent< Molecule > (environment)
{

}

Surface::Surface(const Surface& toCopy)
	:
	molecular::Class< Surface >(
		this,
		toCopy.GetName(),
		toCopy.GetPerspective(),
		toCopy.GetFilter(),
		symmetry_type::Variable()),
	chemical::LinearStructuralComponent< Molecule >(toCopy),
	EnvironmentDependent< Molecule >(toCopy)
{
	for (
		chemical::Valence val = 0;
		val < toCopy.m_valence;
		++val
		)
	{
		if (toCopy.m_bonds[val].GetType() == bond_type::Manage())
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
		if (m_bonds[val].GetType() == bond_type::Manage())
		{
			delete m_bonds[val].GetBonded();
			m_bonds[val].Break();
		}
	}
}

void Surface::SetEnvironment(Molecule* environment)
{
	m_environment = environment;
	SetId(0); //0 should always be invalid.
	Identifiable< StandardDimension >::SetPerspective(environment);
}

void Surface::SetPerspective(Molecule* perspective)
{
	SetEnvironment(perspective);
}

Code Surface::Reify(physical::Symmetry* symmetry)
{
	//TODO...
	return code::NotImplemented();
}

physical::Symmetry* Surface::Spin() const
{
	//TODO...
	return NULL;
}

} //molecular namespace
} //bio namespace
