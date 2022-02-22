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

#include "bio/chemical/common/Types.h"

namespace bio {
namespace molecular {

/**
 * Versioning is major.minor with no patch.
 * Makes things easy ;)
 * Used by DNA.
 */
typedef float Version;

BIO_PERSPECTIVE_SINGLETON(VesiclePerspective,
	StandardDimension
)

BIO_PERSPECTIVE_SINGLETON(MoleculePerspective,
	StandardDimension
)

BIO_PERSPECTIVE_SINGLETON(DNAPerspective,
	StandardDimension
)

BIO_PERSPECTIVE_SINGLETON(ProteinPerspective,
	StandardDimension)

} //molecular namespace
} //bio namespace
