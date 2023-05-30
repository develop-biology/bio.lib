/*
 * This file is a part of the Biology project by eons LLC.
 * Biology (aka Develop Biology) is a framework for approaching software
 * development from a natural sciences perspective.
 *
 * Copyright (C) 2023 Séon O'Shannon & eons LLC
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

#include "bio/physical/Periodic.h"
#include "bio/chemical/common/Types.h"
#include "bio/chemical/common/Filters.h"
#include "bio/chemical/macro/Macros.h"
#include "bio/chemical/Substance.h"
#include "bio/chemical/EnvironmentDependent.h"

namespace bio {
namespace chemical {

class Solution;

/**
 * Solutes are used to implement shared-pointer style garbage collection. <br />
 * The idea is that Solutes have a "Concentration" representing their overall abundance. Once the Concentration of a Solute drops to 0, it is destroyed. <br />
 * The Concentration of a Solute is increased when it becomes available to a novel context (i.e. Solution) through Solution::Efflux (e.g. passed to a function). <br />
 * <br />
 * Solute is very similar to physical::Linear. However, it is intended to be more user-facing and feature-rich. Where Linear is used to implement shared pointers for Structures, Solute implements shared pointer logic for Solutions.
 * See Solution.h for more info, including the definition of Efflux and Influx. <br />
 * <br />
 * Solutes can be accessed (Effluxed) in 2 distinct manners. Both manners of Effluxing increase the Concentration of the desired Solute.<br />
 * 1. When const (read-only) Effluxing, Solutes use a parent-child relationship: only the parent will contain a valid pointer to the Dissolved Substance, each child then points to the parent. This is slower across threads, as each read operation requires a lock, but is faster overall, since the Dissolved Substance doesn't need to be duplicated.<br />
 * 2. When non-const (read-write & mutable) Effluxing, the Dissolved Substance is Cloned and a parent-child relationship is only used to indicate the source Substance. This style of Effluxing can be chained, effectively increasing the lifetime of the Solutes all the way up to the original Source. You should prefer mutable access across threads, as the Cloned Substance can be modified more freely, without the need for mutex locks. When you would like to merge the new Solute back into the original Solution, you can Diffuse it, which will call Influx on the parent's Solution, Mixing the Solute with its parent.<br />
 * <br />
 * For mutable Solutes, Diffusion happens automatically when the Solute is destroyed, unless you set SetDiffusionTime(diffusion::time::Never()). You may also set SetDiffusionTime(diffusion::time::Interval()) and SetInterval(...) to regularly Diffuse (this implies diffusion::time::Destruction() too). <br />
 * The default diffusion::effort is Active(), whereby changes are pushed up the Concentration gradient, toward the source of the Dissolved Substance. However, parents of mutable Solutes can push changes to children using SetDiffusionEffort(diffusion::effort::Passive()) (or ActiveAndPassive()). Passive() Diffusion is only applicable to Solutes which have a diffusion::time::Interval(). <br />
 * You should prefer Active() Diffusion for Solutes with short lifetimes, as it is more efficient and requires less work. Inversely, you should prefer Passive() Diffusion for Solutes that you intend to keep around; this will help keep the system in sync. <br />
 * <br />
 * When Solutes are Mixed, either through Diffusion or Solution::Influx, they are combined according to their Miscibilities. <br />
 * See Types.h & Miscibility.h for more info on Mixing strategies. <br />
 * <br />
 * To illustrate why parent Concentrations are increased when mutable Solutes are Effluxed, if function A uses Solute U1 from Solution V1 and function B also uses U1, even if each function exists in an isolated sub-Solution of V1, both functions will start with the same value. Furthermore, if B depends on modifications to U1 made by A, then having U1 be removed from V1 after A completes would break B. <br />
 * Thus, by keeping Solutes around when they are not needed by the immediate context allows sub-contexts to depend on the assumption that other, isolated sub-contexts can reach the same values. <br />
 */
