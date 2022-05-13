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
#include "bio/common/utilities/IsPrimitive.h"
#include "ByteStream.h"

#include <vector>
#include <map>
#include <string>
#include <ostream>

namespace bio {
namespace utility {

/**
 * This specialization shouldn't be necessary. See IsWrappedPrimitive for more info. <br />
 * All Transparent wrappers should derive their Primitive nature from the type they wrap. <br />
 * On C++ versions below 14, this defaults to true: assuming TransparentWrapper ONLY wraps Primitive types.
 * @tparam T
 * @return IsPrimitive< T >(), stripping away TransparentWrapper
 */
template < typename T >
struct IsPrimitiveImplementation< TransparentWrapper< T > >
{
	//@formatter:off
	#if BIO_CPP_VERSION < 14
		static const bool sValue = true;
	#else
		static const bool sValue = IsPrimitive< T >();
	#endif
	//@formatter:on
};

#if BIO_CPP_VERSION < 11

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

} //utility namespace
} //bio namespace
