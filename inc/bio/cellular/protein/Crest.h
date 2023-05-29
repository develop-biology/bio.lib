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

#include "bio/molecular/Protein.h"
#include "bio/cellular/macro/Macros.h"
#include "bio/cellular/common/Filters.h"

namespace bio {
namespace cellular {

class Crest:
	public molecular::Class< Crest >,
	virtual public molecular::Protein
{
public:

	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(molecular, Crest)

	BIO_DEFAULT_IDENTIFIABLE_CONSTRUCTORS(
		molecular,
		Crest,
		filter::Cellular()
	)
	
	/**
	 * 
	 */
	virtual ~Crest();
};

} //cellular namespace
} //bio namespace
