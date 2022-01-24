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

#include "bio/common/ByteStream.h"

namespace bio {

ByteStream::ByteStream() :
	m_stream(NULL),
	m_typeName(""),
	m_size(0)
{
}

ByteStream::ByteStream(const ByteStream& other) :
	m_stream(other.m_stream),
	m_typeName(other.m_typeName),
	m_size(other.m_size)
{
}

ByteStream:: ~ByteStream()
{
}

void ByteStream::operator=(const ByteStream& other)
{
	m_stream = other.m_stream;
	m_typeName = other.m_typeName;
	m_size = other.m_size;
}

bool ByteStream::IsEmpty() const
{
	return !m_stream;
}

std::string ByteStream::GetTypeName() const
{
	return m_typeName;
}

std::size_t ByteStream::GetSize() const
{
	return m_size;
}

void* ByteStream::IKnowWhatImDoing()
{
	return (void*)m_stream;
}

} //bio namespace