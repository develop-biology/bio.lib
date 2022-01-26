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

#include "bio/chemical/Axis.h"
#include "bio/chemical/Symmetry.h"
#include "bio/chemical/structure/StructuralComponent.h"

namespace bio {
namespace chemical {

Axis::Axis()
{

}

Axis::~Axis()
{

}

std::string Axis::Rotate(Symmetry* symmetry) const
{
	std::string ret = "";
	ret += Encode(symmetry);
	typename StructuralComponent< Symmetry* >::Contents* toRotate = symmetry->GetAll< Symmetry* >();
	for (
		typename StructuralComponent< Symmetry* >::Contents::const_iterator sym = toRotate->begin();
		sym != toRotate->end();
		++sym
		)
	{
		ret += Rotate(*sym);
	}

	return ret;
}

} //chemical namespace
} //bio namespace
