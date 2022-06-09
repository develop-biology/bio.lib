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

#include "ImmutableString.h"
#include <sstream>

//@formatter:off
#if BIO_CPP_VERSION < 11
	#include <stdint.h>
#else
	#include <cstdint>
#endif
//@formatter:on

namespace bio {

/**
 * We use our own String implementation in order to ensure consistency across the library and to provide a single point of optimization for all String operations. <br />
 * We would like bio::Strings to encompass both the compile-time behavior of std::string_view and run-time versatility of std::string while being fully backwards compatible. <br />
 * <br />
 * Like other Biology classes, String is inheritable and designed to be extended. <br />
 * <br />
 * String is implemented as the addition of mutability to ImmutableString by way of a non-trivial destructor with the ability to allocate and delete data. <br />
 * ImmutableStrings can be used in constexpr while Strings cannot. <br />
 * <br />
 * Strings have different Modes. These Modes indicate how to handle internal operations. <br />
 * The Mode of a String is set depending on how it is constructed. <br />
 * You may change the String::Mode using the static SetMode method, which will produce a new String. <br />
 * A READ_ONLY String cannot be set by any means. If you wish to edit a READ_ONLY String, you must SetMode first. <br />
 * A COPY_ON_WRITE String will automatically become a READ_WRITE String when written to. <br />
 * NOTE: There are no "WRITE" operation for COPY_ON_WRITE to use. This will likely change in a future release. <br />
 */
class String : public ImmutableString
{
public:

	typedef enum {
		INVALID = 0,
		READ_ONLY,
		COPY_ON_WRITE, //TODO
		READ_WRITE,
		MODE_MAX
	} Mode;

	/**
	 * Copies the contents of source, up to length characters, into a new const char*. <br />
	 * NOTE: because "new" is used here, a delete needs to be called elsewhere. <br />
	 * @param source
	 * @param length if 0, the whole source is copied.
	 * @return a new const char* with the contents of source.
	 */
	static const char* GetCloneOf(const char* source, ::std::size_t length=0);

	/**
	 * In order to change the Mode of a String, you must create a new String. <br />
	 * UNDEFINED BEHAVIOR if you: <br />
	 * 1. create a READ_ONLY version of a READ_WRITE String and the source string becomes inaccessible. <br />
	 * 2. create a COPY_ON_WRITE String which does not become READ_WRITE before the source string becomes inaccessible. <br />
	 * @param string
	 * @param desiredMode
	 * @return a new String with the desiredMode or a String(), if the desiredMode is not valid.
	 */
	static String SetMode(const String& string, Mode desiredMode);

	/**
	 * An empty string will point to NULL, have a length of 0. <br />
	 * You may specify the Mode of an empty String. <br />
	 */
	String(Mode mode = INVALID);


	/**
	 * Constructing a String from a const char* basically gives a std::string_view. <br />
	 * The resulting string will be READ_ONLY and will give UNDEFINED BEHAVIOR if the original string is deleted, goes out of scope, or otherwise becomes inaccessible. <br />
	 * @param string
	 */
	String(const char* string);

	/**
	 * Constructing a String from a std::string basically gives back an std::string. <br />
	 * std::strings can only ever READ_WRITE, since their c_str() method goes out of scope on subsequent access. <br />
	 * @param string
	 */
	String(::std::string string);

	/**
	 * ImmutableStrings are treated as const char* and will yield a READ_ONLY String. <br />
	 * @param string
	 */
	String(const ImmutableString& string);

	/**
	 * @param toCopy
	 */
	String(const String& toCopy);

	#if BIO_CPP_VERSION >= 11
	/**
	 * @param toMove
	 */
	String(const String&& toMove);
	#endif

	virtual ~String();

	/**
	 * Converts the given value to a string. <br />
	 * @param value
	 */
	template < typename T >
	static String From(const T& value)
	{
		std::ostringstream str;
		str << value;
		return String(str.str());
	}

