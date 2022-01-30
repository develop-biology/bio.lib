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

#include "Language.h"
#include "Macros.h"
#include "IsPointer.h"

//@formatter:off
#if BIO_CPP_VERSION < 11
	#include <string>
	#include <stdint.h>
#else
	#include <type_traits>
	#include <cstdint>
#endif
//@formatter:on

namespace bio {

//@formatter:off
#if BIO_CPP_VERSION < 11
	template < typename T >
	bool IsPrimitiveImplementation()
	{
		return false;
	}

	template < typename T >
	bool IsPrimitiveStringImplementation()
	{
		return TypeName< T >().find("std::") != std::string::npos;
	}
#endif
//@formatter:on

/**
 * IsPrimitive is a bit more complex than it might need to be but the features, while slow, should be robust.
 * First, if T is a pointer, we'll dereference it and try again until it is not.
 * Second, if we're using C++98 and don't have an implementation for T, we'll try looking for std:: in its typename.
 * @tparam T
 * @param t
 * @return whether or not T is a built-in type or a Biology class, which should, except for a few exceptions, always mean a child of physical::Wave; false by default.
 */
template < typename T >
inline bool IsPrimitive(const T t)
{
	BIO_SANITIZE_AT_SAFETY_LEVEL_2(!IsPointer< T >(), ,
		return IsPrimitive(*t));

	//@formatter:off
	#if BIO_CPP_VERSION < 11
		BIO_SANITIZE_AT_SAFETY_LEVEL_2(IsPrimitiveImplementation< T >(),
			return true,
			return IsPrimitiveStringImplementation< T >());
	#else
		return std::is_fundamental<T>::value;
	#endif
	//@formatter:on
}

/**
 * IsPrimitive without an arg is the same as that with, except no automatic pointer dereferencing can be done.
 * If we're using C++98 and don't have an implementation for T, we'll try looking for std:: in its typename.
 * @tparam T
 * @return whether or not T is a built-in type or a Biology class, which should, except for a few exceptions, always mean a child of physical::Wave; false by default.
 */
template < typename T >
inline bool IsPrimitive()
{
	//@formatter:off
	#if BIO_CPP_VERSION < 11
	BIO_SANITIZE_AT_SAFETY_LEVEL_2(IsPrimitiveImplementation< T >(),
		return true,
		return IsPrimitiveStringImplementation< T >());
	#else
		return std::is_fundamental<T>::value;
	#endif
	//@formatter:on
}

#if BIO_CPP_VERSION < 11

template <>
bool IsPrimitiveImplementation< bool >()
{
	return true;
}

template <>
bool IsPrimitiveImplementation< float >()
{
	return true;
}

template <>
bool IsPrimitiveImplementation< double >()
{
	return true;
}

//This is int8_t & may cause compiler errors.
//template <>
//bool IsPrimitiveImplementation< char >()
//{
//	return true;
//}

template <>
bool IsPrimitiveImplementation< std::string >()
{
	return true;
}

template <>
bool IsPrimitiveImplementation< int8_t >()
{
	return true;
}

template <>
bool IsPrimitiveImplementation< int16_t >()
{
	return true;
}

template <>
bool IsPrimitiveImplementation< int32_t >()
{
	return true;
}

template <>
bool IsPrimitiveImplementation< int64_t >()
{
	return true;
}

template <>
bool IsPrimitiveImplementation< uint8_t >()
{
	return true;
}

template <>
bool IsPrimitiveImplementation< uint16_t >()
{
	return true;
}

template <>
bool IsPrimitiveImplementation< uint32_t >()
{
	return true;
}

template <>
bool IsPrimitiveImplementation< uint64_t >()
{
	return true;
}

#endif


} //bio namespace
