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

#include "bio/cellular/common/Class.h"
#include "bio/cellular/common/Types.h"
#include "bio/cellular/common/Filters.h"
#include "bio/cellular/macro/Macros.h"
#include "bio/genetic/Expressor.h"
#include "bio/genetic/Plasmid.h"
#include "Organelle.h"

namespace bio {

namespace cellular {
class Tissue;
}

namespace cellular {

/**
 * A Cell is the basic unit of function-driven organization within Biology. <br />
 * Cells use Proteins & Organelles to accomplish tasks. You can think of each Protein as a stand-in for a class method except, instead of hard-coding your classes, you instead code in (hard or soft) the TranscriptionFactors and Plasmids present in a Cell. The Cell then determines its functionality at runtime. <br />
 *
 * In order to simplify the arbitrarily complex behavior that a Cell can perform, Cells are made to Crest, allowing their main function to be called on a clock at a regular interval. <br />
 * Programming a Cell this way is similar to programming an Arduino with a main loop. <br />
 * Of course, you are allowed to modify this behavior in any way you'd like ;) <br />
 */
class Cell :
	public cellular::Class< Cell >,
	public Metallic< chemical::DependentMotif< Organelle*, Cell* > >,
	public chemical::EnvironmentDependent< cellular::Tissue* >
{
public:

	/**
	 * Ensure virtual methods point to Class implementations. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(cellular, Cell)

	/**
	 * Standard constructors. <br />
	 */
 	BIO_DEFAULT_IDENTIFIABLE_CONSTRUCTORS(
		cellular,
		Cell,
		filter::Cellular()
	)

	/**
	 *
	 */
	virtual ~Cell();

	/**
	 * First, injects all Plasmids in *this into each Organelle in *this. <br />
	 * Then calls ExpressGenes() on each Organelle in *this. <br />
	 * Then Transcribes all Genes from all Plasmids in *this, iff *this has the necessary TranscriptionFactors for each Gene, populating mTranscriptome. <br />
	 * Then, Translates all mRNA from the mTranscriptome into Proteins. <br />
	 * @return whether or not *this should be functional.
	 */
	virtual Code ExpressGenes();

	/**
	 * Traverse up the Environment hierarchy to see if the given Tissue is anywhere above *this. <br />
	 * @param tissueId
	 * @return whether or not the given Tissue contains *this or another Tissue which does. <br />
	 */
	virtual bool IsWithinTissue(const Id& tissueId) const;

	/**
	 * Traverse up the Environment hierarchy to see if the given Tissue is anywhere above *this. <br />
	 * @param name
	 * @return whether or not the given Tissue contains *this or another Tissue which does. <br />
	 */
	virtual bool IsWithinTissue(const Name& name) const;
};

} //cellular namespace
} //bio namespace
