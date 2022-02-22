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

#include "bio/common/Types.h"
#include "bio/phyisical/Periodic.h"
#include "bio/chemical/structure/LinearStructuralComponent.h"
#include "bio/log/Writer.h"
#include "bio/genetic/Expressor.h"


namespace bio {

namespace visceral {
class Tissue;
}

namespace molecular {
class Protein;
}

namespace cellular {

/**
 * A Cell is the basic unit of function-driven organization within Biology.
 * Cells use Proteins to accomplish tasks. You can think of each Protein as a stand-in for a class method except, instead of hard-coding your classes, you instead code in (hard or soft) the TranscriptionFactors and Plasmids present in a Cell. The Cell then determines its functionality at runtime.
 * NOTE: While Cells are Vesicles, they do not register with the VesiclePerspective; instead, they can be Identifiable by the CellPerspective.
 *
 * In order to simplify the arbitrarily complex behavior that a Cell can perform, Cells are made to Peak, allowing their main function to be called on a clock at a regular interval.
 * Programming a Cell this way is similar to programming an Arduino.
 * Of course, you are allowed to modify this behavior in any way you'd like ;)
 */
class Cell :
	virtual public genetic::Expressor,
	public chemical::LinearStructuralComponent< Organelle** >,
	public molecular::EnvironmentDependent<Tissue>,
{
public:

	/**
	 *
	 */
	Cell(Name name = "cell");

	/**
	 * Copies all TranscriptionFactors & DNA in *this, giving a copy to the progeny.
	 * THIS DOES NOT PRODUCE A FUNCTIONAL CELL!
	 * The Progeny must Transcribe then Translate all Genes & Fold the resulting Proteins.
	 * Proteins are not copied nor produced here in order to accommodate novel differentiation between the time of copying and the time of becoming functional.
	 * For example, you could copy a Cell, then add or remove TranscriptionFactors in order to change the behavior of the Cell.
	 * This DOES copy the m_environment by assuming the new Cell exists in the same Tissue as its progenitor. If this is not the case, you can always set the new Tissue with MoveTo().
	 */
	Cell(const Cell& toCopy);

	/**
	 *
	 */
	virtual ~Cell();

};

} //cellular namespace
} //bio namespace
