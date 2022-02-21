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

#include "bio/chemical/structure/LinearStructuralComponent.h"
#include "Molecule.h"
#include "bio/molecular/common/Types.h"

namespace bio {
namespace molecular {

/**
 * Vesicles define an inside / outside dichotomy between the Surfaces on the "outside" of *this and the Molecules on the inside of *this.
 * NOTE: The Molecules inside *this can, themselves, be Vesicles (thought they don't have to be).
 * Another way to think of Vesicles is as a "vacuum" whereby Molecules can exist in an isolated context (which is probably not true of any real system). For example think of 1 Molecules on one side of a room and another on the other side; the Vesicle would be the Room and what is ultimately responsible for defining the scale by which the Molecule's separation can be measured.
 */
class Vesicle :
	virtual public Molecule,
	public Class<Vesicle>,
	public chemical::LinearStructuralComponent< Molecule >,
{
public:

	/**
	 *
	 */
	Vesicle();

	/**
	 * @param name
	 */
	explicit Vesicle(Name name);

	/**
	 * @param id
	 */
	explicit Vesicle(StandardDimension id);

	/**
	 * @param toCopy
	 */
	Vesicle(const Vesicle& toCopy);

	/**
	 *
	 */
	virtual ~Vesicle();
};

} //molecular namespace
} //bio namespace
