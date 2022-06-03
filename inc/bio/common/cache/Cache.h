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
#include "bio/common/macro/Macros.h"

namespace bio {

class AbstractCached;

/**
 * A Cache is a register of Cached objects. <br />
 * This class provides an easy to use interface for accessing cached variables. <br />
 * <br />
 * When to use: <br />
 * For the most part, you will never need to use *this directly. <br />
 * The one exception is if you are creating a save / load system and need to Clear the Cache after changing the underlying values. <br />
 * <br />
 * What to cache: <br />
 * The Biology library make heavy use of Name <-> Id pairings. Ids are faster; names are more robust. <br />
 * Any kind of speed trade off through pairing is a candidate for caching. <br />
 */
class Cache :
	public Arrangement< AbstractCached* >
{
public:

	/**
	 *
	 */
	Cache();

	/**
	 *
	 */
	virtual ~Cache();

	/**
	 * Flushes all Cached objects, causing them to be looked up again. <br />
	 */
	virtual void Flush();
};

BIO_SINGLETON(GlobalCache,
	Cache)

} //bio namespace
