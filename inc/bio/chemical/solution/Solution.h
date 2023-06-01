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

#include "bio/chemical/common/Types.h"
#include "bio/chemical/common/Filters.h"
#include "bio/chemical/common/Class.h"
#include "bio/chemical/common/Diffusion.h"
#include "bio/chemical/macro/Macros.h"
#include "bio/chemical/structure/motif/DependentMotif.h"
#include "bio/chemical/Substance.h"
#include "bio/physical/shape/Line.h"

namespace bio {
namespace chemical {

class Solute;

/**
 * Solutions contain Solutes and embody the idea of context. <br />
 * This system provides traditional member access while simultaneously ensuring multiple conflicting writes from external sources never produce a conflict. <br />
 * <br />
 * Solutions closely model physical::Lines & structural Motifs, but are intended to be more user facing and have a slightly different features.
 * The main difference between Solutions and structural Motifs is that multiple Solutions cannot exist in the same context. Because Solutions represent context, combining 2 Solutions simply yields a novel context, i.e. a singular Solution. On the other hand, multiple Structures can be easily separated after being combined. With that said, Solutions can be nested (e.g. through Vesicles). <br />
 * Another notable difference between Solutions and structural Motifs is that Structures can be formed of many different components, while Solutions are only comprised of Solutes; if you would like to add something to a Solution, it must be a Solute (i.e. a Dissolved Substance).
 * <br />
 * Solutions and structural Motifs define 2 points in a continuum of organization: Structural Motifs are highly ordered and predictable (i.e. they have a preset type) while Solutions are dynamic and more "chaotic" (i.e. any Substance can be Dissolved in them). The remainder of this continuum is populated by the usages of these 2 classes. <br />
 * <br />
 * Solutes themselves are essentially shared pointers which track their reference count via their Concentration. <br />
 * Effluxing a Solute to other Solutions increases the Concentration of the Solute and allows its Substance to be accessed from other "contexts". <br />
 * Solutes can be Effluxed as const for read-only access or as non-const for read-write access. <br />
 * <br />
 * This style of "Concentration goes up on access" is the inverse of real life. In the real world, "access", as quantified by binding affinity and reaction rate, is limited by a solute's concentration. We find this inversion to be more in line with state machine linear access semantics but may enforce a ConcentrationLimit or similar mechanism in a future release. <br />
 * <br />
 * In real-world chemistry, the notation "[chemical]" is used to indicate the concentration of "chemical" in some Solution. However, Concentration is mostly irrelevant for access purposes and is thus ignored. You may access the ByteStream representation of a Solute with [Index || SmartIterator] or the Solute itself with [Id || Name].<br />
 * <br />
 * Solutions rely on the IdPerspective to map their contents (all Solutes are Identifiable<Id>). <br />
 */
class Solution :
	public chemical::Class< Solution >,
	virtual public chemical::Substance
{
public:

	/**
	 * Ensure virtual methods point to Class implementations. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(chemical, Solution)

	BIO_DEFAULT_IDENTIFIABLE_CONSTRUCTORS(
		chemical,
		Solution,
		filter::Chemical()
	)

	/**
	 * Dissolving a Substance in a Solution creates a Solute that represents the Substance. <br />
	 * The Solute will take ownership of the Substance and delete it when the Solute's Concentration drops to 0. <br />
	 * Dissolving a Substance which has already been Dissolved will result in the 2 Substances Mixing together. <br />
	 * @param toDissolve
	 * @return the Id of the created Solute in *this or INVALID_ID.
	 */
	virtual Id Dissolve(
		Substance* toDissolve,
		const DiffusionTime& diffusionTime = diffusion::time::Destruction(),
		const DiffusionEffort& diffusionEffort = diffusion::effort::Active()
	);

	/**
	 * Separating a Substance from a Solution removes the associated Solute from the Solution (*this). <br />
	 * Use this method if you would like to take control of a previously Dissolved Substance. <br />
	 * @param id
	 * @return a previously Dissolved Substance of the given Id or NULL.
	 */
	virtual Substance* Separate(const Id& id);

	/**
	 * Influxing a Solute into a Solution does either one of 2 things: <br />
	 * 1. If the Solute does not already exist in *this, Clones toInflux (including the associated Substance) and adds the clone to *this. After being Influxed, the original Solute can be safely deleted. <br />
	 * 2. If the Solute already exists in *this, the existing Solute "mixes" with toInflux according to the Miscibility of the existing Solute. <br />
	 * @param toInflux the Solute you want to add or combine within *this.
	 * @param existing ignore this.
	 * @return the Id of the Solute created or mixed in *this.
	 */
	virtual Id Influx(const Solute& toInflux);

	/**
	 * Efflux a Solute to access it. <br />
	 * Effluxing a Solute will create a new Solute that has the relevant parent Solute information set. Thus, Effluxing that (already Effluxed) Solute will maintain the appropriate parent relationship and automatically handle Concentration changes. <br />
	 * <br />
	 * This should be done for all non-native Solution access, e.g. sub-Solutions or external Solutions. <br />
	 * You don't have to use Efflux to access Solutes within *this iff "you" own it. <br />
	 * @param soluteId the Id of the desired Solute.
	 * @return a Solute from within *this.
	 */
	virtual Solute Efflux(const Id& soluteId);

	/**
	 * Efflux a Solute to access it. <br />
	 * Effluxing a Solute will create a new Solute that has the relevant parent Solute information set. Thus, Effluxing that (already Effluxed) Solute will maintain the appropriate parent relationship and automatically handle Concentration changes. <br />
	 * <br />
	 * This should be done for all non-native Solution access, e.g. sub-Solutions or external Solutions. <br />
	 * You don't have to use Efflux to access Solutes within *this iff "you" own it. <br />
	 * @param soluteId the Id of the desired Solute.
	 * @return a Solute from within *this.
	 */
	virtual const Solute Efflux(const Id& soluteId) const;

	/**
	 * Efflux a Solute to access it. <br />
	 * Effluxing a Solute will create a new Solute that has the relevant parent Solute information set. Thus, Effluxing that (already Effluxed) Solute will maintain the appropriate parent relationship and automatically handle Concentration changes. <br />
	 * <br />
	 * This should be done for all non-native Solution access, e.g. sub-Solutions or external Solutions. <br />
	 * You don't have to use Efflux to access Solutes within *this iff "you" own it. <br />
	 * @param substanceName the Name of the Substance associated with the desired Solute.
	 * @return a Solute from within *this.
	 */
	virtual Solute Efflux(const Name& substanceName);

	/**
	 * Efflux a Solute to access it. <br />
	 * Effluxing a Solute will create a new Solute that has the relevant parent Solute information set. Thus, Effluxing that (already Effluxed) Solute will maintain the appropriate parent relationship and automatically handle Concentration changes. <br />
	 * <br />
	 * This should be done for all non-native Solution access, e.g. sub-Solutions or external Solutions. <br />
	 * You don't have to use Efflux to access Solutes within *this iff "you" own it. <br />
	 * @param substanceName the Name of the Substance associated with the desired Solute.
	 * @return a Solute from within *this.
	 */
	virtual const Solute Efflux(const Name& substanceName) const;

	/**
	 * operator wrappers around Efflux(). <br />
	 * @param soluteId
	 * @return Efflux(...)
	 */
	virtual Solute operator[](const Id& soluteId);

	/**
	 * operator wrappers around Efflux(). <br />
	 * @param soluteId
	 * @return Efflux(...)
	 */
	virtual const Solute operator[](const Id& soluteId) const;

	/**
	 * operator wrappers around Efflux(). <br />
	 * @param substanceName
	 * @return Efflux(...)
	 */
	virtual Solute operator[](const Name& substanceName);

	/**
	 * operator wrappers around Efflux(). <br />
	 * @param substanceName
	 * @return Efflux(...)
	 */
	virtual const Solute operator[](const Name& substanceName) const;

	/**
	 * @return the mSolutes from *this.
	 */
	virtual physical::Line* GetAllSolutes();

protected:
	physical::Line mSolutes;
};

} //chemical namespace
} //bio namespace
