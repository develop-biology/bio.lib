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

namespace bio {

/**
 * AbstractCached is a base class for all Cached objects.  <br />
 * NOTE: *this will Register on construction and Deregister on destruction. Because these functions are called in ?tors, they cannot be virtual. This may change in a future release.
 */
class AbstractCached
{
public:
	/**
	 *
	 */
	AbstractCached();

	/**
	 *
	 */
	virtual ~AbstractCached();

	/**
	 * Remove whatever *this has cached and re-look up the newest value.
	 */
	virtual void Flush()
	{
		//nop
	}

	/**
	 * Register *this with the global Cache
	 */
	void Register();

	/**
	 * Remove *this from the global Cache's register.
	 */
	void Deregister();
};


} //bio namespace
