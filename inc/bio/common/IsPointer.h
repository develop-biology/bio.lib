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
	struct IsPointerImplementation {static const bool m_value = false;};

	template<typename T>
	struct IsPointerImplementation<T*> {static const bool m_value = true;};
#endif
//@formatter:on

/**
 * Check whether or not T is a pointer <br />
 * @tparam T
 * @return whether or not T is a pointer.
 */
template < typename T >
BIO_CONSTEXPR
bool IsPointer()
{
	//@formatter:off
	#if BIO_CPP_VERSION >= 11
		return std::is_pointer<T>::value;
	#else
		return IsPointerImplementation< T >::m_value;
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
BIO_CONSTEXPR
bool IsPointer(const T t)
{
	return IsPointer< T >();
}

/**
 * Maybe a helper method? <br />
 * This is currently unused. <br />
 * @tparam T
 * @param t
 * @return T
 */
template < typename T >
inline T& Dereference(T& t)
{
	return t;
}

/**
 * Maybe a helper method? <br />
 * This is currently unused. <br />
 * @tparam T
 * @param t
 * @return *T
 */
template < typename T >
inline T& Dereference(T* t) 
{
	return *t;
}

} //utility namespace
} //bio namespace
