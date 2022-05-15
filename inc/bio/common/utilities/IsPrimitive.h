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

#include "bio/common/macros/Macros.h"
#include "bio/common/TransparentWrapper.h"
#include "IsPointer.h"

//@formatter:off
#if BIO_CPP_VERSION < 11
	#include <stdint.h>
#else
	#include <cstdint>
#endif
//@formatter:on

//@formatter:off
#if BIO_CPP_VERSION >= 11
	#include <type_traits>
#endif
//@formatter:on

namespace bio {

class StandardDimension;

namespace utility {

/**
 * Helper for IsPrimitiveImplementation.
 * @tparam T
 * @return whether or not T derives from a TransparentWrapper of a hard-coded primitive type.
 */
template < typename T >
BIO_CONSTEXPR bool IsWrappedPrimitive()
{
	#if BIO_CPP_VERSION >= 11
	if (::std::is_base_of< TransparentWrapper< bool >, T >::value)
	{
		return true;
	}
	if (::std::is_base_of< TransparentWrapper< float >, T >::value)
	{
		return true;
	}
	if (::std::is_base_of< TransparentWrapper< double >, T >::value)
	{
		return true;
	}
	if (::std::is_base_of< TransparentWrapper< int8_t >, T >::value)
	{
		return true;
	}
	if (::std::is_base_of< TransparentWrapper< int16_t >, T >::value)
	{
		return true;
	}
	if (::std::is_base_of< TransparentWrapper< int32_t >, T >::value)
	{
		return true;
	}
	if (::std::is_base_of< TransparentWrapper< int64_t >, T >::value)
	{
		return true;
	}
	if (::std::is_base_of< TransparentWrapper< uint8_t >, T >::value)
	{
		return true;
	}
	if (::std::is_base_of< TransparentWrapper< uint16_t >, T >::value)
	{
		return true;
	}
	if (::std::is_base_of< TransparentWrapper< uint32_t >, T >::value)
	{
		return true;
	}
	if (::std::is_base_of< TransparentWrapper< uint64_t >, T >::value)
	{
		return true;
	}
	#endif
	return false;
}

/**
 * Defines which types return true when called with IsPrimitive(). <br />
 * This implementation is finicky and cannot be reliably extended beyond the confines of this library. <br />
 * THERE IS NO SUPPORT FOR SPECIALIZING THIS IN DOWNSTREAM CODE.
 * @tparam T
 * @return whether or not T is a built in type.
 */
template < typename T >
struct IsPrimitiveImplementation
{
	//@formatter:off
	#if BIO_CPP_VERSION < 11
		static const bool sValue = false;
	#else
		static const bool sValue = IsWrappedPrimitive< T >();
	#endif
	//@formatter:on
};

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
		return IsPrimitiveImplementation< T >::sValue;
	#else
		//We need to make sure we include our custom overrides of what IsPrimitive.
		if (IsPrimitiveImplementation< T >::sValue)
		{
			return true;
		}
		return ::std::is_fundamental< T >::value;
	#endif
	//@formatter:on
}

} //utility namespace
} //bio namespace
