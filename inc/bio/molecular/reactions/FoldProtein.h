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

#include "bio/chemical/reaction/Reaction.h"
#include "bio/physical/common/Class.h"

namespace bio {
namespace molecular {

/**
 * FoldProtein is a chemical::Reaction that takes a Protein [0] and Fold()s it.
 */
class FoldProtein :
	public chemical::Reaction,
	public physical::Class<FoldProtein>
{
public:
	/**
	 *
	 */
	FoldProtein();

	/**
	 *
	 */
	virtual ~FoldProtein();

	/**
	 * Do the actual work.
	 * See Reaction.h (in bio/chemical/) for more info and the class description for what *this does.
	 * @param reactants [0] = Protein
	 * @return reactants
	 */
	virtual chemical::Products Process(chemical::Substances& reactants);
};

} //molecular namespace
} //bio namespace
