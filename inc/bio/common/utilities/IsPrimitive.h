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
namespace utility {

/**
 * Defines which types return true when called with IsPrimitive(). <br />
 * This implementation is finicky and support of c++ versions less than 11 would damage the sanity of our API. <br />
 * Thus, if using a c++ version less than 11, EVERYTHING is considered Primitive. <br />
 * @tparam T
 * @return whether or not T is a built in type.
 */
template < typename T >
BIO_CONSTEXPR bool IsPrimitiveImplementation()
{
	//@formatter:off
	#if BIO_CPP_VERSION < 11
		return true;
	#else
		return false;
	#endif
	//@formatter:on
}

/**
 * IsPrimitive will return whether or not the given type is built in. <br />
 * If the type provided is a pointer and we can dereference it, we will (c++11 and above). <br />
 * For Biology classes, this should, with few exceptions, always mean a child of physical::Wave. <br />
 * @tparam T
 * @return whether or not T is a built-in type or a custom class; false by default.
 */
template < typename T >
BIO_CONSTEXPR bool IsPrimitive()
{
	if (IsPointer< T >())
	{
		//@formatter:off
		#if BIO_CPP_VERSION < 11
			return false;
		#else
			return IsPrimitive< typename ::std::remove_pointer< T >::type >();
		#endif
		//@formatter:on
	}

	//@formatter:off
	#if BIO_CPP_VERSION < 11
		return IsPrimitiveImplementation< T >();
	#else
		//We need to make sure we include our custom overrides of what IsPrimitive.
		if (IsPrimitiveImplementation< T >())
		{
			return true;
		}
		return ::std::is_fundamental< T >::value;
	#endif
	//@formatter:on
}

} //utility namespace
} //bio namespace


#if BIO_CPP_VERSION < 11

//NOTE: These are vestigial at the moment: if BIO_CPP_VERSION < 11, ALL types are Primitive.

BIO_SET_PRIMITIVE(bool)
BIO_SET_PRIMITIVE(float)
BIO_SET_PRIMITIVE(double)

//This is int8_t & may cause compiler errors.
//BIO_SET_PRIMITIVE(char)

BIO_SET_PRIMITIVE(::std::string)

BIO_SET_PRIMITIVE(int8_t)
BIO_SET_PRIMITIVE(int16_t)
BIO_SET_PRIMITIVE(int32_t)
BIO_SET_PRIMITIVE(int64_t)
BIO_SET_PRIMITIVE(uint8_t)
BIO_SET_PRIMITIVE(uint16_t)
BIO_SET_PRIMITIVE(uint32_t)
BIO_SET_PRIMITIVE(uint64_t)

#endif