class Solute :
	public chemical::Class< Solute >,
	public chemical::EnvironmentDependent< Solution* >,
	public Arrangement< Solute* >,
	virtual public physical::Periodic
{
public:

	/**
	 * Ensure virtual methods point to Class implementations. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(chemical, Solute)

	BIO_DEFAULT_IDENTIFIABLE_CONSTRUCTORS_WITH_COMMON_CONSTRUCTOR(
		chemical,
		Solute,
		filter::Chemical()
	)

	/**
	 * Copy constructor. <br />
	 * Adds the newly created Solute to the parent's Solution. <br />
	 * NOTE: const is a lie! <br />
	 * @param other
	 */
	Solute(const Solute& other);

	/**
	 *
	 */
	virtual ~Solute();

	/**
	 * Same as copy ctor. <br />
	 * const is a lie! <br />
	 * @param other
	 * @return *this.
	 */
	Solute& operator=(const Solute& other);

	/**
	 * Only applicable if *this is mutable. <br />
	 * @return the Substance that was Dissolved to form *this. <br />
	 */
	Substance* GetDissolvedSubstance();

	/**
	 * @return the Substance that was Dissolved to form *this. <br />
	 */
	const Substance* GetDissolvedSubstance() const;

	/**
	 * Get the Concentration of *this (i.e. its reference count). <br />
	 * @return the mConcentration of *this.
	 */
    virtual Concentration GetConcentration() const;

	/**
	 * Influx() *this back into the parent Solution, if it exists. <br />
	 */
	virtual void Diffuse() const;

	/**
	 * physical::Periodic method. <br />
	 * Only does work if the DiffusionTime is Interval(). <br />
	 * @return the result of diffusion
	 */
	virtual Code Crest();

	/**
	 * Combine *this with another Solute. <br />
	 * Uses the Mix Reaction to implement; see Mix.h for more info. <br />
	 * Only *this will be modified. <br />
	 * @param other
	 * @return the result of Mixing *this with other.
	 */
	virtual Code MixWith(const Solute& other);

	/**
	 * Combine *this with another Substance. <br />
	 * Uses the Mix Reaction to implement; see Mix.h for more info. <br />
	 * Only *this will be modified. <br />
	 * @param other
	 * @return the result of Mixing *this with other.
	 */
	virtual Code MixWith(const Substance* other);

	/**
	 * Sets the Environment and the Perspective of *this. <br />
	 * Don't let the environment go out of scope or be deleted before *this! <br />
	 * @param environment
	 */
	virtual void SetEnvironment(Solution* environment);

	/**
	 * @return the Index of *this in its parent Solution.
	 */
	virtual Index GetIndexInParentSolution() const;

	/**
	 * Set the Index of *this in its parent Solution. <br />
	 * @param indexInParentSolution
	 */
	virtual void SetIndexInParentSolution(Index indexInParentSolution);

	/**
	 * Set the diffusion time of *this. <br />
	 * Setting the DiffusionTime will control when *this is Diffused. <br />
	 * See Diffusion.h for some example values. <br />
	 * @param diffusionTime
	 */
	virtual void SetDiffusionTime(const DiffusionTime& diffusionTime);

	/**
	 * @return the diffusion time of *this.
	 */
	virtual DiffusionTime GetDiffusionTime() const;

	/**
	 * Set the diffusion effort of *this. <br />
	 * Setting the DiffusionEffort will control how *this is Diffused. <br />
	 * See Diffusion.h for some example values. <br />
	 * @param diffusionEffort
	 */
	virtual void SetDiffusionEffort(const DiffusionEffort& diffusionEffort);

	/**
	 * @return the diffusion effort of *this.
	 */
	virtual DiffusionEffort GetDiffusionEffort() const;

protected:

	/**
	 * The actual pointer to be shared. <br />
	 */
	Substance* mDissolvedSubstance;

	//Mutable for use in Increment & Decrement
    mutable Solute* mParentSolute;
	mutable Index mIndexInParent;

	DiffusionTime mDiffusionTime;
	DiffusionEffort mDiffusionEffort;

	/**
	 * This is what happens when mConcentration = 0. <br />
	 * We use this instead of the proper destructor method to allow resolution of virtual methods. <br />
	 */
	virtual void Destructor();

private:
	/**
	 *
	 */
	void CommonConstructor();

	//For creation & deletion.
	friend class Solution;
};

} //chemical namespace
} //bio namespace
