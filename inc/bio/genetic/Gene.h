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

#include "Types.h"
#include "bio/cellular/Types.h"
#include "bio/molecular/DNA.h"

namespace bio {
namespace genetic {

/**
 * Genes are responsible for creating Proteins in cells. Genes do this by recognizing TranscriptionFactors that the Cells express and cloning a Protein into the given m_localization.
 *
 * NOTE: for simplicity, Genes are considered mRNA.
 * There is currently no need to create a new object to more closely mimic Transcription.
 *
 * Genes are treated more like a struct than a Class.
 * Members should be accessed directly.
 */
class Gene :
	molecular::DNA
{
public:
	/**
	 * @param requiredTranscriptionFactors
	 * @param protein
	 * @param localization
	 * @param position
	 * @param optionalInsertionArg
	 * @param transferSubProteins
	 */
	Gene(const TranscriptionFactors& requiredTranscriptionFactors,
		cellular::Protein* protein,
		StandardDimensions localization,
		Position position = BOTTOM,
		const StandardDimension optionalInsertionArg = 0,
		bool transferSubProteins = false)

	/**
	 *
	 */
	virtual ~Gene();

	/**
	 * If a Cell has the indicated TranscriptionFactors, insert the given protein at the localization (i.e. Cell::InsertProtein(protein, localization).
	 */
	TranscriptionFactors m_requiredTranscriptionFactors;


	/**
	 * The localization is a vector of Protein ids that corresponds to the tree that would be present in whatever Cell this is being applied to. Index 0 in this vector is the topLevelProtein, with the index number increasing for each level down in the tree. Index end()-1 is what the protein will be added to.
	 * This is a functional equivalent to proteins being moved around the cell after being folded. The resulting sub-cellular localization puts certain proteins in closer proximity to each other such that they might affect (or, in this case, effect) each other's activity.
	 * See Protein::operator() for more info on how sub-proteins are handled.
	 */
	StandardDimensions m_localization;

	/**
	 * If there are sub-proteins, should this be added to the top, bottom, etc.
	 */
	Position m_position;
	StandardDimension m_optionalInsertionParameter;

	/**
	 * Copy existing sub proteins when disabling and removing a conflicting protein.
	 */
	bool m_transferSubProteins;
};

} //cellular namespace
} //bio namespace
