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

//
// Created by eons on 12/17/21.
//

#include "bio/molecular/Pathway.h"

namespace bio {
namespace molecular {

Pathway::Pathway(Name name);

Pathway::~Pathway();

chemical::Products Pathway::Process(chemical::Substances& reactants)
{
	chemical::Products products = reactants;
	for (LinearStructuralComponent<Reaction*>::Contents::iterator rct = GetAll<Reaction*>()->Begin(); rct != GetAll<Reaction*>()->end(); ++rct)
	{
		if (products == code::Success() && products != code::NoErrorNoSuccess())
		{
			products = (**rct)(products);
		}
		else
		{
			break;
		}
	}
	return products;
}

bool Pathway::SubstancesCanReact(const Substances& toCheck) const
{

}

} //molecular namespace
} //bio namespace