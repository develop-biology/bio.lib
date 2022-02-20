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

#include "bio/molecular/Pathway.h"
#include "bio/physical/common/Class.h"

namespace bio {
namespace genetic {

/**
 * GeneToProtein is a chemical::Reaction that takes a Gene [0] & a genetic::Expressor [1] and produces a functional Protein (assuming no Reactions fail).
 * It then goes through the Process of:
 * 1. Transcribing the given Gene.
 * 2. Translating the Produced mRNA.
 * 3. RecruitingChaperones for the Produced Protein.
 * 4. Folding the Produced Protein.
 */
class GeneToProtein :
	public molecular::Pathway,
	public physical::Class<GeneToProtein>
{
public:
	/**
	 *
	 */
	GeneToProtein();

	/**
	 *
	 */
	virtual ~GeneToProtein();
};

} //genetic namespace
} //bio namespace
