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

#include "bio/physical/arrangement/Line.h"

namespace bio {
namespace physical {

Line::Line(Position expectedSize)
	:
	TypeOptimizedArrangement< Linear >(expectedSize)
{

}

Line::~Line()
{

}

ByteStream Line::Access(const Index index)
{
	BIO_SANITIZE(IsInRange(index),,return NULL)
	return Cast< Identifiable< StandardDimension >* >(TypeOptimizedArrangement< Linear >::Access(index));
}

const ByteStream Line::Access(const Index index) const
{
	return Cast< Identifiable< StandardDimension >* >(TypeOptimizedArrangement< Linear >::Access(index));
}

bool Line::AreEqual(Index internal, ByteStream external)
{
	Cast< Linear >(Access(internal)) == Cast< Identifiable< StandardDimension >* >(external.IKnowWhatImDoing());
}

} //physical namespace
} //bio namespace
