/*
 * This file is a part of the Biology project by eons LLC.
 * Biology (aka Develop Biology) is a framework for approaching software
 * development from a natural sciences perspective.
 *
 * Copyright (C) 2021 Séon O'Shannon & eons LLC
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

#include "bio/common/macros/AssertMacros.h"
#include "TypeName.h"
#include <cstddef>

namespace bio {

/**
 * Generic byte stream class. Kinda like a Cast that you can save and use later.
 * Work around for c++98 auto keyword
 *
 *******************************************************************************
 *                    DO NOT USE THIS IMPROPERLY!!
 *******************************************************************************
 *
 * If you don't understand what this does and how it CAN GO HORRIBLY WRONG,
 * DO NOT USE THIS CLASS!
 *
 * This is used by BIO_SANITIZE_WITH_CACHE
 */
class ByteStream
{
public:
	/**
	 * DON'T USE THIS UNLESS YOU KNOW WHAT YOU'RE DOING
	 */
	ByteStream();

	template<typename T>
	ByteStream(T in)
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
	virtual ~ByteStream();

	/**
	 * @param other
	 */
	virtual void operator=(const ByteStream& other);

	/**
	 * Casts stored data to T.
	 * @tparam T
	 * @return stored bytes as T.
	 */
	template <typename T>
	operator T() const
	{
		BIO_ASSERT(Is<T>());
		union Converter
		{
			char* bytes;
			T type;
		};
		Converter c;
		c.bytes = m_stream;
		return c.type;
	}

	/**
	 * Stores bytes of T in *this.
	 * @tparam T
	 */
	template <typename T>
	void Set(T in)
	{
		union Converter
		{
			char* bytes;
			T type;
		};
		Converter c;
		c.type = in;
		m_stream = c.bytes;
		m_typeName = TypeName<T>();
		m_size = sizeof(T);
	}

	/**
	 * Check if *this has been Set.
	 * @return Whether or not *this points to any possibly valid data.
	 */
	bool IsEmpty() const;

	/**
	 * Check if Set was called with T.
	 * @tparam T
	 * @return whether or not *this should be pointing to data of type T.
	 */
	template <typename T>
	bool Is() const
	{
		return sizeof(T) == m_size && TypeName<T>() == m_typeName;
	}

	/**
	 * Auto template determining version of Is<T>().
	 * @tparam T
	 * @param t only used for automatically determining T.
	 * @return whether or not *this should be pointing to data of type T.
	 */
	template <typename T>
	bool Is(const T& t) const
	{
		return Is<T>();
	}

	/**
	 * @return the type stored in *this as a string.
	 */
	std::string GetTypeName() const;

	/**
	 * @return the number of bytes *this points to.
	 */
	std::size_t GetSize() const;

	/**
	 * Assume the caller knows something we don't.
	 * Please don't use this.
	 * @return the data in *this as void*.
	 */
	void* IKnowWhatImDoing();

protected:
	char* m_stream;
	std::string m_typeName;
	std::size_t m_size;
};
} //bio namespace
