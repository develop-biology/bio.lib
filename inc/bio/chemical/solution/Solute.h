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

#include "bio/chemical/common/Types.h"
#include "bio/chemical/macro/Macros.h"
#include "bio/chemical/Substance.h"
#include "bio/chemical/EnvironmentDependent.h"

namespace bio {
namespace chemical {

class Solvent;

/**
 * Solutes are used to implement shared-pointer style garbage collection. <br />
 * The idea is that Solutes have a "Concentration" representing their overall abundance. Once the Concentration of a Solute drops to 0, it is destroyed. <br />
 * The Concentration of a Solute is increased when it becomes available to a novel context through Solvent::Diffuse (e.g. passed to a function). <br />
 * Solutes are cloned when non-const Diffusing; while const access to a Solute simply increases its Concentration. <br />
 * See Solvent.h for more info on how Diffusion works. <br />
 * <br />
 * When a Solute's Concentration reaches 0, before it is destroyed, it automatically lets its parent Solvent know that it is no longer needed. <br />
 * For example, if Solvent V2 gets Solute U2 by cloning Solute U1 from Solvent V1, the originating Solute U1 will have its Concentration increased while U2 exists. Once U2 is removed from V2, the Concentration of U1 (in V1) will drop. <br />
 * This process ensures that Solutes persist in greater contexts while in use by sub-contexts, and makes it possible for isolated sub-contexts to communicate through their common greater context. <br />
 * For example, if function A uses Solute U1 from Solvent V1 and function B also uses U1, even if each function exists in an isolated sub-Solvent of V1, both functions will start with the same value. Furthermore, if B depends on modifications to U1 made by A, then having U1 be removed from V1 after A completes would break B. <br />
 * Thus, by keeping Solutes around when they are not needed by the immediate context allows sub-contexts to depend on the assumption that other, isolated sub-contexts can reach the same values. <br />
 * <br />
 * Additionally, when a Solute's Concentration reaches 0 and before it's destroyed, it is Dissolved back into its parent Solvent according to the Miscibility of its parent Solute. This allows sub-contexts to affect greater contexts and thus cooperate with neighboring, isolated contexts. <br />
 */
class Solute :
	public chemical::Class< Solute >,
	public chemical::EnvironmentDependent< Solvent >,
	virtual public Substance
{
public:

	BIO_DISAMBIGUATE_ALL_CLASS_METHODS_FOR(chemical, Solute)

	BIO_DEFAULT_IDENTIFIABLE_CONSTRUCTORS_WITH_COMMON_CONSTRUCTOR(
		chemical,
		Solute,
		filter::Chemical
	)

	/**
	 *
	 */
	virtual ~Solute();

	/**
	 * Get the Concentration of *this (i.e. its reference count). <br />
	 * @return the mConcentration of *this.
	 */
    virtual Concentration GetConcentration() const;

    /**
	 * Increase the mConcentration of *this by 1. <br />
	 * Must be const to make Solute::Diffusion mechanics work. <br />
	 */
	virtual void IncrementConcentration() const;

	/**
	 * Decrease the mConcentration of *this by 1. <br />
	 * Must be const to make Solute::Diffusion mechanics work. <br />
	 */
	virtual void DecrementConcentration() const;

	/**
	 * Manually set the mConcentration of *this. <br />
	 * This should not be used except in VERY controlled cases (such as Solvent Cloning). <br />
	 * @param toSet
	 */
	virtual void SetConcentration(Concentration toSet) const;

	/**
	 * Dissolve() *this back into the parent Solvent, if it exists. <br />
	 */
	virtual void Resolve() const;

	/**
	 * Sets the Environment and the Perspective of *this. <br />
	 * Don't let the environment go out of scope or be deleted before *this! <br />
	 * @param environment
	 */
	virtual void SetEnvironment(Solvent* environment);

protected:
    mutable Concentration mConcentration;
    mutable Solvent* mParentSolvent;
	const Id mIdInParent;

	/**
	 * This is what happens when mConcentration = 0.
	 */
	virtual void Destructor();

private:
	/**
	 *
	 */
	void CommonConstructor();
};

} //chemical namespace
} //bio namespace
