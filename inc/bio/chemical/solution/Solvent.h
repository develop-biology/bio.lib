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
 * Solvents and Structural Motifs define 2 points in a continuum of organization: Structural Motifs are highly ordered and predictable while Solvents are dynamic and "chaotic". The remainder of this continuum is populated by the usages of these 2 classes. <br />
 * The main difference between Solvents and Structural Motifs is that multiple Solvents cannot exist in the same context. Because Solvents represent context, combining 2 Solvents simply yields a novel context, i.e. a singular Solvent. On the other hand, multiple Structures can be easily separated after being combined. <br />
 * Another notable difference between Solvents and Structural Motifs is that Structures can be formed of many different components, while Solvents are only comprised of Solutes; if you would like to add something to a Solvent, it must be a Solute.
 * <br />
 * Solutes are essentially shared pointers which track their reference count via their Concentration. <br />
 * Diffusing Solutes to other Solvents allows the variables in *this to be accessed from other "contexts". <br />
 * When const Diffusing (i.e. read-only), only the Concentration of the Solute is increased. <br />
 * When non-const Diffusing (i.e. read-write) to a Solvent other than *this, the Solute is Cloned and passed to the writer. <br />
 * This system provides traditional member access while simultaneously ensuring multiple conflicting writes from external sources never produce a conflict. The entire point of the Circulatory system is to merge these multiple Solutes back into one. <br />
 * NOTE that this style of "Concentration goes up on access" is the inverse of real life. In the real world, "access", as quantified by binding affinity and reaction rate, is limited by a solute's concentration. We find this inversion to be more in line with state machine linear access semantics but may enforce a ConcentrationLimit or similar mechanism in a future release. <br />
 * <br />
 * In real-world chemistry, the notation "[chemical]" is used to indicate the concentration of "chemical" in some solvent. We imitate that here with the [] operator, the only difference is that the caller must specify which Solvent they are talking about and the return is not the Concentration but the Solute itself. <br />
 * For example, Cytoplasm["Glucose"] would give the Glucose Solute within the Cytoplasm Solvent (its Concentration is irrelevant for our purposes). <br />
 * <br />
 * Solvents are their own Perspectives to allow for easy identification of their contents: "VariableA" in "Solvent1" can have Id 3, while "VariableA" in "Solvent2" can have Id 234687. Because Solutes are not identified through a global Perspective, we can use Diffuse(Name ...) calls, etc. (consider if they were identified globally and we store a child of Solute which is Identifiable through a different Perspective than that of the other Solutes we store). <br />
 */
