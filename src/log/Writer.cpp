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
#include "bio/common/macros/Macros.h"

#include <stdarg.h>

namespace bio {
namespace log {

Writer::Writer()
	:
	physical::Class< Writer >(this),
	physical::Filterable(filter::Default()),
	m_logEngine(NULL)
{

}

Writer::Writer(
	Engine* logEngine, 
	Filter filter
)
	:
	physical::Class< Writer >(this),
	physical::Filterable(filter::Default()),
	m_logEngine(logEngine)
{

}

Writer::~Writer()
{

}

void Writer::SetLogEngine(Engine* logEngine) 
{
	m_logEngine = logEngine;
}

Engine* Writer::GetLogEngine() 
{
	return m_logEngine;
}

const Engine* Writer::GetLogEngine() const 
{
	return m_logEngine;
}

bool Writer::HasLogEngine() const
{
	return m_logEngine != NULL;
}

void Writer::Log(
	LogLevel level,
	const char* format, 
	...
) const
{

	BIO_SANITIZE(m_logEngine, ,
		return);

	//Check if filter is on
	BIO_SANITIZE_AT_SAFETY_LEVEL_2(m_logEngine->FilterPass(
		m_filter,
		level
	), ,
		return);

	va_list args;
	va_start(args,
		format);
	m_logEngine->Log(
		m_filter,
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
	...
) const
{
	BIO_SANITIZE(m_logEngine, ,
		return);

	va_list args;
	va_start(args,
		format);
	m_logEngine->Log(
		filter,
		level,
		format,
		args
	);
	va_end(args);
}

void Writer::InitializeImplementation(ByteStreams args)
{
	if (args.Size() == 2)
	{
		if (args[1].Is(m_logEngine))
		{
			m_logEngine = args[1];
		}
		args.pop_back();
	}
	if (args.Size() == 1 && args[0].Is< Filter >())
	{
		Filterable::InitializeImplementation(args);
	}
}

} //log namespace
} //bio namespace
