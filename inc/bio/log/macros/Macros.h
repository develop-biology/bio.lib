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

#define BIO_LOG_PRINTF_MAX_LINE_SIZE 2000 //(+1 for \0)

/**
 * To make defining LogLevels easier, use this macro to define the function body of your LogLevel Function(). <br />
 * This will assign a value to a string that is identical to your FunctionName (e.g. SafelyAccess<LogLevelPerspective>()->GetNameFromId(MySpecialInformation()) would give "MySpecialInformation") <br />
 * Necessitates that functionName be a part of the log namespace and that any other namespaces are already specified (e.g. using namespace) <br />
 */
#define BIO_LOG_LEVEL_FUNCTION_BODY(functionName)                              \
BIO_ID_FUNCTION_BODY(                                                          \
    functionName,                                                              \
    ::bio::LogLevelPerspective::Instance(),                                    \
    ::bio::LogLevel)

//@formatter:off

#ifdef BIO_LOG_DISABLE_DEBUG
	#define BIO_LOG_DEBUG(...)
#else
	#define BIO_LOG_DEBUG(...)	this->Log(::bio::log_level::Debug(), __VA_ARGS__);
#endif
#ifdef BIO_LOG_DISABLE_INFO
	#define BIO_LOG_INFO(...)
#else
	#define BIO_LOG_INFO(...)	this->Log(::bio::log_level::Info(), __VA_ARGS__);
#endif
#ifdef BIO_LOG_DISABLE_WARN
	#define BIO_LOG_WARN(...)
#else
	#define BIO_LOG_WARN(...)	this->Log(::bio::log_level::Warn(), __VA_ARGS__);
#endif
#ifdef BIO_LOG_DISABLE_ERROR
	#define BIO_LOG_ERROR(...)
#else
	#define BIO_LOG_ERROR(...)	this->Log(::bio::log_level::Error(), __VA_ARGS__);
#endif
