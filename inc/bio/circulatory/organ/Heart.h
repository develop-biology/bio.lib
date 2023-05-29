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

#include "bio/cellular/Organ.h"
#include "bio/circulatory/common/Class.h"
#include "bio/circulatory/common/Types.h"
#include "bio/circulatory/macro/Macros.h"
#include "bio/circulatory/common/Filters.h"

namespace bio {
namespace circulatory {

class Heart :
	public circulatory::Class< Heart >,
	public cellular::Organ
{
public:
	/**
	 * Ensure virtual methods point to Class implementations. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(circulatory, Heart)

	/**
	 * Standard constructors. <br />
	 */
	BIO_DEFAULT_IDENTIFIABLE_CONSTRUCTORS(
		circulatory,
		Heart,
		filter::Circulatory()
	)

	/**
	 *
	 */
	virtual ~Heart();

	/**
	 * Use this method to populate any member variable Protein*s. <br />
	 * You'll want to do this to speed up your code by bypassing the dynamic execution provided by genetic::Expressor. <br />
	 */
	virtual Code CacheProteins();

	/**
	 * If you use CacheProteins, you'll likely want to create your default Proteins here. <br />
	 * This will prevent dereferencing null or garbage pointers when using your cached Proteins. <br />
	 */
	virtual Code CreateDefaultProteins();

};

} // circulatory
} //bio namespace
