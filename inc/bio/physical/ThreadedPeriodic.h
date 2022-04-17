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

#include "bio/common/Threaded.h"
#include "Periodic.h"

namespace bio {
namespace physical {

/**
 * A ThreadedPeriodic is simply an Periodic that vibrates in its own thread. <br />
 * See Periodic.h & Threaded.h for more info. <br />
 */
class ThreadedPeriodic :
	virtual public Periodic,
	virtual public Threaded,
	physical::Class< ThreadedPeriodic >
{
public:

	/**
	 * Ensure virtual methods point to Class implementations. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(physical,
		ThreadedPeriodic)

	/**
	 *
	 */
	ThreadedPeriodic(TimeUS interval = GetDefaultInterval());

	/**
	 *
	 */
	virtual ~ThreadedPeriodic();

	/**
	 * Calls Peak() then sleeps. <br />
	 */
	virtual bool Work();
};

} //physical namespace
} //bio namespace
