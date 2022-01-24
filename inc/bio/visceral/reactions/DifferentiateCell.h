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

#include "bio/chemical/Reaction.h"
#include "bio/physical/Class.h"

namespace bio {
namespace visceral {

/**
 * DifferentiateCell is a chemical::Reaction that takes a Cell [0] & a Tissue [1] and:
 *     1. Injects all Plasmid*s from the Tissue into the Cell.
 *     2. Transcribes & Translates all Genes in the Cell.
 *     3. Folds all Proteins in the Cell.
 */
class DifferentiateCell :
	public chemical::Reaction,
	public physical::Class<DifferentiateCell>
{
public:
	/**
	 *
	 */
	DifferentiateCell();

	/**
	 *
	 */
	virtual ~DifferentiateCell();

	/**
	 * Do the actual work.
	 * See Reaction.h (in bio/chemical/) for more info and the class description for what *this does.
	 * @param reactants [0] = Cell, [1] = Tissue
	 * @return reactants
	 */
	virtual chemical::Products Process(chemical::Substances& reactants);
};

} //visceral namespace
} //bio namespace