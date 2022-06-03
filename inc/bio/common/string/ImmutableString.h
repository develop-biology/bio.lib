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

#include <string>
#include <cstring>
#include <cstdlib>

namespace bio {

/**
 * ImmutableStrings provide a bare-bones String implementation and are essentially a backwards-compatible std::string_view. <br />
 * These are INTENDED ONLY FOR USE IN CONSTEXPR. <br />
 * For more on Strings, see String.h
 */
class ImmutableString
{
	friend class String; //makes life easy, no need to use this-> or define extra access methods here.

public:

	/**
	 * An empty string will point to NULL and have a length of 0. <br />
	 */
	BIO_CONSTEXPR ImmutableString() :
		mString(NULL),
		mLength(0)
	{

	}

	/**
	 * @param string
	 */
	BIO_CONSTEXPR ImmutableString(const char* string) :
		mString(string),
		mLength(strlen(string))
	{

	}

	/**
	 * For derived classes only.
	 * @param string
	 * @param length
	 */
	explicit ImmutableString(const char* string, ::std::size_t length) :
		mString(string),
		mLength(length)
	{

	}

	/**
	 * @param toCopy
	 */
	BIO_CONSTEXPR ImmutableString(const ImmutableString& toCopy) :
		mString(toCopy.mString),
		mLength(toCopy.mLength)
	{

	}

	#if BIO_CPP_VERSION >= 11
	/**
	 * @param toMove
	 */
	BIO_CONSTEXPR ImmutableString(const ImmutableString&& toMove):
		mString(toMove.mString),
		mLength(toMove.mLength)
	{

	}
	#endif

	#if BIO_CPP_VERSION >= 11
	/**
	 * @param toMove
	 * @return *this
	 */
	BIO_CONSTEXPR ImmutableString& operator=(const ImmutableString&& toMove)
	{
		mString = toMove.mString;
		mLength = toMove.mLength;
		return *this;
	}
	#endif

	/**
	 * @param toCopy
	 * @return
	 */
	BIO_CONSTEXPR ImmutableString& operator=(const ImmutableString& toCopy)
	{
		mString = toCopy.mString;
		mLength = toCopy.mLength;
		return *this;
	}

	/**
	 * @return the number of characters in *this.
	 */
	BIO_CONSTEXPR ::std::size_t Length() const
	{
		return mLength;
	}

	/**
	 * Find the start position of a sub-string in *this.
	 * @param substring
	 * @return the first Index where substring appears.
	 */
	BIO_CONSTEXPR ::std::size_t Find(const ImmutableString& substring) const
	{
		//We can almost use strstr here. However, our use of mLength means we would need strnstr, which does not appear to be standard.
		//We could also simplify this by making the inner loop use strncmp but meh.

		::std::size_t start = 0;
		::std::size_t str = 0;
		::std::size_t sub = 0;
		while (start < mLength)
		{
			if (mString[start] == substring.mString[0])
			{
				str = start + 1;
				sub = 1;
				if (sub == substring.mLength) //mLength is 1.
				{
					return start;
				}
				while (mString[sub] == substring.mString[sub])
				{
					if (sub == substring.mLength)
					{
						return start;
					}
					++str;
					++sub;
					if (str == mLength)
					{
						return 0;
					}
				}
			}
			++start;
		}
		return 0;
	}

	/**
	 * Get a smaller string from *this. <br />
	 * @param start
	 * @param end
	 * @return a READ_ONLY String consisting of the contents of *this between start and end.
	 */
	BIO_CONSTEXPR ImmutableString GetImmutableSubString(::std::size_t start, ::std::size_t length) const
	{
		BIO_SANITIZE(start + length < Length(),
		,
			return ImmutableString()
		)
		ImmutableString ret;
		ret.mString = &mString[start];
		ret.mLength = length;
		return ret;
	}


protected:
	mutable const char* mString;
	::std::size_t mLength;
};

} //bio namespace
