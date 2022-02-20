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
#include "bio/chemical/structure/LinearStructuralComponent.h"

namespace bio {
namespace molecular {

class Molecule;

/**
 * Surfaces can be thought of as variables for the Biology syntax.
 * They either hold a single, Quantum  value, or are composed of a complex array of Molecules.
 * See Molecule.h for a more detailed explanation.
 */
class Surface :
	public Class<Surface>,
	public chemical::LinearStructuralComponent<Molecule*>,
	public EnvironmentDependent<Molecule>
{
public:

	/**
	 * @param name
	 */
	Surface(Name name, Molecule* environment=NULL);

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
	 * Required override of Wave::Clone()
	 * @return a new copy of *this.
	 */
	virtual Surface* Clone() const;

	/**
	 * Required method from Wave. See that class for details.
	 * @return a Symmetrical image of *this
	 */
	virtual Symmetry* Spin() const;

	/**
	 * Required method from Wave. See that class for details.
	 * Reconstruct *this from the given Symmetry.
	 * @param symmetry
	 */
	virtual Code Reify(Symmetry* symmetry);

	/**
	 * 2 Surfaces are equal if they contain the same Molecules, States, and Properties.
	 * @param other
	 * @return whether or not other is equivalent to *this.
	 */
	virtual bool operator==(const Surface& other) const;

	/**
	 * Create a Manage()d Bond with the given var.
	 * varPtr will be deleted when *this is destroyed.
	 * See Bonds.h for more on what Manage() does.
	 * @tparam T
	 * @param varPtr
	 * @return
	 */
	template <typename T>
	T* Manage(T* varPtr)
	{
		FormBond(varPtr, bond_type::Manage());
	}

	/**
	 * Create a Use()d Bond with the given var.
	 * These will not be deleted by *this.
	 * See Bonds.h for more on what Use() does.
	 * @tparam T
	 * @param varPtr
	 * @return
	 */
	template <typename T>
	T* Use(T* varPtr)
	{
		FormBond(varPtr, bond_type::Use());
	}

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

protected:

	/**
	 * Adds handler for Mange() Bonds.
	 * Otherwise passes to Atom::FormBondImplementation.
	 * Currently a nop & just passes to Atom; this is here so children can call this method in case we need to add to it in the future.
	 * @param toBond
	 * @param id
	 * @param position
	 * @return whether or not the Bond was Formed.
	 */
	virtual bool FormBondImplementation(Wave* toBond, AtomicNumber id, BondType type);

	/**
	 * Adds handler for Manage() Bonds.
	 * Otherwise passes to Atom::BreakBondImplementation.
	 * Currently a nop & just passes to Atom; this is here so children can call this method in case we need to add to it in the future.
	 * @param toDisassociate
	 * @param id
	 * @param type
	 * @return whether or not the Bond was Broken.
	 */
	virtual bool BreakBondImplementation(Wave* toDisassociate, AtomicNumber id, BondType type);

private:
	/**
	 * Common constructor code.
	 */
	void CtorCommon();
};

} //molecular namespace
} //bio namespace
