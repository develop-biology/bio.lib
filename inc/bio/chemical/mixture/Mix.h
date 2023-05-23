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

#include "bio/chemical/reaction/Reaction.h"

namespace bio {
namespace chemical {

/**
 * Mix is used by Solutes to combine within Solutions. However, you are welcome to mix un-Dissolved Substances as well. <br />
 */
class Mix :
	public chemical::Class< Mix >,
	public Reaction
{
public:

	/**
	 * Ensure virtual methods point to Class implementations. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(chemical, Mix)

	/**
	 *
	 */
	Mix();

	/**
	 *
	 */
	virtual ~Mix();

	/**
	 * Mixing 2 (or more) Reactants will recursively invoke all available miscible Reactions according to the Miscibilities of the first Reactant. <br />
	 * @param reactants
	 * @return All given Reactants after the first has been modified; all other Reactants should be left untouched (but this is not guaranteed).
	 */
	virtual Products Process(Reactants* reactants) const;

	/**
	 * All Reactants can be mixed.
	 * @param toCheck
	 * @return true iff there are 2 or more Reactants.
	 */
	virtual bool ReactantsMeetRequirements(const Reactants* toCheck) const;
};

} //chemical namespace
} //bio namespace
