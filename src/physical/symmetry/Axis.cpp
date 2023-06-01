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

#include "bio/physical/symmetry/Axis.h"
#include "bio/physical/symmetry/Symmetry.h"
#include "bio/physical/wave/Wave.h"

namespace bio {
namespace physical {

Axis::Axis()
{

}

Axis::~Axis()
{

}

/*static*/ ::std::string Axis::Failed()
{
	return "FAILED";
}

std::string Axis::Rotate(const Symmetry* symmetry) const
{
	return Encode(symmetry);
}

Symmetry* Axis::Rotate(std::string) const
{
	return NULL;
}

std::string Axis::operator|(Wave* particle) const
{
	BIO_SANITIZE(particle,,return Failed());
	return Rotate(particle->Spin());
}

Symmetry* Axis::operator()(std::string encoded) const
{
	return Rotate(encoded);
}

std::string Axis::Encode(const Symmetry* symmetry) const
{
	return Failed();
}

} //physical namespace
} //bio namespace
