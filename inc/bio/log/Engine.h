/*
 * This file is a part of the Biology project by eons LLC.
 * Biology (aka Develop Biology) is a framework for approaching software
 * development from a natural sciences perspective.
 *
 * Copyright (C) 2021 Séon O'Shannon & eons LLC
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

#include "bio/physical/Filters.h"
#include "Macros.h"
#include <sstream>

namespace bio {
namespace log {

/**
 * log::Engines are responsible for printing logs.
 * Create your own Engine, if you have your own output schema you'd like to use (e.g. to the screen of a gui instead of to a file or standard out).
 */
class Engine
{
public:

	/**
	 *
	 */
	Engine();

	/**
	 *
	 */
	virtual ~Engine();

	/**
	 * Required override for outputting logs.
	 * logString will include a newline (\n) character.
	 * @param logString fully prepared text to be logged.
	 */
	virtual void Output(const std::string& logString) = 0;

	/**
	 * Generates a log string and calls Output
	 * (caller is responsible for using filters).
	 * @param filter
	 * @param level
	 * @param format
	 * @param args
	 */
	void Log(
		Filter filter,
		Level level,
		const char* format,
		va_list args);

	/**
	 * Create a log string that will be Output().
	 * *this can be invoked directly or via Writer::ExternalLog Both calls will have the same effect.
	 * @param filter
	 * @param level
	 * @param format
	 * @param ...
	 */
	void Log(
		Filter filter,
		Level level,
		const char* format,
		...);

	/**
	 * @param filter
	 * @param level
	 * @return true if level is enabled for the filter, false if disabled; false if filter is ALL or out of range; false if level is out of range.
	 */
	bool FilterPass(
		Filter filter,
		Level level) const;

	/**
	 * Change filter level for the filter
	 * @param filter what to affect; use ALL to set the level of all filters.
	 * @param level value to set for the given filter (Log() calls must be >= to this to be seen).
	 * @return true on success, false otherwise.
	 */
	bool FilterSet(
		Filter filter,
		Level level);

	/**
	 * Change filter level for the filter
	 * @param filter what to affect; use ALL to set the level of all filters.
	 * @param level value to set for the given filter (Log() calls must be >= to this to be seen).
	 * @return true on success, false otherwise.
	 */
	bool FilterSet(
		Name filter,
		Name level); //calls StringTo...

	/**
	 * @param filter
	 * @return Current level for the given filter.
	 */
	Level FilterGet(Filter filter) const;

protected:
	/**
	 * This should not need to be accessed from children, as it is passed to Output but is here for convenience.
	 * TODO: see if there is a faster data type for logging.
	 */
	std::ostringstream m_logMessage;

private:
	/**
	 * Contains enabled level for every filter
	 * Only log if level is >= value loaded in this vector
	 * the index in the vector is the Filter
	 */
	std::vector<Level> m_levelFilter;
};
} //log namespace
} //bio namespace
