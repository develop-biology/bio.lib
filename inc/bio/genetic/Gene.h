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

#include "bio/genetic/common/Types.h"
#include "bio/genetic/common/Class.h"
#include "bio/genetic/common/Filters.h"
#include "bio/genetic/macros/Macros.h"
#include "bio/genetic/localization/Insertion.h"
#include "bio/molecular/DNA.h"

namespace bio {
namespace genetic {

/**
 * Genes are responsible for creating Proteins in cells. Genes do this by recognizing TranscriptionFactors that the Cells express and cloning a Protein into the given m_insertion.
 *
 * If a Cell has the indicated TranscriptionFactors, insert the given protein at the insertion.
 * For more info, see Expressor and localization/*.h
 *
 * NOTE: for simplicity, Genes are considered mRNA.
 * There is currently no need to create a new object to more closely mimic transcription.
 *
 * Genes are treated more like a struct than a Class.
 * Members should be accessed directly.
 */
class Gene :
	public Class< Gene >,
	public molecular::DNA,
	public chemical::UnorderedMotif< TranscriptionFactor >
{
public:
	/**
	 * Ensure virtual methods point to Class implementations.
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(genetic,
		Gene)

	/**
	 * Standard ctors.
	 * These are easy to use but require setting member variables manually.
	 */
	BIO_DEFAULT_IDENTIFIABLE_CONSTRUCTORS_WITH_CTOR_COMMON(genetic,
		Gene,
		&molecular::DNAPerspective::Instance(),
		filter::Genetic())

	/**
	 *
	 */
	virtual ~Gene();

	/**
	 * The insertion is the Name and type of Site that corresponds to the Vesicle, Molecule, Surface, etc. that *this should to be added to.
	 * This is a functional equivalent to proteins being moved around a cell after being folded. The resulting sub-cellular localization puts certain proteins in closer proximity to each other such that they might affect (or, in this case, effect) each other's activity.
	 * This also enables protein encapsulation, excretion, etc. by making the Proteins in a Vesicle stay in the Vesicle as it's Transferred between other Vesicles, etc.
	 * For more info, see Localization.h and Insertion.h
	 */
	Insertion m_insertion;

private:
	/**
	 * common constructor code
	 */
	void CtorCommon();
};

} //genetic namespace
} //bio namespace
