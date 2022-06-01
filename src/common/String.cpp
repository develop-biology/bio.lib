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

#include "bio/common/String.h"

#include <cstring>
#include <cstdlib>

namespace bio {

String::String()
{

}

String::String(const char* string) :
	mString(string ? string : "")
{

}

String::String(::std::string string) :
	mString(string)
{

}

String::String(const String& toCopy) :
	mString(toCopy)
{

}

String::~String()
{

}

String& String::operator=(const char* string)
{
	if (!string)
	{
		mString = "";
	}
	else
	{
		mString = string;
	}
	return *this;
}

String& String::operator=(::std::string string)
{
	mString = string;
	return *this;
}

bool String::operator==(const String& other) const
{
	return mString == other.mString;
}

String::operator ::std::string() const
{
	return mString;
}

String::operator bool() const
{
	return mString.size();
}

::std::string String::AsStdString() const
{
	return mString;
}

const char* String::AsCharString() const
{
	return GetCloneOf(mString.c_str());
}

bool String::AsBool() const
{
	BIO_SANITIZE(mString.size(),
		,
		return false
	)

	return !strcasecmp(
		"true",
		mString.c_str()
	);
}

int32_t String::AsInt() const
{
	BIO_SANITIZE(mString.size(),
		,
		return 0
	)

	char* endptr = NULL;
	return strtol(
		mString.c_str(),
		&endptr,
		10
	);
}

uint32_t String::AsUInt() const
{
	BIO_SANITIZE(mString.size(),
		,
		return 0
	)

	char* endptr = NULL;
	return strtoul(
		mString.c_str(),
		&endptr,
		10
	);
}

float String::AsFloat() const
{
	BIO_SANITIZE(mString.size(),
		,
		return 0.0f
	)

	char* endptr = NULL;
	return strtof(
		mString.c_str(),
		&endptr
	);
}

/*static*/ const char* String::GetCloneOf(const char* source)
{
	const size_t len = strlen(source);
	char* tmpName = new char[len + 1];
	strncpy(
		tmpName,
		source,
		len
	);
	tmpName[len] = '\0';
	return tmpName;
}

} //bio namespace