class Solvent :
	public chemical::Class< Solvent >,
	public Covalent< chemical::DependentMotif< Solute* > >,
	virtual public physical::Perspective< Id >,
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
	 * Dissolving a Solute in a Solvent does either one of 2 things: <br />
	 * 1. Clones toDissolve and assigns it a new Id; after being Dissolved, the original Solute can be deleted; this is likely what you want. <br />
	 * 2. "mixes" an existing Solute with toDissolve according to the Miscibility of the existing Solute; this should be done automatically by Solutes which Diffused out of *this. <br />
	 * @param toDissolve the Solute you want to add to *this.
	 * @param existing ignore this.
	 * @return the Id of the Solute created or mixed in *this.
	 */
	virtual Id Dissolve(const Solute* toDissolve, const Id& existing=InvalidId());

	/**
	 * Access a Solute in *this from any Solvent, including *this. <br />
	 * IMPORTANT: Remember to Solute::DecrementConcentration() when you are done using the returned Solute!!! <br />
	 * @param soluteId the Id of the desired Solute
	 * @param targetSolvent the Id of the calling Solvent; if 0, this->GetId() is used instead.
	 * @return a Solute from this with the given targetSolvent.
	 */
	virtual Solute* Diffuse(const Id& soluteId) const;

	/**
	 * Access a Solute in *this from any Solvent, including *this. <br />
	 * When targetSolvent does not match this->GetId(), either a new or previously Cloned Solute is returned. <br />
	 * Cloned Solutes are stored in *this and have an AccessorId equal to targetSolvent. <br />
	 * @param soluteName the Name of the desired Solute
	 * @param targetSolvent the Id of the calling Solvent; if 0, this->GetId() is used instead.
	 * @return a Solute from this; a Cloned Solute if targetSolvent != this->GetId().
	 */
	virtual Solute* Diffuse(const Name& soluteName, const Id& targetSolvent=0);

	/**
	 * Access a Solute in *this from any Solvent, including *this. <br />
	 * IMPORTANT: Remember to Solute::DecrementConcentration() when you are done using the returned Solute!!! <br />
	 * @param soluteName the Name of the desired Solute
	 * @param targetSolvent the Id of the calling Solvent; if 0, this->GetId() is used instead.
	 * @return a Solute from this; a Cloned Solute if targetSolvent != this->GetId().
	 */
	virtual const Solute* Diffuse(const Name& soluteName, const Id& targetSolvent=0) const;

	/**
	 * operator wrappers around Diffuse(). <br />
	 * Uses this->GetId() and SHOULD NOT BE CALLED FROM AN EXTERNAL SOLVENT! <br />
	 * @param soluteId
	 * @return Diffuse(..., this->GetId())
	 */
	virtual Solute* operator[](const Id& soluteId);

	/**
	 * operator wrappers around Diffuse(). <br />
	 * Uses this->GetId() and SHOULD NOT BE CALLED FROM AN EXTERNAL SOLVENT! <br />
	 * @param soluteId
	 * @return Diffuse(..., this->GetId())
	 */
	virtual const Solute* operator[](const Id& soluteId) const;

	/**
	 * operator wrappers around Diffuse(). <br />
	 * Uses this->GetId() and SHOULD NOT BE CALLED FROM AN EXTERNAL SOLVENT! <br />
	 * @tparam T
	 * @param soluteId
	 * @return Diffuse(..., this->GetId())
	 */
	template < typename T >
	T operator[](const Id& soluteId)
	{
		return ChemicalCast< T >(Diffuse(soluteId, GetId()));
	}

	/**
	 * operator wrappers around Diffuse(). <br />
	 * Uses this->GetId() and SHOULD NOT BE CALLED FROM AN EXTERNAL SOLVENT! <br />
	 * @param soluteName
	 * @return Diffuse(..., this->GetId())
	 */
	virtual Solute* operator[](const Name& soluteName);

	/**
	 * operator wrappers around Diffuse(). <br />
	 * Uses this->GetId() and SHOULD NOT BE CALLED FROM AN EXTERNAL SOLVENT! <br />
	 * @param soluteName
	 * @return Diffuse(..., this->GetId())
	 */
	virtual const Solute* operator[](const Name& soluteName) const;

	/**
	 * operator wrappers around Diffuse(). <br />
	 * Uses this->GetId() and SHOULD NOT BE CALLED FROM AN EXTERNAL SOLVENT! <br />
	 * @tparam T
	 * @param soluteName
	 * @return Diffuse(..., this->GetId())
	 */
	template < typename T >
	T operator[](const Name& soluteName)
	{
		return ChemicalCast< T >(Diffuse(soluteName, GetId()));
	}
	
	/**
	 * operator wrappers around Diffuse(). <br />
	 * May be called from anywhere. <br />
	 * @param soluteId
	 * @param targetSolvent
	 * @return Diffuse(..., targetSolvent)
	 */
	virtual Solute* operator[](const Id& soluteId, const Id& targetSolvent);

	/**
	 * operator wrappers around Diffuse(). <br />
	 * May be called from anywhere. <br />
	 * @param soluteId
	 * @param targetSolvent
	 * @return Diffuse(..., targetSolvent)
	 */
	virtual const Solute* operator[](const Id& soluteId, const Id& targetSolvent) const;

	/**
	 * operator wrappers around Diffuse(). <br />
	 * May be called from anywhere. <br />
	 * @tparam T
	 * @param soluteId
	 * @param targetSolvent
	 * @return Diffuse(..., targetSolvent)
	 */
	template < typename T >
	T operator[](const Id& soluteId, const Id& targetSolvent)
	{
		return ChemicalCast< T >(Diffuse(soluteId, GetId()));
	}

	/**
	 * operator wrappers around Diffuse(). <br />
	 * May be called from anywhere. <br />
	 * @param soluteName
	 * @param targetSolvent
	 * @return Diffuse(..., targetSolvent)
	 */
	virtual Solute* operator[](const Name& soluteName, const Id& targetSolvent);

	/**
	 * operator wrappers around Diffuse(). <br />
	 * May be called from anywhere. <br />
	 * @param soluteName
	 * @param targetSolvent
	 * @return Diffuse(..., targetSolvent)Diffuse(..., this->GetId())
	 */
	virtual const Solute* operator[](const Name& soluteName, const Id& targetSolvent) const;

	/**
	 * operator wrappers around Diffuse(). <br />
	 * May be called from anywhere. <br />
	 * @tparam T
	 * @param soluteName
	 * @param targetSolvent
	 * @return Diffuse(..., targetSolvent)
	 */
	template < typename T >
	T operator[](const Name& soluteName, const Id& targetSolvent)
	{
		return ChemicalCast< T >(Diffuse(soluteName, GetId()));
	}

	/**
	 * Adds a Solute* to *this and takes ownership of it. <br />
	 * For more information see LinearMotif::AddImplementation(). <br />
	 * @param content
	 * @return the modified content or NULL.
	 */
	virtual Solute* AddImplementation(Solute* content);

	/**
	 * Takes ownership of a Solute and adds it to *this at the indicated position. <br />
	 * See LinearMotif::InsertImplementation() for more info. <br />
	 * @param toAdd what to add. IMPORTANT: This must not already be in a LinearMotif (i.e. create a clone() before adding it to another destination).
	 * @param position determines where in *this the Content is added.
	 * @param optionalPositionArg If a position is specified, the optionalPositionArg is the id of the Content referenced (e.g. BEFORE, MyContentId()).
	 * @param transferSubContents allows all of the Contents within a conflicting Content to be copied into the new Content, before the conflicting Content is deleted (similar to renaming an upper directory while preserving it's contents).
	 * @return Status of addition (e.g. success or failure).
	 */
	virtual Code InsertImplementation(
		Solute* toAdd,
		const Position position = BOTTOM,
		const Id optionalPositionArg = 0, //i.e. invalid.
		const bool transferSubContents = false
	);
};

} //chemical namespace
} //bio namespace
