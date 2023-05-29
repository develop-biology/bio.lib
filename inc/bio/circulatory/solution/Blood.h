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

#include "bio/chemical/solution/Solution.h"
#include "bio/circulatory/common/Types.h"
#include "bio/circulatory/macro/Macros.h"
#include "bio/circulatory/common/Filters.h"

namespace bio {
namespace circulatory {

/**
 * Blood holds all Solutes which need to be moved through the Circulatory system. This includes both imports and exports to local Blood Vessels. <br />
 * All Blood Solutions are different until they are homogenized by the Heart. <br />
 * Currently, Blood is simply a ThreadSafe Solution. <br />
 * This class may be expanded later as specific Circulatory requirements are expanded. <br />
 */
class Blood:
	public chemical::Class< Blood >,
	public chemical::Solution
{
public:
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(chemical, Blood)

	BIO_DEFAULT_IDENTIFIABLE_CONSTRUCTORS(
		chemical,
		Blood,
		filter::Circulatory()
	)

	/**
	 *
	 */
	virtual ~Blood();
};

} //circulatory namespace
} //bio namespace
