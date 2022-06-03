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

#include "ByteStream.h"
#include "Primitives.h"
#include "bio/common/string/String.h"
#include "bio/common/macro/Macros.h"
#include "bio/common/container/Arrangement.h"
#include "bio/common/container/SmartIterator.h"
#include "bio/common/thread/SafelyAccess.h"
#include "bio/common/thread/ThreadSafe.h"

namespace bio {

/**
 * Arrangements of Iterators can be used in buffers, etc.
 */
typedef Arrangement< SmartIterator > SmartIterators;

/**
 * Names are used in NameTracker<>s but are defined here for simplicity. <br />
 */
typedef String Name;
typedef Arrangement< Name > Names;

typedef Container ByteStreams; //ByteStream is the default TYPE, so no need to re-specify.

/**
 * Timestamp. Usually in ms since epoch (epoch is sometimes the uptime of the kernel) <br />
 */
typedef uint32_t Timestamp;
typedef Arrangement< Timestamp > Timestamps;

/**
 * microseconds (us for short). <br />
 */
typedef uint32_t MicroSeconds;
typedef MicroSeconds US;

/**
 * milliseconds (ms for short). <br />
 */
typedef uint32_t MilliSeconds;
typedef MilliSeconds MS;

/**
 * seconds (s or sec for short). <br />
 */
typedef uint32_t Seconds;
typedef Seconds S;

} //bio namespace
