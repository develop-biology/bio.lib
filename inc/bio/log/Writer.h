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
#include "bio/log/common/Levels.h"
#include "bio/log/macros/Macros.h"

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
	virtual public physical::Filterable,
	public physical::Class< Writer >
{
public:

	/**
	 * Ensure virtual methods point to Class implementations. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(physical,
		Writer)

	/**
	 *
	 */
	Writer();

	/**
	 * @param logEngine
	 * @param logFilter
	 */
	Writer(
		Engine* logEngine, 
		Filter logFilter
	);

	/**
	 *
	 */
	virtual ~Writer();

	/**
	 * For static callers only. <br />
	 * This is a little hacky, but its better than having a static logger <br />
	 * @param logFilter
	 * @param level
	 * @param format
	 * @param ...
	 */
	void ExternalLog(
		Filter logFilter,
		Level level,
		const char* format, 
		...
	) const;

	/**
	 * Set the log::Engine* for *this. 
	 * Propagate logEngine to any and all related classes that should receive the change. <br />
	 * if *this contains member LoggerObjects, these may be overridden to pass the call along to those objects as well. <br />
	 * It should be assumed (and is true for bio classes) that the creation of any Writer within another Writer will have its m_logEngine* set by the owner object AND that calling SetLogEngine on an owner object will also call SetLogEngine on all objects owned by it. 
	 * This is not true for SetFilter, as each class may use a different filter <br />
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

protected:

	/**
	 * VirtualBase required method. See that class for details (in common/) <br />
	 * @param args
	 */
	virtual void InitializeImplementation(ByteStreams args);

	/**
	 * In case the above are too restrictive <br />
	 * @param level
	 * @param format
	 * @param ...
	 */
	void Log(
		Level level,
		const char* format, 
		...
	) const;

private:
	Engine* m_logEngine; 
};
} //log namespace
} //bio namespace
