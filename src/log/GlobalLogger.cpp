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

#include "bio/log/GlobalLogger.h"
#include "bio/log/Engine.h"
#include "bio/common/macro/Macros.h"

#include <stdarg.h>

namespace bio {
namespace log {

GlobalLoggerImplementation::GlobalLoggerImplementation()
	:
	physical::Class< GlobalLoggerImplementation >(this),
	mLogEngine(NULL)
{

}

GlobalLoggerImplementation::~GlobalLoggerImplementation()
{
	if (mLogEngine != NULL)
	{
		delete mLogEngine;
		mLogEngine = NULL;
	}
}

void GlobalLoggerImplementation::SetLogEngine(Engine* logEngine)
{
	mLogEngine = logEngine;
}

Engine* GlobalLoggerImplementation::GetLogEngine()
{
	return mLogEngine;
}

const Engine* GlobalLoggerImplementation::GetLogEngine() const
{
	return mLogEngine;
}

bool GlobalLoggerImplementation::HasLogEngine() const
{
	return mLogEngine != NULL;
}

void GlobalLoggerImplementation::Log(
	const Filter& filter,
	const LogLevel& level,
	const char* format,
	...
) const
{
	BIO_SANITIZE(mLogEngine,
		,
		return);

	va_list args;
	va_start(args, format);
	mLogEngine->Log(
		filter,
		level,
		format,
		args
	);
	va_end(args);
}

} //log namespace
} //bio namespace
