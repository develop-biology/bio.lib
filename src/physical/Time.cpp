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

#include "bio/physical/Time.h"
#include "bio/common/macro/LanguageMacros.h"

#if BIO_CPP_VERSION < 11
#else

	#include <chrono> //C++11

#endif

namespace bio {
namespace physical {

#ifdef BIO_FAKE_SYSTEM_TIME
static Timestamp sFakeTime = 0;
void SetFakeTime(const Timestamp newTime)
{
	sFakeTime = newTime;
}
#endif

Timestamp GetCurrentTimestamp()
{
	#ifdef BIO_FAKE_SYSTEM_TIME
	return sFakeTime;
	#elif BIO_CPP_VERSION < 11
	return 0; //TODO...
	#else
	using namespace ::std::chrono;
	auto now = time_point_cast< milliseconds >(system_clock::now());
	return now.time_since_epoch().count();
	#endif
}

} //physical namespace
} //bio namespace
