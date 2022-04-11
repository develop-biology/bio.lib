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

#include "bio/molecular/common/BondTypes.h"
#include "bio/molecular/common/Types.h"
#include "bio/molecular/common/Class.h"
#include "bio/molecular/macros/Macros.h"
#include "EnvironmentDependent.h"
#include "bio/chemical/structure/motif/LinearMotif.h"

namespace bio {
namespace molecular {

class Molecule;

/**
 * Surfaces can be thought of as variables for the Biology syntax.
 * They either hold a single, Quantum  value, or are composed of a complex array of Molecules.
 * See Molecule.h for a more detailed explanation.
 */
class Surface :
	public Class< Surface >,
	public chemical::LinearMotif< Molecule* >,
	public EnvironmentDependent< Molecule >
{
public:

	/**
	 * Ensure virtual methods point to Class implementations.
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(molecular,
		Surface)

	/**
	 * @param name
	 */
	Surface(
		Name name,
		Molecule* environment = NULL
	);

	/**
	 * Copying a Surface generates a new set of Molecules and will Clone any Manage()d Waves from toCopy into *this.
	 * NOTE: all Use()d Waves will be lost. Since *this does not control what it Uses, it cannot (will not) duplicate it.
	 * Keep in mind that *this will delete all Managed Waves on destruction.
	 * @param toCopy
	 */
	Surface(const Surface& toCopy);

	/**
	 * deletes all Manage()d Bonds.
	 */
	virtual ~Surface();

	/**
	 * Required method from Wave. See that class for details.
	 * @return a Symmetrical image of *this
	 */
	virtual physical::Symmetry* Spin() const;

	/**
	 * Required method from Wave. See that class for details.
	 * Reconstruct *this from the given Symmetry.
	 * @param symmetry
	 */
	virtual Code Reify(physical::Symmetry* symmetry);

	/**
	 * Create a Manage()d Bond with the given var.
	 * varPtr will be deleted when *this is destroyed.
	 * See Bonds.h for more on what Manage() does.
	 * @tparam T
	 * @param varPtr
	 * @return
	 */
	template < typename T >
	T Manage(T varPtr)
	{
		FormBond(
			varPtr,
			bond_type::Manage());
	}

	/**
	 * Create a Use()d Bond with the given var.
	 * These will not be deleted by *this.
	 * See Bonds.h for more on what Use() does.
	 * @tparam T
	 * @param varPtr
	 * @return
	 */
	template < typename T >
	T Use(T varPtr)
	{
		FormBond(
			varPtr,
			bond_type::Use());
	}

	/**
	 * Binding, as opposed to permanent Bonding, forms a temporary association with the given Substance.
	 * Binding forms a Temporary Bond, allowing *this to be treated as the Bound Substance.
	 * @tparam T
	 * @param toBind
	 * @return the Bound type.
	 */
	template < typename T >
	T Bind(T toBind, BondType bondType = bond_type::Temporary())
	{
		FormBond(
			toBind,
			bondType
		);
		return toBind;
	}

	/**
	 * Breaks the Temporary Bond formed by Bind.
	 * @param toRelease
	 * @return the previously bound Substance or NULL.
	 */
	virtual physical::Wave* Release(physical::Wave* toRelease, BondType bondType = bond_type::Temporary());

	/**
	 * Breaks the Temporary Bond formed by Bind.
	 * NOTE: the given Substance could be Identifiable through some unknown Perspective, so this does actual string comparison. Unless a Perspective is given, in which case numeric comparison is done on the given Name.
	 * @param toRelease
	 * @return the previously bound Substance or NULL.
	 */
	virtual chemical::Substance* Release(Name toRelease, physical::Perspective<StandardDimension>* perspective = NULL, BondType bondType = bond_type::Temporary());

	/**
	 * Breaks the Temporary Bond formed by Bind.
	 * NOTE: the given Substance could be Identifiable through some unknown Perspective, so this does an unreliable numeric comparison. However, if a Perspective is given, we can be certain if the id we find is correct or not.
	 * @param toRelease
	 * @return the previously bound Substance or NULL.
	 */
	virtual chemical::Substance* Release(StandardDimension toRelease, physical::Perspective<StandardDimension>* perspective = NULL, BondType bondType = bond_type::Temporary());

	/**
	 * Releases all Temporarily Bound Substances
	 * @return all Temporarily Bound Substances
	 */
	virtual physical::Waves ReleaseAll(BondType bondType = bond_type::Temporary());

	/**
	 * Sets both the m_environment and m_perspective and updates m_id.
	 * @param environment
	 */
	virtual void SetEnvironment(Molecule* environment);

	/**
	 * Sets both the m_environment and m_perspective and updates m_id.
	 * @param perspective a Molecule.
	 */
	virtual void SetPerspective(Molecule* perspective);

	/**
	 * Wrapper around Bind
	 * @param toBind
	 * @return result of Bind(...)
	 */
	template < typename T >
	T operator+=(T toBind)
	{
		return Bind< T >(toBind);
	}

	/**
	 * Wrapper around Release
	 * @param toRelease
	 * @return result of Release
	 */
	virtual physical::Wave* operator-=(physical::Wave* toRelease);

	/**
	 * Wrapper around Release
	 * @param toRelease
	 * @return result of Release
	 */
	virtual chemical::Substance* operator-=(Name toRelease);

	/**
	 * Wrapper around Release
	 * @param toRelease
	 * @return result of Release
	 */
	virtual chemical::Substance* operator-=(StandardDimension toRelease);

	/**
	 * Wrapper around ReleaseAll
	 * @return all Temporarily Bound Substances.
	 */
	virtual physical::Waves operator--();
};

} //molecular namespace
} //bio namespace
