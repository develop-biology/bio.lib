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

#include "bio/common/macro/Macros.h"

//@formatter:off
#if BIO_CPP_VERSION >= 11
	#include <type_traits>
#endif
//@formatter:on

namespace bio {
namespace utility {

//@formatter:off
#if BIO_CPP_VERSION < 11
template<typename T>
struct IsReferenceImplementation {static const bool sValue = false;};

template<typename T>
struct IsReferenceImplementation<T&> {static const bool sValue = true;};

#endif
//@formatter:on

/**
 * Check whether or not T is a reference <br />
 * @tparam T
 * @return whether or not T has an '&' at the end..
 */
template < typename T >
BIO_CONSTEXPR bool IsReference()
{
	//@formatter:off
	#if BIO_CPP_VERSION < 11
		return IsReferenceImplementation< T >::sValue;
	#else
		return ::std::is_reference<T>::value;
	#endif
	//@formatter:on
}

/**
 * Ease of use method for passing T as arg. <br />
 * @tparam T
 * @param t
 * @return whether or not T is a pointer.
 */
template < typename T >
bool IsReference(const T t)
{
	return IsReference< T >();
}

} //utility namespace
} //bio namespace
