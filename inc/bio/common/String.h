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
 * Common string operations <br />
 */

#include <string>
#include <cstring>
#include <vector>
#include <stdlib.h>
#include <sstream>

namespace bio {

typedef ::std::vector< const char* > CharStrings;
typedef ::std::vector< ::std::string > StdStrings;

namespace string {
/**
 * Convert "true" or "false" to bool <br />
 * Case sensitive. TODO: make insensitive. <br />
 * @param s
 * @param returned
 * @return true on success, false if s is not an integer
 */
bool ToBool(
	const char* s,
	bool* returned
);

/**
 * convert string s to an integer and return result in value <br />
 * @param s
 * @param returned
 * @return true on success, false if s is not an integer
 */
bool ToInt(
	const char* s,
	int32_t* returned
);

/**
 * convert string s to an unsigned integer and return result in value <br />
 * @param s
 * @param returned
 * @return true on success, false if s is not an unsigned integer
 */
bool ToUInt(
	const char* s,
	uint32_t* returned
);

/**
 * convert string s to a float and return result in value. <br />
 * @param s
 * @param returned
 * @return true on success, false if s is not a float.
 */
bool ToFloat(
	const char* s,
	float* returned
);

/**
 * Converts the given value to a string. <br />
 * This is slower than the To____ methods but is more flexible. <br />
 * @param value
 * @return value as a string.
 */
template < typename T >
::std::string From(const T& value)
{
	std::ostringstream str;
	str << value;
	return str.str();
}

/**
 * Convert a string containing substrings separated by delimiter to vector of substrings <br />
 * Example: s is "1,abc,3000"   Results will be vector of 3 strings:   "1" "abc" "3000" <br />
 * @param s the string with substrings separated by delimiter.
 * @param delimiter item separator (e.g. ',').
 * @param trimLeadingSpaces if true, removes leading spaces from substrings.
 * @return vector of substrings.
 */
StdStrings Parse(
	const ::std::string& s,
	char delimiter = ',',
	bool trimLeadingSpaces = true
);

/**
 * Take a vector of strings and output as a single string with delimiter separating the strings. <br />
 * @param v vector of .
 * @param delimiter item separator (e.g. ',').
 * @param trimLeadingSpaces if true, removes leading spaces from substrings.
 * @return a single string with all strings from v.
 */
::std::string FromVectorOfStrings(
	const StdStrings& v,
	char delimiter = ',',
	bool trimLeadingSpaces = true
);

/**
 * Take a vector of char*s and outputs a single string with delimiter separating the strings. <br />
 * @param v
 * @param delimiter
 * @param trimLeadingSpaces
 * @return
 */
::std::string FromVectorOfStrings(
	const CharStrings& v,
	char delimiter = ',',
	bool trimLeadingSpaces = true
);

/**
 * Takes a vector of ::std::strings and converts it to a vector of const char*. <br />
 * @param strings
 * @return strings as CharStrings.
 */
CharStrings ToCharStrings(const StdStrings& strings);

/**
 * Takes a vector of const char*s and converts it to a vector of ::std::strings. <br />
 * @param strings
 * @return strings as StdStrings
 */
StdStrings ToStdStrings(const CharStrings& strings);

/**
 * Copies the contents of source into a new const char* and sets target to point to the new value. 
 * @param source
 * @param target
 */
void CloneInto(
	const char* source,
	const char*& target
);

/**
 * A string at a level. <br />
 */
struct Echelon
{
	const char* string;
	unsigned int echelon;
};
typedef ::std::vector< Echelon > Echelons;


/**
 * This is a simple wrapper around strcmp.
 * @param str1
 * @param str2
 * @return whether or not str1 == str2
 */
bool AreEqual(const char* str1, const char* str2);

} //string namespace
} //bio namespace
