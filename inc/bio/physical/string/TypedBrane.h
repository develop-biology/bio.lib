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

#include "Brane.h"

namespace bio {
namespace physical {

class Wave;

/**
 * Adds a Wave* type to Perspective< DIMENSION >::Brane. <br />
 * @tparam DIMENSION
 */
template< typename DIMENSION >
class TypedBrane : public Brane< DIMENSION >
{
public:
	TypedBrane(
		const DIMENSION& id,
		const Name& name,
		Wave* type = NULL
	)
		:
		Brane< DIMENSION >(id, name),
		mType(type)
	{
	}

	virtual ~TypedBrane()
	{
		if (mType)
		{
			delete mType;
			mType = NULL;
		}
	}

	Wave* mType;
};

} //physical namespace
} //bio namespace
