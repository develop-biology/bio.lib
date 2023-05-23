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

#include "bio/physical/common/Superpositions.h"
#include "bio/physical/macro/Macros.h"

namespace bio {
namespace superposition {

BIO_SUPERPOSITION_FUNCTION_BODY(Noninterfering)

BIO_SUPERPOSITION_FUNCTION_BODY(Complex)

BIO_SUPERPOSITION_FUNCTION_BODY(LastToWrite)

BIO_SUPERPOSITION_FUNCTION_BODY(FirstToWrite)

BIO_SUPERPOSITION_FUNCTION_BODY(Average)

BIO_SUPERPOSITION_FUNCTION_BODY(Highest)

BIO_SUPERPOSITION_FUNCTION_BODY(Lowest)

BIO_SUPERPOSITION_FUNCTION_BODY(Sum)

BIO_SUPERPOSITION_FUNCTION_BODY(Difference)

BIO_SUPERPOSITION_FUNCTION_BODY(And)

BIO_SUPERPOSITION_FUNCTION_BODY(Or)

BIO_SUPERPOSITION_FUNCTION_BODY(Xor)

BIO_SUPERPOSITION_FUNCTION_BODY(Not)

BIO_SUPERPOSITION_FUNCTION_BODY(Nand)

BIO_SUPERPOSITION_FUNCTION_BODY(Nor)

BIO_SUPERPOSITION_FUNCTION_BODY(Xnor)

} //superposition namespace
} //bio namespace
