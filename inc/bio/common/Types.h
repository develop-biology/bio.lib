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

#include <assert.h>
#include "ByteStream.h"
#include "Primitives.h"
#include "bio/common/macros/Macros.h"
#include "bio/common/container/Arrangement.h"
#include "bio/common/container/SmartIterator.h"

namespace bio {

/**
 * Arrangements of Iterators can be used in buffers, etc.
 */
typedef Arrangement< SmartIterator > SmartIterators;

/**
 * Names are used in NameTracker<>s but are defined here for simplicity. <br />
 */
typedef const char* Name; 
typedef Arrangement< Name > Names;

typedef Arrangement< const char* > CharStrings;
typedef Arrangement< ::std::string > StdStrings;

typedef Arrangement< ByteStream > ByteStreams;

/**
 * milliseconds (ms for short) <br />
 */
typedef uint32_t Timestamp;
typedef Arrangement< Timestamp > Timestamps;

/**
 * microseconds (us for short) <br />
 */
typedef uint32_t TimeUS;

/**
 * milliseconds (ms for short) <br />
 */
typedef uint32_t TimeMS;

/**
 * seconds (s or sec for short) <br />
 */
typedef uint32_t TimeSec;

} //bio namespace
