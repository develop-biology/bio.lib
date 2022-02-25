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

#include "bio/physical/Axis.h"

namespace bio {
namespace chemical {

class Symmetry;

/**
 * chemical::Axes simplify the Axis definition process by leveraging the nested nature of chemical::Symmetry.
 * All you need to do to make an Axis using chemical::Axis is define Encode(Symmetry*) and Rotate(std::string).
 */
class Axis :
	public physical::Axis
{
public:
	/**
	 *
	 */
	Axis();

	/**
	 *
	 */
	virtual ~Axis();

	/**
	 * Express the given Symmetry into something intelligible by another system.
	 * Takes advantage of Encode and chemical::Symmetry to make defining Axes easier.
	 * You shouldn't have to override this from here on out.
	 * NOTE: this does not apply to Rotate(std::string); you'll still need to override that one.
	 * @param symmetry
	 * @return text, possibly a command (like mysql), structured values (like json), an entire program (like python), etc. or Failed().
	 */
	virtual std::string Rotate(Symmetry* symmetry) const;

};

} //chemical namespace
} //bio namespace
