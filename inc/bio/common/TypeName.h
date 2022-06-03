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

#include "bio/common/string/ImmutableString.h"
#include "bio/common/macro/Macros.h"

namespace bio {

//Modified from: https://stackoverflow.com/questions/35941045/can-i-obtain-c-type-names-in-a-constexpr-way

/**
 * FORWARD DECLARATION <br />
 * @tparam T
 * @return Just T as a string.
 */
template < typename T >
BIO_CONSTEXPR ImmutableString TypeName();

/**
 * @return "void"
 */
template <>
BIO_CONSTEXPR inline ImmutableString TypeName< void >()
{
	return ImmutableString("void");
}

/**
 * Used to determine magic numbers in extracting symbols as type. <br />
 */
typedef void TypeNameProber;

/**
 * Converts the T symbol to a string. <br />
 * Only supported on a limited number of compilers. <br />
 * @tparam T
 * @return T as a string with wrapping symbols.
 */
template < typename T >
BIO_CONSTEXPR static const ImmutableString WrappedTypeName()
{
	#ifdef __clang__
	return __PRETTY_FUNCTION__;
	#elif defined(__GNUC__)
	return __PRETTY_FUNCTION__;
	#elif defined(_MSC_VER)
	return __FUNCSIG__;
	#else
		#error "Unsupported compiler"
	#endif
}

/**
 * Used to trim leading characters from symbol string. <br />
 * @return magic number for prefix length.
 */
BIO_CONSTEXPR static const ::std::size_t WrappedTypeNamePrefixLength()
{
	return WrappedTypeName< TypeNameProber >().Find(TypeName< TypeNameProber >());
}

/**
 * Used to trim trailing characters from symbol string. <br />
 * @return magic number for suffix length.
 */
BIO_CONSTEXPR static const ::std::size_t WrappedTypeNameSuffixLength()
{
	return WrappedTypeName< TypeNameProber >().Length() - WrappedTypeNamePrefixLength() - TypeName< TypeNameProber >().Length();
}

/**
 * Converts the T symbol to a string. <br />
 * @tparam T
 * @return Just T as a string.
 */
template < typename T >
BIO_CONSTEXPR static ImmutableString TypeName()
{
	ImmutableString wrappedName = WrappedTypeName< T >();
	::std::size_t prefixLength = WrappedTypeNamePrefixLength();
	::std::size_t suffixLength = WrappedTypeNameSuffixLength();
	::std::size_t typeNameLength = wrappedName.Length() - prefixLength - suffixLength;
	return wrappedName.GetImmutableSubString(
		prefixLength,
		typeNameLength
	);
}

/**
 * Converts the T symbol to a string. <br />
 * @tparam T
 * @param t used for auto type determination.
 * @return Just T as a string.
 */
template < typename T >
BIO_CONSTEXPR static ImmutableString TypeName(const T t)
{
	return TypeName< T >();
}

} //bio namespace
