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

#include "Types.h"

namespace bio {
namespace miscibility {

/**
 * Ignore all but the last written Miscible Symmetry. <br />
 */
Miscibility LastToWrite();

/**
 * Ignore all but the first written Miscible Symmetry. <br />
 * You probably don't want this, and should use LastToWrite() instead. <br />
 */
Miscibility FirstToWrite();

/**
 * Average all Miscible Symmetries. <br />
 */
Miscibility Average();

/**
 * Take only the highest of all Miscible Symmetries. <br />
 */
Miscibility Highest();

/**
 * Take only the lowest of all Miscible Symmetries. <br />
 */
Miscibility Lowest();

} //miscibility namespace
} //bio namespace
