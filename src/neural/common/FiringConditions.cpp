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

#include "bio/neural/common/FiringConditions.h"
#include "bio/neural/macro/Macros.h"

namespace bio {
namespace neural {
namespace firing_condition {

BIO_FIRING_CONDITION_FUNCTION_BODY(None)

BIO_FIRING_CONDITION_FUNCTION_BODY(Any)

BIO_FIRING_CONDITION_FUNCTION_BODY(RisingEdge)

BIO_FIRING_CONDITION_FUNCTION_BODY(Continuing)

BIO_FIRING_CONDITION_FUNCTION_BODY(FallingEdge)

} //firing_condition namespace
} //neural namespace
} //bio namespace
