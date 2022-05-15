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

#include "bio/common/ByteStream.h"

namespace bio {

ByteStream::ByteStream()
	:
	mStream(NULL),
	mTypeName(""),
	mSize(0),
	mHolding(false)
{
}

ByteStream::ByteStream(const ByteStream& other)
	:
	mHolding(false)
{
	*this = other;
}

ByteStream::~ByteStream()
{
	Release();
}

void ByteStream::operator=(const ByteStream& other)
{
	Release(); //wipe old state.

	if (other.mHolding)
	{
		//We can't free the same memory twice, so we have to allocate a new block for ourselves.
		Set(other);
	}
	else
	{
		mStream = other.mStream;
		mTypeName = other.mTypeName;
		mSize = other.mSize;
		mHolding = false;
	}
}

bool ByteStream::IsEmpty() const
{
	return !mStream;
}

std::string ByteStream::GetTypeName() const
{
	return mTypeName;
}

std::size_t ByteStream::GetSize() const
{
	return mSize;
}

void* ByteStream::DirectAccess()
{
	return mStream;
}

void ByteStream::Set(const ByteStream& other)
{
	Release();
	mStream = ::std::malloc(other.mSize);
	memcpy(
		mStream,
		other.mStream,
		other.mSize
	);
	mSize = other.mSize;
	mTypeName = other.mTypeName;
	mHolding = true;
}

void ByteStream::Release()
{
	if (!mHolding)
	{
		return;
	}
	std::free(mStream);
	mSize = 0;
	mTypeName.clear();
	mHolding = false;
}

bool ByteStream::operator==(const ByteStream& other) const
{
	if (mSize != other.mSize || mTypeName != other.mTypeName)
	{
		return false;
	}
	return memcmp(
		mStream,
		other.mStream,
		mSize
	) == 0;
}

} //bio namespace
