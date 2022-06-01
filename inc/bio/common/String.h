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

//@formatter:off
#if BIO_CPP_VERSION < 11
	#include <stdint.h>
#else
	#include <cstdint>
#endif
//@formatter:on

#include <string>
#include <sstream>

namespace bio {

/**
 * We use our own String implementation in order to ensure consistency across the library and to provide a single point of optimization for all String operations. <br />
 * For example, we would like bio::Strings to encompass the compile-time behavior of std::string_view and run-time versatility of std::string. <br />
 * Right now, this is essentially just a wrapper around std::string. <br />
 * <br />
 * Like other Biology classes, String is inheritable and designed to be extended.
 */
class String
{
public:
	
	/**
	 * 
	 */
	String();
	
	/**
	 * @param string 
	 */
	String(const char* string);
	
	/**
	 * @param string 
	 */
	String(::std::string string);

	/**
	 * @param toCopy
	 */
	String(const String& toCopy);
	
	/**
	 * 
	 */
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

	/**
	 * @param string 
	 * @return *this.
	 */
	virtual String& operator=(const char* string);

	/**
	 * @param string 
	 * @return *this.
	 */
	virtual String& operator=(::std::string string);

	/**
	 * @param other
	 * @return whether or not the contents of *this match those of other.
	 */
	virtual bool operator==(const String& other) const;

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

	/**
	 * Copies the contents of source into a new const char*. <br />
	 * NOTE: because "new" is used here, a delete needs to be called elsewhere. <br />
	 * @param source
	 * @return a new const char* with the contents of source.
	 */
	static const char* GetCloneOf(const char* source);
	
protected:
	::std::string mString;

};

} //bio namespace
