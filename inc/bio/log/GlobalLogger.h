/*
 * This file is a part of the Biology project by eons LLC.
 * Biology (aka Develop Biology) is a framework for approaching software
 * development from a natural sciences perspective.
 *
 * Copyright (C) 2023 Séon O'Shannon & eons LLC
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

#include "bio/log/common/Types.h"
#include "bio/log/macro/Macros.h"
#include "bio/physical/common/Class.h"

namespace bio {
namespace log {

class Engine;

class GlobalLoggerImplementation :
	public bio::physical::Class< GlobalLoggerImplementation >
{
public:

	/**
	 * Disambiguate all class methods. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(physical, GlobalLoggerImplementation)

	/**
	 *
	 */
	GlobalLoggerImplementation();

	/**
	 *
	 */
	virtual ~GlobalLoggerImplementation();


	/**
	 * Set the log::Engine* for *this.
	 * @param logEngine
	 */
	virtual void SetLogEngine(Engine* logEngine);

	/**
	 * @return a pointer to the log::Engine used by *this.
	 */
	Engine* GetLogEngine();

	/**
	 * @return a const pointer to the log::Engine used by *this.
	 */
	const Engine* GetLogEngine() const;

	/**
	 * @return whether or not log::Engine* has been set.
	 */
	bool HasLogEngine() const;

	/**
	 * Send a log message to the log::Engine* used by *this.
	 * @param logFilter
	 * @param level
	 * @param format
	 * @param ...
	 */
	void Log(
		const Filter& logFilter,
		const LogLevel& level,
		const char* format,
		...
	) const;

private:

	/**
	 * The log::Engine* used by *this.
	 */
	Engine* mLogEngine;
};

BIO_SINGLETON(GlobalLogger, GlobalLoggerImplementation)

} //log namespace
} //bio namespace
