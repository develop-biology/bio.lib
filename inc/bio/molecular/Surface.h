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
#include "bio/molecular/macro/Macros.h"
#include "bio/chemical/EnvironmentDependent.h"

namespace bio {
namespace molecular {

class Molecule;

/**
 * Surfaces can be thought of as variables for the Biology syntax. <br />
 * They hold a single value but the type of that value may be arbitrarily complex (e.g. a Molecule with more Surfaces on it). <br />
 * See Molecule.h for a more detailed explanation. <br />
 */
class Surface :
	public Class< Surface >,
	public chemical::EnvironmentDependent< Molecule >
{
public:

	/**
	 * Ensure virtual methods point to Class implementations. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(molecular, Surface)

	/**
	 * Don't use this.
	 */
	Surface();

	/**
	 * @param name
	 */
	Surface(
		const Name& name,
		Molecule* environment = NULL
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
	virtual physical::Symmetry* Spin() const;

	/**
	 * Required method from Wave. See that class for details. <br />
	 * Reconstruct *this from the given Symmetry. <br />
	 * @param symmetry
	 */
	virtual Code Reify(physical::Symmetry* symmetry);

	/**
	 * Create a Manage()d Bond with the given var. <br />
	 * varPtr will be deleted when *this is destroyed. <br />
	 * See Bonds.h for more on what Manage() does. <br />
	 * @tparam T
	 * @param varPtr
	 * @return
	 */
	template < typename T >
	T& Manage(T* varPtr)
	{
		BIO_STATIC_ASSERT(!type::IsPointer< T >())

		chemical::AtomicNumber bondedId = GetBondId< T >();
		mBoundPosition = FormBondImplementation(
			(new physical::Quantum< T >(varPtr))->AsWave(),
			bondedId,
			bond_type::Manage()
		);

		return Probe< T >();
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
	T& Use(T* varPtr)
	{
		BIO_STATIC_ASSERT(!type::IsPointer< T >())

		chemical::AtomicNumber bondedId = GetBondId< T >();
		mBoundPosition = FormBondImplementation(
			(new physical::Quantum< T >(varPtr))->AsWave(),
			bondedId,
			bond_type::Use()
		);

		return Probe< T >();
	}

	/**
	 * Probe is the Biology style "get". <br />
	 * This is a simple wrapper around Atom::As<>(). If you need to Get the T* *this is Bound to, use As directly. <br />
	 * @tparam T a non-pointer type that is Bound to *this.
	 * @return a T that is Bound to *this or 0.
	 */
	template < typename T >
	T& Probe()
	{
		BIO_STATIC_ASSERT(!type::IsPointer< T >())

		BIO_SANITIZE(mBoundPosition,,return 0)
		BIO_SANITIZE(mBonds.IsAllocated(mBoundPosition),,return 0)

		//We won't bother re-implementing the Atom::As method here, even though we could be more efficient since we already know the Bonded position.

		T* ret = this->As< T* >();
		return *ret;
	}

	/**
	 * Bind is the Biology style "set". <br />
	 * If *this is not Managing or Using a value already, the provided value will be Temporarily Bonded (i.e. Bound) to *this. Otherwise, the already Bound value will be set to that provided. <br />
	 * When *this already has a value Bound and Bind is called, the
	 * @tparam T
	 * @param toBind
	 * @param bondType
	 * @return the Bound type.
	 */
	template < typename T >
	T& Bind(
		T toBind, //must be pass-by-value for basic types.
		BondType bondType = bond_type::Temporary())
	{
		BIO_STATIC_ASSERT(!type::IsPointer< T >());
		if (mBoundPosition)
		{
			T* bound = As< T* >();
			*bound = toBind;
		}

		FormBond< T >(
			toBind,
			bondType
		);
		mBoundPosition = GetBondPosition< T >();
		return Probe< T >();
	}

	/**
	 * Release a Surface Binding if you need to change the type of the Surface. <br />
	 * Generally you shouldn't be changing variable types at runtime, so if you think you need this, double check your design. <br />
	 * Breaks the Temporary Bond formed by Bind. <br />
	 * Can also be used on the Bond formed by Use and Manage. <br />
	 * @param toRelease
	 * @param bondType
	 * @return the previously bound Substance or NULL.
	 */
	virtual physical::Wave* Release(
		physical::Wave* toRelease,
		BondType bondType = bond_type::Temporary());

	/**
	 * Release a Surface Binding if you need to change the type of the Surface. <br />
	 * Generally you shouldn't be changing variable types at runtime, so if you think you need this, double check your design. <br />
	 * Breaks the Temporary Bond formed by Bind. <br />
	 * Can also be used on the Bond formed by Use and Manage. <br />
	 * NOTE: the given Substance could be Identifiable through some unknown Perspective, so this does actual string comparison. Unless a Perspective is given, in which case numeric comparison is done on the given Name. <br />
	 * @param toRelease
	 * @param perspective
	 * @param bondType
	 * @return the previously bound Substance or NULL.
	 */
	virtual chemical::Substance* Release(
		const Name& toRelease,
		physical::Perspective< Id >* perspective = NULL,
		BondType bondType = bond_type::Temporary());

	/**
	 * Release a Surface Binding if you need to change the type of the Surface. <br />
	 * Generally you shouldn't be changing variable types at runtime, so if you think you need this, double check your design. <br />
	 * Breaks the Temporary Bond formed by Bind. <br />
	 * Can also be used on the Bond formed by Use and Manage. <br />
	 * NOTE: the given Substance could be Identifiable through some unknown Perspective, so this does an unreliable numeric comparison. However, if a Perspective is given, we can be certain if the id we find is correct or not. <br />
	 * @param toRelease
	 * @param perspective
	 * @param bondType
	 * @return the previously bound Substance or NULL.
	 */
	virtual chemical::Substance* Release(
		Id toRelease,
		physical::Perspective< Id >* perspective = NULL,
		BondType bondType = bond_type::Temporary());

	/**
	 * Release a Surface Binding if you need to change the type of the Surface. <br />
	 * Generally you shouldn't be changing variable types at runtime, so if you think you need this, double check your design. <br />
	 * Releases all Temporarily Bound Substances <br />
	 * Can also be used on the Bond formed by Use and Manage. <br />
	 * @return all Temporarily Bound Substances
	 */
	virtual physical::Waves Release(BondType bondType = bond_type::Temporary());

	/**
	 * Sets both the mEnvironment and mPerspective and updates mId. <br />
	 * @param environment
	 */
	virtual void SetEnvironment(Molecule* environment);

	/**
	 * Sets both the mEnvironment and mPerspective and updates mId. <br />
	 * @param perspective a Molecule.
	 */
	virtual void SetPerspective(Molecule* perspective);

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
	virtual physical::Wave* operator-=(physical::Wave* toRelease);

	/**
	 * Wrapper around Release <br />
	 * @param toRelease
	 * @return result of Release
	 */
	virtual chemical::Substance* operator-=(const Name& toRelease);

	/**
	 * Wrapper around Release <br />
	 * @param toRelease
	 * @return result of Release
	 */
	virtual chemical::Substance* operator-=(const Id& toRelease);

	/**
	 * Wrapper around Release <br />
	 * @return all Temporarily Bound Substances.
	 */
	virtual physical::Waves operator--();

protected:
	/**
	 * should be 0 or 1 in practice (i.e. we prevent >1 Binding).
	 */
	chemical::Valence mBoundPosition;
};

} //molecular namespace
} //bio namespace
