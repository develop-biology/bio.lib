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

#include "bio/chemical/common/SymmetryTypes.h"
#include "bio/physical/macros/Macros.h"


namespace bio {
namespace symmetry_type {

BIO_SYMMETRY_TYPE_FUNCTION_BODY(Object);

BIO_SYMMETRY_TYPE_FUNCTION_BODY(StartObject);

BIO_SYMMETRY_TYPE_FUNCTION_BODY(EndObject);

BIO_SYMMETRY_TYPE_FUNCTION_BODY(StartList);

BIO_SYMMETRY_TYPE_FUNCTION_BODY(EndList);

BIO_SYMMETRY_TYPE_FUNCTION_BODY(StartGroup);

BIO_SYMMETRY_TYPE_FUNCTION_BODY(EndGroup);

BIO_SYMMETRY_TYPE_FUNCTION_BODY(StartScope);

BIO_SYMMETRY_TYPE_FUNCTION_BODY(EndScope);

BIO_SYMMETRY_TYPE_FUNCTION_BODY(ReferenceVariable);

BIO_SYMMETRY_TYPE_FUNCTION_BODY(Operation);

} //symmetry_type namespace
} //bio namespace
