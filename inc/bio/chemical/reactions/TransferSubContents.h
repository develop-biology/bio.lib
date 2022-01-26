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

#include "bio/chemical/Class.h"
#include "bio/chemical/Reaction.h"

namespace bio {
namespace chemical {

/**
 * TransferSubContents uses the ImportAll method from StructuralComponent to copy the contents of the 2nd Reactant into the 1st (i.e. 1->ImportAll(2)).
 * Both Reactants must have be state::Enabled.
 */
class TransferSubContents :
	virtual public Reaction,
	public Class< TransferSubContents >
{
public:
	/**
	 *
	 */
	TransferSubContents();

	/**
	 *
	 */
	virtual ~TransferSubContents();

	/**
	 * Do the actual work.
	 * See Reaction.h (in bio/chemical/) for more info and the class description for what *this does.
	 * @param reactants
	 * @return reactants
	 */
	virtual Products Process(Substances& reactants);

} //chemical namespace
} //bio namespace
