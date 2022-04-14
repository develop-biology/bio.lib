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
#include "bio/physical/macros/Macros.h"
#include "bio/physical/Perspective.h"

namespace bio {

/**
 * The BondType of an Atom::Bond provides a simple description of the nature of the Bond.
 * This can be used to inform other systems (e.g. molecular) of how to treat the Bonded Wave.
 * NOTE: Atoms can Bond things that are not Atoms. The only requirement is that they are Wave.
 */
BIO_STRONG_TYPEDEF(uint8_t,
	BondType,
	0)

BIO_PERSPECTIVE_SINGLETON(BondTypePerspective,
	BondType)

/**
 * Mainly used for inserting children into Nested classes.
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
 * Valence defines how many Bonds an Atom (e.g. Substance) can have.
 * The max of valence is essentially the max number of parents a child class can have.
 * Valence can be any size up to the size of Index (used by physical::Arrangements).
 * For now, we set Valence to the max possible size and may optimize later.
 */
typedef Index Valence;

/**
 * The AtomicNumber tracks class definitions.
 * The StandardDimension can be used to track object configurations as well class definitions, so there is some overlap. However, the AtomicNumber should only ever have 1 Perspective: the PeriodicTable.
 * See PeriodicTable.h for more info
 */
typedef uint16_t AtomicNumber;

BIO_PERSPECTIVE_SINGLETON(ReactionPerspective,
	StandardDimension)

class Substance;

typedef std::vector< Substance* > Substances;

BIO_PERSPECTIVE_SINGLETON(SubstancePerspective,
	StandardDimension)

typedef std::vector< ByteStream > Emission;

} //chemical namespace
} //bio namespace
