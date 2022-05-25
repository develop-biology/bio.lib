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
#include "TypeName.h"
#include <cstddef>
#include <cstdlib>
#include <cstring>

namespace bio {

/**
 * Generic byte stream class. Kinda like a void* that you can save and cast later. 
 * Work around for c++98 auto keyword and other wonky problems. <br />
 *
 *******************************************************************************<br />
 *                    DO NOT USE THIS IMPROPERLY!!                              <br />
 *******************************************************************************<br />
 *
 * If you don't understand what this does and how it CAN GO HORRIBLY WRONG, <br />
 * DO NOT USE THIS CLASS! <br />
 *
 * This is used by BIO_SANITIZE_WITH_CACHE and Containers. <br />
 *
 * NOTE: ByteStreams are not virtual to save what space we can. This may change in a future release if we decide we somehow need more hacky, abstract storage. <br />
 */
class ByteStream
{
public:
	/**
	 * DON'T USE THIS UNLESS YOU KNOW WHAT YOU'RE DOING <br />
	 */
	ByteStream();

	template < typename T >
	ByteStream(T in)
		:
		mHolding(false)
	{
		Set(in);
	}

	/**
	 * @param other
	 */
	ByteStream(const ByteStream& other);

	/**
	 *
	 */
	~ByteStream();

	/**
	 * @param other
	 */
	void operator=(const ByteStream& other);

	/**
	 * Compares the memory contained in both *this and other. <br />
	 * @param other
	 * @return whether or not other holds the same bits as *this.
	 */
	bool operator==(const ByteStream& other) const;

	/**
	 * Casts stored data to T. <br />
	 * @tparam T
	 * @return stored bytes as T.
	 */
	template < typename T >
	T As()
	{
		BIO_ASSERT(Is< T >());
		return *(T*)mStream;
	}

	/**
	 * Casts stored data to T. <br />
	 * @tparam T
	 * @return stored bytes as T.
	 */
	template < typename T >
	const T As() const
	{
		BIO_ASSERT(Is< T >());
		return *(T*)mStream;
	}

	/**
	 * Casts stored data to T. <br />
	 * @tparam T
	 * @return stored bytes as T.
	 */
	template < typename T >
	operator T()
	{
		return As< T >();
	}

	/**
	 * Casts stored data to T. <br />
	 * @tparam T
	 * @return stored bytes as T.
	 */
	template < typename T >
	operator const T() const
	{
		return As< T >();
	}

	/**
	 * Copies the data given to a new memory location. <br />
	 * This should be used if the provided "in" is expected to go out of scope but the value still be valid. <br />
	 * Make sure you Release *this to delete the stored content. <br />
	 * @tparam T
	 * @param in data to store
	 */
	template < typename T >
	void Set(T in)
	{
		Release();
		mStream = ::std::malloc(sizeof(T));
		std::memcpy(
			mStream,
			&in,
			sizeof(T));
		mSize = sizeof(T);
		mTypeName = TypeName< T >();
		mHolding = true;
	}

	/**
	 * Copies the data from an other into *this and Holds it. <br />
	 */
	void Set(const ByteStream& other);

	/**
	 * Frees the memory *this was Holding. <br />
	 * Nop if *this was not holding anything. <br />
	 * NOTE: This does not call any destructors. You must do that yourself. <br />
	 * (i.e. there is no typename -> new type* -> union -> delete; delete) 
	 */
	void Release();

	/**
	 * Check if *this has been Set. <br />
	 * @return Whether or not *this points to any possibly valid data.
	 */
	bool IsEmpty() const;

	/**
	 * Check if Set was called with T. <br />
	 * @tparam T
	 * @return whether or not *this should be pointing to data of type T.
	 */
	template < typename T >
	bool Is() const
	{
		return sizeof(T) == mSize && strcmp(TypeName< T >(), mTypeName) == 0;
	}

	/**
	 * Auto template determining version of Is<T>(). <br />
	 * @tparam T
	 * @param t only used for automatically determining T.
	 * @return whether or not *this should be pointing to data of type T.
	 */
	template < typename T >
	bool Is(const T& t) const
	{
		return Is< T >();
	}

	/**
	 * @return the type stored in *this as a string.
	 */
	const char* GetTypeName() const;

	/**
	 * @return the number of bytes *this points to.
	 */
	std::size_t GetSize() const;

	/**
	 * Assume the caller knows something we don't. <br />
	 * Please don't use this. <br />
	 * @return the data in *this
	 */
	void* DirectAccess();

protected:
	mutable void* mStream;
	const char* mTypeName;
	std::size_t mSize;
	bool mHolding;
};
} //bio namespace
