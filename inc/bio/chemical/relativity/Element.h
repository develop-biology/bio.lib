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

#include "bio/common/container/Arrangement.h"
#include "bio/chemical/common/Types.h"
#include "bio/physical/string/TypedBrane.h"

namespace bio {
namespace chemical {

/**
 * Elements track the Properties and types of items in the PeriodicTable. <br />
 * Elements are Branes as they are the fundamental constituents of the PeriodicTable. <br />
 * NOTE: this is not chemical::Elementary<T>, which is used to define these Elements. <br />
 */
class Element :
	public physical::TypedBrane< AtomicNumber >
{
public:
	/**
	 * @param id
	 * @param name
	 */
	Element(
		const AtomicNumber& id,
		const Name& name
	)
		:
		physical::TypedBrane< AtomicNumber >(
			id,
			name,
			NULL
		)
	{
	}

	/**
	 * @param id
	 * @param name
	 * @param properties
	 */
	Element(
		const AtomicNumber& id,
		const Name& name,
		Properties* properties
	)
		:
		physical::TypedBrane< AtomicNumber >(
			id,
			name,
			NULL
		),
		mProperties(properties)
	{
	}

	/**
	 *
	 */
	virtual ~Element()
	{

	}

	/**
	 * Optional property storage. <br />
	 * See Elemental.h and PeriodicTable.h for more info. <br />
	 */
	Arrangement< Property > mProperties;
};

} //chemical namespace
} //bio namespace
