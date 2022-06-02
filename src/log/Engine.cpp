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

#include "bio/log/Engine.h"
#include "bio/log/common/LogLevels.h"
#include "bio/common/macros/Macros.h"
#include "bio/common/macros/OSMacros.h"
#include "bio/physical/common/Types.h"
#include "bio/physical/Time.h"
#include <cstring>
#include <stdarg.h>
#include <ios>
#include <sstream>
#include <cstdio>

namespace bio {
namespace log {

Engine::Engine()
{
	//Set all filters to only log if level is >= Info
	mLevelFilter.assign(
		SafelyAccess<FilterPerspective>()->GetNumUsedIds(),
		log_level::Info());
}

Engine::~Engine()
{

}

void Engine::Log(
	Filter filter,
	LogLevel level,
	const char* format,
	va_list args
)
{
	if (FilterPass(
		filter,
		level
	))
	{
		return; //filter off, don't log
	}

	char str[BIO_LOG_PRINTF_MAX_LINE_SIZE + 1];

	vsnprintf(
		str,
		sizeof(str),
		format,
		args
	);
	va_end(args);
	str[BIO_LOG_PRINTF_MAX_LINE_SIZE] = '\0';

	mLogMessage.clear();
	mLogMessage.str(""); //TODO: is seekp good enough? what is faster?

	mLogMessage << physical::GetCurrentTimestamp() << " " << SafelyAccess<FilterPerspective>()->GetNameFromId(filter) << " " << SafelyAccess<LogLevelPerspective>()->GetNameFromId(level) << ": " << str << "\n";
	Output(mLogMessage.str());
}

void Engine::Log(
	Filter filter,
	LogLevel level,
	const char* format,
	...
)
{
	if (FilterPass(
		filter,
		level
	))
	{
		return; //filter off, don't log
	}

	va_list args;
	va_start(args,
		format);
	Log(
		filter,
		level,
		format,
		args
	);
	va_end(args);
}

bool Engine::FilterPass(
	Filter filter,
	LogLevel level
) const
{
	if (filter == filter::All())
	{
		return false;
	}
	return level >= mLevelFilter[filter];
}

bool Engine::SetFilter(
	Filter filter,
	LogLevel level
)
{
	if (filter == filter::All())
	{
		mLevelFilter.assign(
			SafelyAccess<FilterPerspective>()->GetNumUsedIds(),
			level);
	}
	else
	{
		mLevelFilter[filter] = level;
	}
	return true; //SUCCESS
}

bool Engine::SetFilter(
	const Name& filter,
	const Name& level
)
{
	return SetFilter(
		SafelyAccess<FilterPerspective>()->GetIdFromName(filter),
		SafelyAccess<LogLevelPerspective>()->GetIdFromName(level));
}

LogLevel Engine::GetFilter(Filter filter) const
{
	return mLevelFilter[filter];
}

} //log namespace
} //bio namespace
