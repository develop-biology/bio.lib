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

#include "bio/common/macros/LanguageMacros.h"
#include "bio/common/macros/Macros.h"
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
	template<typename T>
	struct IsPrimitiveImplementation {static const bool m_value = false;};
#endif
//@formatter:on

/**
 * IsPrimitive is a bit more complex than it might need to be but the features, while slow, should be robust.
 * First, if T is a pointer, we'll dereference it and try again until it is not.
 * Second, if we're using C++98 and don't have an interface for T, we'll try looking for std:: in its typename.
 * @tparam T
 * @param t
 * @return whether or not T is a built-in type or a Biology class, which should, except for a few exceptions, always mean a child of physical::Wave; false by default.
 */
template < typename T >
bool IsPrimitive(const T t)
{
	BIO_SANITIZE_AT_SAFETY_LEVEL_2(!IsPointer< T >(), ,
		return IsPrimitive(*t));

	//@formatter:off
	#if BIO_CPP_VERSION < 11
		return IsPrimitiveImplementation< T >::m_value;
	#else
		return std::is_fundamental<T>::value;
	#endif
	//@formatter:on
}

/**
 * IsPrimitive without an arg is the same as that with, except no automatic pointer dereferencing can be done.
 * If we're using C++98 and don't have an interface for T, we'll try looking for std:: in its typename.
 * @tparam T
 * @return whether or not T is a built-in type or a Biology class, which should, except for a few exceptions, always mean a child of physical::Wave; false by default.
 */
template < typename T >
bool IsPrimitive()
{
	if (IsPointer< T >())
	{
		return false;
	}
	//@formatter:off
	#if BIO_CPP_VERSION < 11
		return IsPrimitiveImplementation< T >::m_value;
	#else
		return std::is_fundamental<T>::value;
	#endif
	//@formatter:on
}

#if BIO_CPP_VERSION < 11

template <>
struct IsPrimitiveImplementation< bool > {static const bool m_value = true;};

template <>
struct IsPrimitiveImplementation< float > {static const bool m_value = true;};

template <>
struct IsPrimitiveImplementation< double > {static const bool m_value = true;};

//This is int8_t & may cause compiler errors.
//template <>
//struct IsPrimitiveImplementation< char > {static const bool m_value = true;};

template <>
struct IsPrimitiveImplementation< std::string >{static const bool m_value = true;};

template <>
struct IsPrimitiveImplementation< int8_t > {static const bool m_value = true;};

template <>
struct IsPrimitiveImplementation< int16_t > {static const bool m_value = true;};

template <>
struct IsPrimitiveImplementation< int32_t > {static const bool m_value = true;};

template <>
struct IsPrimitiveImplementation< int64_t > {static const bool m_value = true;};

template <>
struct IsPrimitiveImplementation< uint8_t > {static const bool m_value = true;};

template <>
struct IsPrimitiveImplementation< uint16_t > {static const bool m_value = true;};

template <>
struct IsPrimitiveImplementation< uint32_t > {static const bool m_value = true;};

template <>
struct IsPrimitiveImplementation< uint64_t > {static const bool m_value = true;};

#endif

} //bio namespace
