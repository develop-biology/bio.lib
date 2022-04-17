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

/**
 * WARNING: THIS FILE USES C++11 for cross platform time management. <br />
 * TODO: add C++98 compatibility for popular operating systems. <br />
*/

// #define BIO_FAKE_SYSTEM_TIME //DEVELOPMENT ONLY!!!

#include "bio/physical/common/Types.h"

namespace bio {
namespace physical {

#ifdef BIO_FAKE_SYSTEM_TIME
/**  <br />
 * Because mocking global functions is such a pain, this method has been provided FOR TESTING PURPOSES ONLY <br />
 * Use of this method requires that the bio library be compiled with BIO_FAKE_SYSTEM_TIME <br />
 * @param newTime the time that will be returned by GetCurrentTimestamp().
 */
void SetFakeTime(const Timestamp newTime);
#endif

/**
 * Uses std::chrono to return the time since epoch. <br />
 * @return the current time as a Timestamp.
 */
Timestamp GetCurrentTimestamp();

} //physical namespace
} //bio namespace
