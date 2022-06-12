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

#include "bio/log/Writer.h"
#include "bio/log/Engine.h"
#include "bio/common/macro/Macros.h"

#include <stdarg.h>

namespace bio {
namespace log {

Writer::Writer()
	:
	physical::Class< Writer >(this),
	physical::Filterable(filter::Default()),
	mLogEngine(NULL)
{

}

Writer::Writer(
	Engine* logEngine,
	Filter filter
)
	:
	physical::Class< Writer >(this),
	physical::Filterable(filter::Default()),
	mLogEngine(logEngine)
{

}

Writer::~Writer()
{

}

void Writer::SetLogEngine(Engine* logEngine)
{
	mLogEngine = logEngine;
}

Engine* Writer::GetLogEngine()
{
	return mLogEngine;
}

const Engine* Writer::GetLogEngine() const
{
	return mLogEngine;
}

bool Writer::HasLogEngine() const
{
	return mLogEngine != NULL;
}

void Writer::Log(
	LogLevel level,
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
		mFilter,
		level,
		format,
		args
	);
	va_end(args);
}

void Writer::Log(
	LogLevel level,
	const char* format,
	va_list args
) const
{
	BIO_SANITIZE(mLogEngine,
		,
		return);

	mLogEngine->Log(
		mFilter,
		level,
		format,
		args);
}

void Writer::ExternalLog(
	Filter filter,
	LogLevel level,
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

void Writer::ExternalLog(
	Filter filter,
	LogLevel level,
	const char* format,
	va_list args
) const
{
	BIO_SANITIZE(mLogEngine,
		,
		return);

	mLogEngine->Log(
		filter,
		level,
		format,
		args
	);
}

void Writer::InitializeImplementation(ByteStreams& args)
{
	if (args.Size() == 2)
	{
		if (args[args.GetEndIndex()].Is(mLogEngine))
		{
			mLogEngine = args[args.GetEndIndex()];
		}
		args.Erase(args.GetEndIndex());
	}
	if (args.Size() == 1 && args[args.GetEndIndex()].Is< Filter >())
	{
		Filterable::InitializeImplementation(args);
	}
}

} //log namespace
} //bio namespace
