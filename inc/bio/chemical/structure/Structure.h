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

#include "bio/chemical/structure/interface/UnorderedStructureInterface.h"
#include "bio/chemical/structure/interface/LinearStructureInterface.h"

namespace bio {
namespace chemical {

/**
 * Chemical structures can have Motifs.
 * Here, we implement all the necessary interfaces for adding Motifs to your classes.
 *
 * This class should be virtually inherited.
 *
 * Motifs should be inherited (but can be added through Bonding). The reason that we break the "has a motif" idea and instead go for "is a motif" inheritance style is that chemical compositions define the identity of chemical structures. So if a chemical substance would change its identity by containing different motifs, it makes sense that the motifs define the class structure of the substance.
 */
class Structure :
	public UnorderedStructureInterface,
	public LinearStructureInterface
{
public:

	/**
	 *
	 */
	Structure();

	/**
	 *
	 */
	virtual ~Structure();
};

} //chemical namespace
} //bio namespace
