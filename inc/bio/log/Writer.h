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

#include "bio/common/VirtualBase.h"
#include "bio/physical/Filterable.h"
#include "bio/physical/common/Filters.h"
#include "bio/log/common/Types.h"
#include "bio/log/common/LogLevels.h"
#include "bio/log/macro/Macros.h"
#include <cstdarg>

namespace bio {
namespace log {

class Engine;

/**
 * A Writer is a base class for any other object that can write to a log engine. <br />
 * The reason for Writer to be a base class, rather than using a singleton log engine is that some objects may wish to write logs to different files than others or, perhaps some logs should be seen only in console output while others should be saved to a file. <br />
 * This also allows an easy drop-in replacement of log::Engine for specific platforms and apps. <br />
 * The above is accomplished by passing log::Engine as a pointer to all LoggerObjects. <br />
 */
class Writer :
	public physical::Class< Writer >,
	virtual public physical::Filterable
{
public:

	/**
	 * Ensure virtual methods point to Class implementations. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(physical, Writer)

	/**
	 *
	 */
	Writer();

	/**
	 * @param logFilter
	 */
	Writer(Filter logFilter);

	/**
	 *
	 */
	virtual ~Writer();

	/**
	 * @return this
	 */
	virtual const Writer* AsLogWriter() const;

	/**
	 * Easy log method. <br />
	 * Uses the mFilter stored in *this. <br />
	 * @param level
	 * @param format
	 * @param args
	 */
	virtual void Log(
		LogLevel level,
		const char* format,
		va_list args
	) const;

	/**
	 * Easy log method. <br />
	 * Uses the mFilter stored in *this. <br />
	 * @param level
	 * @param format
	 * @param ...
	 */
	static void Log(
		const Writer* writer,
		LogLevel level,
		const char* format,
		...
	);

};
} //log namespace
} //bio namespace
