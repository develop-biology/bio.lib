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
 * Surfaces can be thought of as variables for the Biology syntax. <br />
 * They either hold a single, Quantum  value, or are composed of a complex array of Molecules. <br />
 * See Molecule.h for a more detailed explanation. <br />
 */
class Surface :
	public Class< Surface >,
	public chemical::LinearMotif< Molecule* >, <br />
	public EnvironmentDependent< Molecule >
{
public:

	/**
	 * Ensure virtual methods point to Class implementations. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(molecular,
		Surface)

	/**
	 * @param name
	 */
	Surface(
		Name name,
		Molecule* environment = NULL <br />
	);

	/**
	 * Copying a Surface generates a new set of Molecules and will Clone any Manage()d Waves from toCopy into *this. <br />
	 * NOTE: all Use()d Waves will be lost. Since *this does not control what it Uses, it cannot (will not) duplicate it. <br />
	 * Keep in mind that *this will delete all Managed Waves on destruction. <br />
	 * @param toCopy
	 */
	Surface(const Surface& toCopy);

	/**
	 * deletes all Manage()d Bonds. <br />
	 */
	virtual ~Surface();

	/**
	 * Required method from Wave. See that class for details. <br />
	 * @return a Symmetrical image of *this
	 */
	virtual physical::Symmetry* Spin() const; <br />

	/**
	 * Required method from Wave. See that class for details. <br />
	 * Reconstruct *this from the given Symmetry. <br />
	 * @param symmetry
	 */
	virtual Code Reify(physical::Symmetry* symmetry); <br />

	/**
	 * Create a Manage()d Bond with the given var. <br />
	 * varPtr will be deleted when *this is destroyed. <br />
	 * See Bonds.h for more on what Manage() does. <br />
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
	 * Create a Use()d Bond with the given var. <br />
	 * These will not be deleted by *this. <br />
	 * See Bonds.h for more on what Use() does. <br />
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
	 * Binding, as opposed to permanent Bonding, forms a temporary association with the given Substance. <br />
	 * Binding forms a Temporary Bond, allowing *this to be treated as the Bound Substance. <br />
	 * @tparam T
	 * @param toBind
	 * @param bondType
	 * @return the Bound type.
	 */
	template < typename T >
	T Bind(
		T toBind,
		BondType bondType = bond_type::Temporary())
	{
		FormBond(
			toBind,
			bondType
		);
		return toBind;
	}

	/**
	 * Breaks the Temporary Bond formed by Bind. <br />
	 * @param toRelease
	 * @param bondType
	 * @return the previously bound Substance or NULL.
	 */
	virtual physical::Wave* Release( <br />
		physical::Wave* toRelease, <br />
		BondType bondType = bond_type::Temporary());

	/**
	 * Breaks the Temporary Bond formed by Bind. <br />
	 * NOTE: the given Substance could be Identifiable through some unknown Perspective, so this does actual string comparison. Unless a Perspective is given, in which case numeric comparison is done on the given Name. <br />
	 * @param toRelease
	 * @param perspective
	 * @param bondType
	 * @return the previously bound Substance or NULL.
	 */
	virtual chemical::Substance* Release( <br />
		Name toRelease,
		physical::Perspective< StandardDimension >* perspective = NULL, <br />
		BondType bondType = bond_type::Temporary());

	/**
	 * Breaks the Temporary Bond formed by Bind. <br />
	 * NOTE: the given Substance could be Identifiable through some unknown Perspective, so this does an unreliable numeric comparison. However, if a Perspective is given, we can be certain if the id we find is correct or not. <br />
	 * @param toRelease
	 * @param perspective
	 * @param bondType
	 * @return the previously bound Substance or NULL.
	 */
	virtual chemical::Substance* Release( <br />
		StandardDimension toRelease,
		physical::Perspective< StandardDimension >* perspective = NULL, <br />
		BondType bondType = bond_type::Temporary());

	/**
	 * Releases all Temporarily Bound Substances <br />
	 * @return all Temporarily Bound Substances
	 */
	virtual physical::Waves ReleaseAll(BondType bondType = bond_type::Temporary());

	/**
	 * Sets both the m_environment and m_perspective and updates m_id. <br />
	 * @param environment
	 */
	virtual void SetEnvironment(Molecule* environment); <br />

	/**
	 * Sets both the m_environment and m_perspective and updates m_id. <br />
	 * @param perspective a Molecule.
	 */
	virtual void SetPerspective(Molecule* perspective); <br />

	/**
	 * Wrapper around Bind <br />
	 * @param toBind
	 * @return result of Bind(...)
	 */
	template < typename T >
	T operator+=(T toBind)
	{
		return Bind< T >(toBind);
	}

	/**
	 * Wrapper around Release <br />
	 * @param toRelease
	 * @return result of Release
	 */
	virtual physical::Wave* operator-=(physical::Wave* toRelease); <br />

	/**
	 * Wrapper around Release <br />
	 * @param toRelease
	 * @return result of Release
	 */
	virtual chemical::Substance* operator-=(Name toRelease); <br />

	/**
	 * Wrapper around Release <br />
	 * @param toRelease
	 * @return result of Release
	 */
	virtual chemical::Substance* operator-=(StandardDimension toRelease); <br />

	/**
	 * Wrapper around ReleaseAll <br />
	 * @return all Temporarily Bound Substances.
	 */
	virtual physical::Waves operator--();
};

} //molecular namespace
} //bio namespace
