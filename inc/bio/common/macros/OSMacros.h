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


#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
	#define BIO_OS_IS_WINDOWS 1
#endif

#if defined(__unix__) || defined(__unix) || defined(unix)
	#define BIO_OS_IS_UNIX 1
#endif

#if defined(__linux__) || defined(__linux) || defined(linux)
	#define BIO_OS_IS_LINUX
#endif

#if defined(BSD)
	#define BIO_OS_IS_BSD
#endif

#if defined(__APPLE__) || defined(__MACH__)
	#define BIO_OS_IS_APPLE
#endif

#if defined(TARGET_OS_EMBEDDED) || defined(TARGET_IPHONE_SIMULATOR) || defined(TARGET_OS_IPHONE) || defined(TARGET_OS_MAC)
	#define BIO_OS_IS_I
#endif

#if defined(__ANDROID__)
	#define BIO_OS_IS_ANDROID
#endif

#ifdef BIO_OS_IS_WINDOWS
	#define snprintf _snprintf
	#define vsnprintf _vsnprintf
	#define strcasecmp _stricmp
	#define strncasecmp _strnicmp
#endif
