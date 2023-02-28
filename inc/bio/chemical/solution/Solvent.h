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
#include "bio/chemical/structure/motif/DependentMotif.h"
#include "bio/chemical/Substance.h"


namespace bio {
namespace chemical {

class Solute;

/**
 * Solvents contain Solutes and embody the idea of context. <br />
 * This system provides traditional member access while simultaneously ensuring multiple conflicting writes from external sources never produce a conflict. <br />
 * <br />
 * Solvents closely model physical::Lines & structural Motifs, but are intended to be more user facing and have a slightly different features.
 * The main difference between Solvents and structural Motifs is that multiple Solvents cannot exist in the same context. Because Solvents represent context, combining 2 Solvents simply yields a novel context, i.e. a singular Solvent. On the other hand, multiple Structures can be easily separated after being combined. With that said, Solvents can be nested (e.g. through Vesicles). <br />
 * Another notable difference between Solvents and structural Motifs is that Structures can be formed of many different components, while Solvents are only comprised of Solutes; if you would like to add something to a Solvent, it must be a Solute (i.e. a Dissolved Substance).
 * <br />
 * Solvents and structural Motifs define 2 points in a continuum of organization: Structural Motifs are highly ordered and predictable (i.e. they have a preset type) while Solvents are dynamic and more "chaotic" (i.e. any Substance can be Dissolved in them). The remainder of this continuum is populated by the usages of these 2 classes. <br />
 * <br />
 * Solutes themselves are essentially shared pointers which track their reference count via their Concentration. <br />
 * Egressing a Solute to other Solvents increases the Concentration of the Solute and allows its Substance to be accessed from other "contexts". <br />
 * Solutes can be Egressed as const for read-only access or as non-const for read-write access. <br />
 * <br />
 * This style of "Concentration goes up on access" is the inverse of real life. In the real world, "access", as quantified by binding affinity and reaction rate, is limited by a solute's concentration. We find this inversion to be more in line with state machine linear access semantics but may enforce a ConcentrationLimit or similar mechanism in a future release. <br />
 * <br />
 * In real-world chemistry, the notation "[chemical]" is used to indicate the concentration of "chemical" in some solvent. However, Concentration is mostly irrelevant for access purposes and is thus ignored. You may access the ByteStream representation of a Solute with [Index || SmartIterator] or the Solute itself with [Id || Name].<br />
 * <br />
 * Solvents rely on the IdPerspective to map their contents (all Solutes are Identifiable<Id>). <br />
 */
class Solvent :
	public chemical::Class< Solvent >,
	public Arrangement< Solute >,
	virtual public chemical::Substance
{
public:

	BIO_DISAMBIGUATE_ALL_CLASS_METHODS_FOR(chemical, Solvent)

	BIO_DEFAULT_IDENTIFIABLE_CONSTRUCTORS_WITH_COMMON_CONSTRUCTOR(
		chemical,
		Solvent,
		filter::Chemical()
	)

	/**
	 * Dissolving a Substance in a Solvent creates a Solute that represents the Substance. <br />
	 * The Solute will take ownership of the Substance and delete it when the Solute's Concentration drops to 0. <br />
	 * @param toDissolve
	 * @return the Id of the created Solute in *this or INVALID_ID.
	 */
	virtual Id Dissolve(Substance* toDissolve);

	/**
	 * Separating a Substance from a Solvent removes the associated Solute from the Solvent (*this). <br />
	 * Use this method if you would like to take control of a previously Dissolved Substance. <br />
	 * @param id
	 * @return a previously Dissolved Substance of the given Id or NULL.
	 */
	virtual Substance* Separate(const Id& id);

	/**
	 * Ingressing a Solute into a Solvent does either one of 2 things: <br />
	 * 1. If the Solute does not already exist in *this, Clones toIngress (including the associated Substance) and adds the clone to *this. After being Ingressed, the original Solute can be safely deleted. <br />
	 * 2. If the Solute already exists in *this, the existing Solute "mixes" with toIngress according to the Miscibility of the existing Solute. <br />
	 * @param toIngress the Solute you want to add or combine within *this.
	 * @param existing ignore this.
	 * @return the Id of the Solute created or mixed in *this.
	 */
	virtual Id Ingress(const Solute& toIngress);

	/**
	 * Egress a Solute to access it. <br />
	 * Egressing a Solute will create a new Solute that has the relevant parent Solute information set. Thus, Egressing that (already Egressed) Solute will maintain the appropriate parent relationship and automatically handle Concentration changes. <br />
	 * <br />
	 * This should be done for all non-native Solvent access, e.g. sub-Solvents or external Solvents. <br />
	 * You don't have to use Egress to access Solutes within *this iff "you" own it. <br />
	 * @param soluteId the Id of the desired Solute.
	 * @return a Solute from within *this.
	 */
	virtual Solute& Egress(const Id& soluteId);

	/**
	 * Egress a Solute to access it. <br />
	 * Egressing a Solute will create a new Solute that has the relevant parent Solute information set. Thus, Egressing that (already Egressed) Solute will maintain the appropriate parent relationship and automatically handle Concentration changes. <br />
	 * <br />
	 * This should be done for all non-native Solvent access, e.g. sub-Solvents or external Solvents. <br />
	 * You don't have to use Egress to access Solutes within *this iff "you" own it. <br />
	 * @param soluteId the Id of the desired Solute.
	 * @return a Solute from within *this.
	 */
	virtual const Solute& Egress(const Id& soluteId) const;

	/**
	 * Egress a Solute to access it. <br />
	 * Egressing a Solute will create a new Solute that has the relevant parent Solute information set. Thus, Egressing that (already Egressed) Solute will maintain the appropriate parent relationship and automatically handle Concentration changes. <br />
	 * <br />
	 * This should be done for all non-native Solvent access, e.g. sub-Solvents or external Solvents. <br />
	 * You don't have to use Egress to access Solutes within *this iff "you" own it. <br />
	 * @param substanceName the Name of the Substance associated with the desired Solute.
	 * @return a Solute from within *this.
	 */
	virtual Solute& Egress(const Name& substanceName);

	/**
	 * Egress a Solute to access it. <br />
	 * Egressing a Solute will create a new Solute that has the relevant parent Solute information set. Thus, Egressing that (already Egressed) Solute will maintain the appropriate parent relationship and automatically handle Concentration changes. <br />
	 * <br />
	 * This should be done for all non-native Solvent access, e.g. sub-Solvents or external Solvents. <br />
	 * You don't have to use Egress to access Solutes within *this iff "you" own it. <br />
	 * @param substanceName the Name of the Substance associated with the desired Solute.
	 * @return a Solute from within *this.
	 */
	virtual const Solute& Egress(const Name& substanceName) const;

	/**
	 * operator wrappers around Egress(). <br />
	 * @param soluteId
	 * @return Egress(...)
	 */
	virtual Solute& operator[](const Id& soluteId);

	/**
	 * operator wrappers around Egress(). <br />
	 * @param soluteId
	 * @return Egress(...)
	 */
	virtual const Solute& operator[](const Id& soluteId) const;

	/**
	 * operator wrappers around Egress(). <br />
	 * @param substanceName
	 * @return Egress(...)
	 */
	virtual Solute& operator[](const Name& substanceName);

	/**
	 * operator wrappers around Egress(). <br />
	 * @param substanceName
	 * @return Egress(...)
	 */
	virtual const Solute& operator[](const Name& substanceName) const;
};

} //chemical namespace
} //bio namespace
