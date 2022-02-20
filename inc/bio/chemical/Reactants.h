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

#include "structure/LinearStructuralComponent.h"
#include "Substance.h"
#include "bio/physical/Class.h"

namespace bio {
namespace chemical {

class Reactant; //unused here but anything including Reactants will likely use Reactant*.

/**
 * Reactants are a multitude of Substances, all combined into a single Wave.
 * NOTE that a Reactant is a Substance, so Reactants may (but do not have to) contain Reactant*s.
 * Reactants is intended to be the single input to a Reaction. In this way, a Reactants represents all the Reacting Substances coming together, making the job of Reaction then to pull them apart into the appropriate Products.
 */
class Reactants :
	public physical::Class< Reactants >,
	public LinearStructuralComponent< Substance* >
{
public:

	/**
	 *
	 */
	Reactants();

	/**
	 * @param substances
	 */
	Reactants(Substances& substances);

	/**
	 *
	 */
	virtual ~Reactants();

	operator Substances();
};

} //chemical namespace
} //bio namespace