	#if BIO_CPP_VERSION >= 11
	/**
	 * NOTE: Mode is moved.
	 * @param toMove
	 * @return *this.
	 */
	virtual String& operator=(const String&& toMove);
	#endif

	/**
	 * NOTE: Mode is NOT copied.
	 * @param toCopy
	 * @return *this.
	 */
	virtual String& operator=(const String& toCopy);

	/**
	 * This is just lies: we const_cast(this) and change the values.. <br />
	 * This is used in places where a move operation would be performed but since that can't be done prior to c++11, we hack it. <br />
	 * All Values are moved, including Mode,
	 * @param toMoveHack
	 * @return *this.
	 */
	virtual const String& operator=(const String& toMoveHack) const;

	/**
	 * @param toAssign
	 * @return *this.
	 */
	virtual String& operator=(const ImmutableString& toAssign);

	/**
	 * @param string
	 * @return *this.
	 */
	virtual String& operator=(const char* string);

	/**
	 * NOTE: Assigning *this an std::string forces it to be READ_WRITE! <br />
	 * @param string
	 * @return *this.
	 */
	virtual String& operator=(::std::string string);

	/**
	 * NOTE: We ignore Mode when comparing Strings.
	 * @param other
	 * @return whether or not the contents of *this match those of other.
	 */
	virtual bool operator==(const ImmutableString& other) const;

	/**
	 * NOTE: We ignore Mode when comparing Strings.
	 * @param other
	 * @return whether or not the contents of *this match those of other.
	 */
	virtual bool operator==(const String& other) const;

	/**
	 * @param other
	 * @return whether or not the contents of *this match those of other.
	 */
	virtual bool operator==(const char* other) const;

	/**
	 * @param other
	 * @return whether or not the contents of *this match those of other.
	 */
	virtual bool operator==(const ::std::string& other) const;

	/**
	 * @return *this as an std::string.
	 */
	virtual operator ::std::string() const;

	/**
	 * You can implicitly treat Strings as booleans the same way you do pointers (specifically char*). <br />
	 * NOTE: THIS IS NOT this->AsBool()!!!
	 * @return whether or not *this is empty.
	 */
	virtual operator bool() const;

	/**
	 * @return the Mode of *this.
	 */
	virtual Mode GetMode() const;

	/**
	 * Get a smaller string from *this. <br />
	 * @param start
	 * @param end
	 * @return a READ_ONLY String consisting of the contents of *this between start and end.
	 */
	virtual String SubString(::std::size_t start, ::std::size_t length) const;

	/**
	 * Get *this as an std::string.
	 * @return *this as an std::string.
	 */
	virtual ::std::string AsStdString() const;

	/**
	 * Get a *new* const char* from *this. <br />
	 * YOU MUST delete THE RETURNED VALUE TO AVOID MEMORY LEAKS! <br />
	 * @return a new const char* copy of *this.
	 */
	virtual const char* AsCharString() const;

	/**
	 * Convert "true" or "false" to bool. <br />
	 * This is essentially just {== "true"} with no extra test for "false". <br />
	 * This behavior may change in a future release. <br />
	 * Case insensitive. <br />
	 * @return *this as a bool, false by default.
	 */
	virtual bool AsBool() const;

	/**
	 * convert *this to an integer. <br />
	 * @return *this as an integer; 0 by default.
	 */
	virtual int32_t AsInt() const;

	/**
	 * convert *this to an unsigned integer. <br />
	 * @return *this as an unsigned integer; 0 by default.
	 */
	virtual uint32_t AsUInt() const;

	/**
	 * convert *this to a float. <br />
	 * @return *this as a float; 0.0f by default.
	 */
	virtual float AsFloat() const;


protected:
	Mode mMode;

	/**
	 * Will delete mString if *this is READ_WRITE, etc. <br />
	 */
	virtual void Clear();
};

} //bio namespace
