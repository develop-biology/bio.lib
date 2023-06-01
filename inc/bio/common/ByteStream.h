/*
 * This file is a part of the Biology project by eons LLC.
 * Biology (aka Develop Biology) is a framework for approaching software
 * development from a natural sciences perspective.
 *
 * Copyright (C) 2023 Séon O'Shannon & eons LLC
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
#include "bio/common/string/String.h"
#include "bio/common/type/IsPointer.h"
#include "bio/common/type/TypeName.h"
#include <cstddef>
#include <cstdlib>
#include <cstring>

namespace bio {

/**
 * Generic byte stream class. Kinda like a void* that you can save and cast later. <br />
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
		mHolding(false),
		mTypeName(String::READ_ONLY)
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

	//@formatter:off
	#if BIO_CPP_VERSION < 17
	/**
	 * Without a move constructor, we cannot use reference types, like const Name&. <br />
	 * AsImplementation provides a backwards compatible solution. <br />
	 * @tparam T
	 */
	template < typename T >
	struct AsImplementation
	{
		AsImplementation(void* stream) : mStream(stream) {}
		T* Get() {return (T*)mStream;}
		void* mStream;
	};
	template < typename T >
	struct AsImplementation< T& >
	{
		AsImplementation(void* stream) : mStream(stream) {}
		T* Get() {return (T*)mStream;}
		void* mStream;
	};
	#endif
	//@formatter:on

	/**
	 * Casts stored data to T. <br />
	 * @tparam T
	 * @return stored bytes as T.
	 */
	template < typename T >
	T& As()
	{
		BIO_ASSERT(Is< T >())

		//@formatter: off
		#if BIO_CPP_VERSION < 17
			return *AsImplementation< T >(mStream).Get();
		#else
			return *(T*)mStream;
		#endif
		//@formatter:on
	}

	/**
	 * Casts stored data to T. <br />
	 * @tparam T
	 * @return stored bytes as T.
	 */
	template < typename T >
	const T& As() const
	{
		//@formatter: off
		#if BIO_CPP_VERSION < 17
			return *AsImplementation< T >(mStream).Get();
		#else
			return *(T*)mStream;
		#endif
		//@formatter:on
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
		//TODO: Investigate warning: source of this 'memcpy' call is a pointer to dynamic class; vtable pointer will be copied.
		std::memcpy(
			mStream,
			&in,
			sizeof(T));
		mSize = sizeof(T);
		mTypeName = type::TypeName< T >();
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
		if (sizeof(T) != mSize)
		{
			return false;
		}
		return mTypeName == type::TypeName< T >();

		//NOTE: You may have a type T which might be a pointer to either a parent or a child class of what you keep in mStore. How do you know if what you have is convertable to T without access to the actual type of the data you store?
		//ANSWER: You don't care. If the caller tries to pull anything out of *this besides what they put in, the caller is wrong and should be notified.
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
	String GetTypeName() const;

	/**
	 * @return the number of bytes *this points to.
	 */
	std::size_t GetSize() const;

	/**
	 * This is yet another strange and hacky function in the ByteStream menagerie. <br />
	 * If a ByteStream is passed by reference to a function which copies the data of the ByteStream, the temporary variable needs to stop mHolding its mStream so that the copy can be in charge of Releasing the Held data. <br />
	 * Otherwise, once the ByteStream is copied elsewhere (e.g. into a Container) and then the original ByteStream goes out of scope, the persisted copy's mStream will be freed. <br />
	 * To address optimized hand-off situations like this, you can TakeHold of a ByteStream, which will prevent it from Releasing its contents. <br />
	 * As with other methods of this class, don't use this. Just stop. Unless you know exactly what your doing and where your memory is going, pretend this doesn't exist and find a sane solution to whatever problem you have (e.g. create a new ByteStream by copying the reference). <br />
	 * It is UNDEFINED BEHAVIOR to use a ByteStream after its Hold has been Taken. <br />
	 */
	void TakeHold();

	/**
	 * Assume the caller knows something we don't. <br />
	 * Please don't use this. <br />
	 * @return the data in *this
	 */
	void* DirectAccess();

protected:
	mutable void* mStream;
	String mTypeName;
	std::size_t mSize;
	bool mHolding;
};
} //bio namespace
