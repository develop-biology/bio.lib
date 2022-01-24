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
#inclued "IsPointer.h"

#if BIO_CPP_VERSION >= 11

	#include <type_traits>
	#include <stdint.h>

#else
	#include <string>
	#include <cstdint>
#endif

namespace bio {

/**
 * IsPrimitive is a bit more complex than it might need to be but the features, while slow, should be robust.
 * First, if T is a pointer, we'll dereference it and try again until it is not.
 * Second, if we're using C++98 and don't have an implementation for T, we'll try looking for std:: in its typename.
 * @tparam T
 * @return whether or not T is a built-in type or a Biology class, which should, except for a few exceptions, always mean a child of physical::Wave; false by default.
 */
template <typename T>
inline bool IsPrimitive()
{
	BIO_SANITIZE_AT_LEVEL_2(!IsPointer<T>(), , return IsPrimitive<*T>());
	#if BIO_CPP_VERSION >= 11
	return std::is_fundamental<T>::value;
	#else
	bool ret;
	BIO_SANITIZE_AT_LEVEL_2(IsPrimitiveImplementation<T>(&ret),return ret, return IsPrimitiveStringImplementation<T>());
	#endif
}

/**
 * Helper method for automatic type determination of expression.
 * @tparam T an auto-determined template type.
 * @param t an expression yielding a T.
 * @return Whether or not T is a built in type (i.e. the value of IsPrimitive<T>()).
 */
template <typename T>
inline bool IsPrimitive(const T& t)
{
	return IsPrimitive<T>();
}

#if BIO_CPP_VERSION < 11
template <>
	bool IsPrimitiveImplementation<bool>()
	{
		return true;
	}

	template <>
	bool IsPrimitiveImplementation<float>()
	{
		return true;
	}

	tempate <>
	bool IsPrimitiveImplementation<double>()
	{
		return true;
	}

//	template <>
//	bool IsPrimitiveImplementation<char>()
//	{
//		return true;
//	}

	template <>
	bool IsPrimitiveImplementation<std::string>()
	{
		return true;
	}

	template <>
	bool IsPrimitiveImplementation<int8_t>()
	{
		return true;
	}

	template <>
	bool IsPrimitiveImplementation<int16_t>()
	{
		return true;
	}

	template <>
	bool IsPrimitiveImplementation<int32_t>()
	{
		return true;
	}

	template <>
	bool IsPrimitiveImplementation<int64_t>()
	{
		return true;
	}

	template <>
	bool IsPrimitiveImplementation<uint8_t>()
	{
		return true;
	}

	template <>
	bool IsPrimitiveImplementation<uint16_t>()
	{
		return true;
	}

	template <>
	bool IsPrimitiveImplementation<uint32_t>()
	{
		return true;
	}

	template <>
	bool IsPrimitiveImplementation<uint64_t>()
	{
		return true;
	}

		template <>
	bool IsPrimitiveImplementation<int8_t>()
	{
		return true;
	}

	template <>
	bool IsPrimitiveImplementation<int16_t>()
	{
		return true;
	}

	template <>
	bool IsPrimitiveImplementation<int32_t>()
	{
		return true;
	}

	template <typename T>
	bool IsPrimitiveStringImplementation()
	{
		return TypeName<T>().find("std::") != std::string::npos;
	}

#endif


} //bio namespace