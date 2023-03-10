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

#include "bio/chemical/solution/Solvent.h"
#include "bio/circulatory/common/Types.h"

namespace bio {
namespace circulatory {

/**
 * Blood holds all Solutes which need to be moved through the Circulatory system. This includes both imports and exports to local Blood Vessels. <br />
 * All Blood Solvents are different until they are homogenized by the Heart. <br />
 * Currently, Blood is simply a ThreadSafe Solvent. <br />
 * This class may be expanded later as specific Circulatory requirements are expanded. <br />
 */
class Blood :
	public chemical::Solvent,
	public ThreadSafe
{
public:
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS_FOR(chemical, Blood)

	BIO_DEFAULT_IDENTIFIABLE_CONSTRUCTORS_WITH_COMMON_CONSTRUCTOR(
		chemical,
		Blood,
		&BloodPerspective::Instance(),
		filter::Circulatory
	)
};

} //circulatory namespace
} //bio namespace