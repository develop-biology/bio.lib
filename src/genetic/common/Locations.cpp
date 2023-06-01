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

#include "bio/genetic/common/Locations.h"
#include "bio/genetic/macro/Macros.h"
#include "bio/genetic/relativity/Translocator.h"
#include "bio/molecular/Surface.h"
#include "bio/molecular/Molecule.h"
#include "bio/molecular/Vesicle.h"
#include "bio/chemical/reaction/Excitation.h"
#include "bio/chemical/structure/motif/LinearMotif.h"
#include "bio/physical/Periodic.h"

namespace bio {

BIO_LOCATION_FUNCTION_BODY(
	InSurface,
	molecular::Molecule*)

BIO_LOCATION_FUNCTION_BODY(
	OnMolecule,
	molecular::Surface*)

BIO_LOCATION_FUNCTION_BODY(
	InVesicle,
	molecular::Molecule*)

} //bio namespace
