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
#include "bio/log/common/Levels.h"
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
	m_levelFilter.assign(
		FilterPerspective::Instance().GetNumUsedIds(),
		level::Info());
}

Engine::~Engine()
{

}

void Engine::Log(
	Filter filter,
	Level level,
	const char* format, <br />
	va_list args
)
{
	BIO_SANITIZE_AT_SAFETY_LEVEL_2(FilterPass(
		filter,
		level
	), ,
		return);

	char str[BIO_LOG_PRINTF_MAX_LINE_SIZE + 1];

	vsnprintf(
		str,
		sizeof(str),
		format,
		args
	);
	va_end(args);
	str[BIO_LOG_PRINTF_MAX_LINE_SIZE] = '\0';

	m_logMessage.clear();
	m_logMessage.str(""); //TODO: is seekp good enough? what is faster?

	m_logMessage << physical::GetCurrentTimestamp() << " " << FilterPerspective::Instance().GetNameFromId(filter) << " " << LevelPerspective::Instance().GetNameFromId(level) << ": " << str << "\n";
	Output(m_logMessage.str());
}

void Engine::Log(
	Filter filter,
	Level level,
	const char* format, <br />
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
	Level level
) const
{
	return level >= m_levelFilter[filter];
}

bool Engine::FilterSet(
	Filter filter,
	Level level
)
{
	m_levelFilter[filter] = level;
	return true; //SUCCESS
}

bool Engine::FilterSet(
	Name filter,
	Name level
)
{
	return FilterSet(
		FilterPerspective::Instance().GetIdFromName(filter),
		LevelPerspective::Instance().GetIdFromName(level));
}

Level Engine::FilterGet(Filter filter) const
{
	return m_levelFilter[filter];
}

} //log namespace
} //bio namespace
