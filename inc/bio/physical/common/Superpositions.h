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

#include "Types.h"
#include "bio/physical/relativity/TypedPerspective.h"

namespace bio {

//Superposition type declaration needs to be here to avoid circular dependencies with Waves in Types.

/**
 * Superpositions determine how multiple Waves combine. <br />
 * For example, a Superposition could act typically and sum the 2 waves, or it could defy the principle of superposition and establish at least one wave as a soliton. <br />
 * See Wave::Superpose() for more info. <br />
 */
BIO_ID_WITH_TYPED_PERSPECTIVE(Superposition, uint8_t)

namespace superposition {

/**
 * When combined with another Wave, don't change anything, including the other Wave (i.e. make the Wave a soliton). <br />
 * This is default. <br />
 */
Superposition Noninterfering();

/**
 * Refer to sub-interference. <br />
 */
Superposition Complex();

/**
 * Ignore all but the last written Wave, according to it's Symmetry. <br />
 * This is currently not implemented, since Spin() should both update and return a Wave's Symmetry simultaneously. <br />
 */
Superposition LastToWrite();

/**
 * Ignore all but the first written Wave. <br />
 * This is winner-take-all. <br />
 * This is currently not implemented, since Spin() should both update and return a Wave's Symmetry simultaneously. <br />
 */
Superposition FirstToWrite();

/**
 * Average all Waves. <br />
 */
Superposition Average();

/**
 * Take only the highest of all Waves. <br />
 */
Superposition Highest();

/**
 * Take only the lowest of all Waves. <br />
 */
Superposition Lowest();

/**
 * Sum all Waves. <br />
 */
Superposition Sum();

/**
 * Boolean AND. <br />
 */
Superposition And();

/**
 * Boolean OR. <br />
 */
Superposition Or();

/**
 * Boolean XOR. <br />
 */
Superposition Xor();

/**
 * Boolean NOT. <br />
 */
Superposition Not();

/**
 * Boolean NAND. <br />
 */
Superposition Nand();

/**
 * Boolean NOR. <br />
 */
Superposition Nor();

/**
 * Boolean XNOR. <br />
 */
Superposition Xnor();

} //superposition namespace
} //bio namespace
