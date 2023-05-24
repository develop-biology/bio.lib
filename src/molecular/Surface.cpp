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

#include "bio/molecular/Surface.h"
#include "bio/molecular/Molecule.h"
#include "bio/molecular/common/Filters.h"
#include "bio/molecular/common/SymmetryTypes.h"

namespace bio {
namespace molecular {

Surface::Surface() :
	molecular::Class< Surface >(
		this,
		filter::Molecular(),
		symmetry_type::Variable()),
	EnvironmentDependent< Molecule* >(NULL),
	mBoundPosition(InvalidIndex())
{

}


Surface::Surface(
	const Name& name,
	Molecule* environment
)
	:
	molecular::Class< Surface >(
		this,
		name,
		filter::Molecular(),
		symmetry_type::Variable()),
	chemical::EnvironmentDependent< Molecule* >(environment),
	mBoundPosition(InvalidIndex())
{

}

Surface::Surface(const Surface& toCopy)
	:
	molecular::Class< Surface >(
		this,
		toCopy.GetName(),
		toCopy.GetFilter(),
		symmetry_type::Variable()),
	chemical::EnvironmentDependent< Molecule* >(toCopy),
	mBoundPosition(toCopy.mBoundPosition)
{
	chemical::Bond* bond;
	for (
		SmartIterator bnd = toCopy.mBonds.End();
		!bnd.IsBeforeBeginning();
		--bnd
		)
	{
		bond = bnd;
		if (bond->GetType() == bond_type::Manage())
		{
			//Calling FormBondImplementation directly saves us some work and should be safer than trying to do auto-template type determination from Clone().
			FormBondImplementation(
				bond->GetBonded()->Clone(),
				bond->GetId(),
				bond->GetType());
		}
	}
}

Surface::~Surface()
{
	chemical::Bond* bond;
	for (
		SmartIterator bnd = mBonds.End();
		!bnd.IsBeforeBeginning();
		--bnd
		)
	{
		bond = bnd;
		if (bond->GetType() == bond_type::Manage())
		{
			//bypass BreakBondImplementation and just do it.
			delete bond->GetBonded();
			bond->Break();
		}
	}
}

void Surface::SetEnvironment(Molecule* environment)
{
	this->chemical::EnvironmentDependent< Molecule* >::SetEnvironment(environment);
}

Code Surface::Reify(physical::Symmetry* symmetry)
{
	//TODO...
	return code::NotImplemented();
}

const physical::Symmetry* Surface::Spin() const
{
	//TODO...
	return NULL;
}

physical::Wave* Surface::Release(
	physical::Wave* toRelease,
	BondType bondType
)
{
	physical::Wave* ret = NULL;
	chemical::Bond* bond;
	for (
		SmartIterator bnd = mBonds.End();
		!bnd.IsBeforeBeginning();
		--bnd
		)
	{
		bond = bnd;
		if (bond->GetType() == bondType)
		{
			ret = ChemicalCast< physical::Wave* >(bond->GetBonded());
			BIO_SANITIZE_AT_SAFETY_LEVEL_1(!ret || ret != toRelease,
				continue,);
			bond->Break();
			break;
		}
	}

	mBoundPosition = InvalidIndex();

	return ret;
}

chemical::Substance* Surface::Release(
	const Name& toRelease,
	physical::Perspective< Id >* perspective,
	BondType bondType
)
{
	chemical::Substance* ret = NULL;
	chemical::Bond* bond;
	for (
		SmartIterator bnd = mBonds.End();
		!bnd.IsBeforeBeginning();
		--bnd
		)
	{
		bond = bnd;
		if (bond->GetType() == bondType)
		{
			ret = ChemicalCast< chemical::Substance* >(bond->GetBonded());
			BIO_SANITIZE_AT_SAFETY_LEVEL_1(ret, , continue);
			BIO_SANITIZE_AT_SAFETY_LEVEL_1(ret->IsName(toRelease), , continue);
			BIO_SANITIZE_AT_SAFETY_LEVEL_1(perspective && ret->GetPerspective() != perspective,
				continue,);
			bond->Break();
			break;
		}
	}

	mBoundPosition = InvalidIndex();

	return ret;
}

chemical::Substance* Surface::Release(
	Id toRelease,
	physical::Perspective< Id >* perspective,
	BondType bondType
)
{
	chemical::Substance* ret = NULL;
	chemical::Bond* bond;
	for (
		SmartIterator bnd = mBonds.End();
		!bnd.IsBeforeBeginning();
		--bnd
		)
	{
		bond = bnd;
		if (bond->GetType() == bondType)
		{
			ret = ChemicalCast< chemical::Substance* >(bond->GetBonded());
			BIO_SANITIZE_AT_SAFETY_LEVEL_1(ret, , continue);
			BIO_SANITIZE_AT_SAFETY_LEVEL_1(ret->IsId(toRelease), , continue);
			BIO_SANITIZE_AT_SAFETY_LEVEL_1(perspective && ret->GetPerspective() != perspective,
				continue,);
			bond->Break();
			break;
		}
	}

	mBoundPosition = InvalidIndex();

	return ret;
}

physical::Waves Surface::Release(BondType bondType)
{
	physical::Waves ret;
	chemical::Bond* bond;
	for (
		SmartIterator bnd = mBonds.End();
		!bnd.IsBeforeBeginning();
		--bnd
		)
	{
		bond = bnd;
		if (bond->GetType() == bondType)
		{
			ret.Add(ChemicalCast< physical::Wave* >(bond->GetBonded()));
			bond->Break();
		}
	}

	mBoundPosition = InvalidIndex();

	return ret;
}

physical::Wave* Surface::operator-=(physical::Wave* toRelease)
{
	return Release(toRelease);
}

chemical::Substance* Surface::operator-=(const Name& toRelease)
{
	return Release(toRelease);
}

chemical::Substance* Surface::operator-=(const Id& toRelease)
{
	return Release(toRelease);
}

physical::Waves Surface::operator--()
{
	return Release();
}

} //molecular namespace
} //bio namespace
