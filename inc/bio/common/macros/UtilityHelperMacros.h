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

/**
 * Makes IsPrimitive< type >() return true. <br />
 * This only works on c++11 and above. <br />
 */
#if BIO_CPP_VERSION < 11
#define BIO_SET_PRIMITIVE(type) /* nop */
#else
#define BIO_SET_PRIMITIVE(type)                                                \
template <>                                                                    \
BIO_CONSTEXPR                                                                  \
bool ::bio::utility::IsPrimitiveImplementation< type >()                       \
{                                                                              \
	return true;                                                               \
}
#endif

/**
 * Causes calls to IsPrimitive< type >() to return the result of IsPrimitive< forwardTo >() <br />
 * This only works on c++14 and above; on c++11, it compiles but is not constexpr. <br />
 */
#if BIO_CPP_VERSION < 11
#define BIO_FORWARD_PRIMITIVE(type, forwardTo) /*nop*/
#else
#define BIO_FORWARD_PRIMITIVE(type, forwardTo)                                 \
/*Forward primitive name to type.*/                                            \
                                                                               \
template <>                                                                    \
BIO_CONSTEXPR                                                                  \
bool ::bio::utility::IsPrimitiveImplementation< type >()                       \
{                                                                              \
    return ::bio::utility::IsPrimitive< forwardTo >();                         \
};
#endif
