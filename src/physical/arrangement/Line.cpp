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

#include "bio/physical/arrangement/Line.h"

namespace bio {
namespace physical {

Line::Line(Index expectedSize)
	:
	TypeOptimizedArrangement< Linear >(expectedSize)
{

}

Line::~Line()
{

}

ByteStream Line::Access(const Index index)
{
	return Cast< Identifiable< StandardDimension >* >(TypeOptimizedArrangement< Linear >::Access(index).template As< Linear >());
}

const ByteStream Line::Access(const Index index) const
{
	return Cast< Identifiable< StandardDimension >* >(TypeOptimizedArrangement< Linear >::Access(index).template As< Linear >());
}

bool Line::AreEqual(Index internal, const ByteStream external) const
{
	BIO_SANITIZE(external.Is< Identifiable< StandardDimension >* >(),,return false)
	return TypeOptimizedArrangement< Linear >::Access(internal).template As< Linear >() == external.template As< const Identifiable< StandardDimension >* >();
}

} //physical namespace
} //bio namespace
