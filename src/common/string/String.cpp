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

#include "bio/common/string/String.h"

namespace bio {

/*static*/ const char* String::GetCloneOf(const char* source, ::std::size_t length)
{
	BIO_SANITIZE(source, , return NULL)
	BIO_SANITIZE(length <= strlen(source), , return NULL)

	if (!length)
	{
		length = strlen(source);
	}
	char* clone = new char[length + 1];
	strncpy(
		clone,
		source,
		length
	);
	clone[length] = '\0';
	return clone;
}

/*static*/ String String::SetMode(const String& string, Mode desiredMode)
{
	String ret;
	switch(desiredMode)
	{
		case READ_ONLY:
			ret.mMode = READ_ONLY;
			ret.mString = string.mString;
			ret.mLength = string.mLength;
			break;
		case COPY_ON_WRITE:
			ret.mMode = COPY_ON_WRITE;
			ret.mString = string.mString;
			ret.mLength = string.mLength;
			break;
		case READ_WRITE:
			ret.mMode = READ_WRITE;
			ret.mString = GetCloneOf(string.mString);
			ret.mLength = string.mLength;
			break;
		default:
			break;
	}
	return ret;
}

String::String(Mode mode) :
	ImmutableString(NULL, 0),
	mMode(mode)
{

}

String::String(const char* string) :
	ImmutableString(string, string ? strlen(string) : 0),
	mMode(READ_ONLY)
{

}

String::String(::std::string string) :
	ImmutableString(GetCloneOf(string.c_str()), strlen(mString)),
	mMode(READ_WRITE)
{

}

String::String(const ImmutableString& string):
	ImmutableString(string.mString, string.mLength),
	mMode(READ_ONLY)
{

}

String::String(const String& toCopy) :
	ImmutableString(toCopy.mMode == READ_WRITE ? GetCloneOf(toCopy.mString) : toCopy.mString, toCopy.mLength),
	mMode(toCopy.mMode)
{

}

#if BIO_CPP_VERSION >= 11
String::String(const String&& toMove):
	ImmutableString(toMove.mString, toMove.mLength),
	mMode(toMove.mMode)
{

}
#endif

String::~String()
{
	//if we move delete[] out of this function the whole ImmutableString class can be merged into this.

	Clear(); //NOT VIRTUAL!
	mString = NULL;
}

#if BIO_CPP_VERSION >= 11
String& String::operator=(const String&& toMove)
{
	Clear();
	mString = toMove.mString;
	mMode = toMove.mMode;
	mLength = toMove.mLength;
	return *this;
}
#endif

String& String::operator=(const String& toCopy)
{
	switch(mMode)
	{
		case READ_WRITE:
			Clear();
			mString = GetCloneOf(toCopy.mString);
			break;

		default:
			mString = toCopy.mString;
			break;
	}
	mLength = toCopy.mLength;

	return *this;
}

const String& String::operator=(const String& toMoveHack) const
{
	String* hack = const_cast< String* >(this);
	if (mMode == READ_WRITE && mString)
	{
		delete[] hack->mString;
	}

	hack->mString = toMoveHack.mString;
	hack->mMode = toMoveHack.mMode;
	hack->mLength = toMoveHack.mLength;
	return *this;
}

String& String::operator=(const ImmutableString& toAssign)
{
	switch(mMode)
	{
		case READ_WRITE:
			Clear();
			mString = GetCloneOf(toAssign.mString);
			break;

		default:
			mString = toAssign.mString;
			break;
	}
	mLength = toAssign.mLength;

	return *this;
}

String& String::operator=(const char* string)
{
	switch(mMode)
	{
		case READ_WRITE:
			Clear();
			mString = GetCloneOf(string);
			mLength = strlen(mString);
			break;

		default:
			mString = string;
			mLength = strlen(mString);
			break;
	}

	return *this;
}

String& String::operator=(::std::string string)
{
	Clear();
	mString = GetCloneOf(string.c_str());
	mMode = READ_WRITE;
	mLength = strlen(mString);
	return *this;
}

bool String::operator==(const ImmutableString& other) const
{
	return mLength == other.mLength && !strncmp(mString, other.mString, mLength);
}

bool String::operator==(const String& other) const
{
	return mLength == other.mLength && !strncmp(mString, other.mString, mLength);
}

String::operator ::std::string() const
{
	return ::std::string(mString).substr(0, mLength);
}

String::operator bool() const
{
	return mString;
}

String::Mode String::GetMode() const
{
	return mMode;
}

String String::SubString(::std::size_t start, ::std::size_t length) const
{
	String ret(GetImmutableSubString(start, length));
	ret.mMode = READ_ONLY;
	return ret;
}

::std::string String::AsStdString() const
{
	return ::std::string(mString);
}

const char* String::AsCharString() const
{
	return GetCloneOf(mString);
}

bool String::AsBool() const
{
	BIO_SANITIZE(mString, , return false)

	return !strncasecmp(
		"true",
		mString,
		mLength
	);
}

int32_t String::AsInt() const
{
	BIO_SANITIZE(mString, , return 0)

	char* endptr = const_cast< char* >(&mString[mLength]);
	return strtol(
		mString,
		&endptr,
		10
	);
}

uint32_t String::AsUInt() const
{
	BIO_SANITIZE(mString, , return 0)

	char* endptr = const_cast< char* >(&mString[mLength]);
	return strtoul(
		mString,
		&endptr,
		10
	);
}

float String::AsFloat() const
{
	BIO_SANITIZE(mString, , return 0.0f)

	char* endptr = const_cast< char* >(&mString[mLength]);
	return strtof(
		mString,
		&endptr
	);
}

void String::Clear()
{
	if (mMode == READ_WRITE && mString)
	{
		delete[] mString;
	}
}

} //bio namespace
