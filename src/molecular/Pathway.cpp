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

//
// Created by eons on 12/17/21.
//

#include "bio/molecular/Pathway.h"

namespace bio {
namespace molecular {

Pathway::~Pathway()
{

}

chemical::Products Pathway::Process(chemical::Reactants* reactants)
{
	BIO_SANITIZE(reactants, ,
		return code::BadArgument1())

	chemical::Products products(reactants);
	Reaction* reactionBuffer;
	for (
		SmartIterator rct = GetAll< Reaction* >()->Begin();
		!rct.IsAtEnd();
		++rct
		)
	{
		if (products == code::Success() && products != code::NoErrorNoSuccess())
		{
			chemical::Reactants nextReactants(products);
			reactionBuffer = rct;
			products = (*reactionBuffer)(&nextReactants);
		}
		else
		{
			break;
		}
	}
	return products;
}

bool Pathway::ReactantsMeetRequirements(const chemical::Reactants* toCheck) const
{
	BIO_SANITIZE(GetCount< chemical::Reaction* >(), ,
		return false)
	return GetAll< chemical::Reaction* >()->Begin().As< chemical::Reaction* >()->ReactantsMeetRequirements(toCheck);
}

} //molecular namespace
} //bio namespace
