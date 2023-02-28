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

#include "bio/physical/common/Types.h"
#include "bio/physical/macro/Macros.h"
#include "bio/physical/relativity/Perspective.h"

namespace bio {

/**
 * The BondType of an Atom::Bond provides a simple description of the nature of the Bond. <br />
 * This can be used to inform other systems (e.g. molecular) of how to treat the Bonded Wave. <br />
 * NOTE: Atoms can Bond things that are not Atoms. The only requirement is that they are Wave. <br />
 */
BIO_STRONG_TYPEDEF(uint8_t,	BondType, 0)

BIO_PERSPECTIVE_SINGLETON(BondTypePerspective, BondType)

/**
 * Miscibility is the ability of 2 Substances (usually Solutes) to Mix. <br />
 * Here, we refer to "Miscibility" as a noun meaning "the strategy by which 2 Substances are mixed"; however, in practice, we apply a set of Miscibilities to the set of chemical::Symmetries the Substance possesses and not to the Substances themselves. Thus, Miscibilities are atomic and applied to a recursive structure. <br />
 * See Miscibilities.h for examples. <br />
 */
BIO_ID_WITH_PERSPECTIVE_WITH_PLURAL(Miscibility, Miscibilities, uint8_t)

/**
 * Diffusion Time and Effort determine when and which Solutes are Mixed. <br />
 * See Solute.h for more info. <br />
 */
BIO_ID_WITH_PERSPECTIVE(DiffusionTime, uint8_t)
BIO_ID_WITH_PERSPECTIVE(DiffusionEffort, uint8_t)

/**
 * Mainly used for inserting children into Nested classes. <br />
 */
typedef enum
{
	TOP = 0,
	BOTTOM,
	AFTER,
	BEFORE,
	INSERTION_POSITION_TOTAL
} Position;

} //bio namespace

namespace bio {
namespace chemical {

/**
 * Valence defines how many Bonds an Atom (e.g. Substance) can have. <br />
 * The max of valence is essentially the max number of parents a child class can have. <br />
 * Valence can be any size up to the size of Index (used by  Arrangements). <br />
 * For now, we set Valence to the max possible size and may optimize later. <br />
 */
typedef Index Valence;

/**
 * The AtomicNumber tracks class definitions. <br />
 * The Id can be used to track object configurations as well class definitions, so there is some overlap. However, the AtomicNumber should only ever have 1 Perspective: the PeriodicTable. <br />
 * See PeriodicTable.h for more info <br />
 */
typedef uint16_t AtomicNumber;

/**
 * Chemical Concentrations are expressions of fractional quantity and are used by classes like Solute. <br />
 */
typedef uint16_t Concentration;

class Substance;
typedef ::bio::Arrangement< Substance* > Substances;

/**
 * Emissions are given off (returned) by Excitations.
 */
typedef ::bio::Arrangement< ByteStream > Emission;

} //chemical namespace
} //bio namespace
