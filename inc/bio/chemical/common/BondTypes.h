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

#include "Types.h"

namespace bio {
namespace bond_type {

/**
 * Signals a bond is not pointing to anything. <br />
 */
BondType Empty();

/**
 * Default if no type is specified. <br />
 */
BondType Unknown();

/**
 * Virtual() Bonds represent a sort of pseudo-vtable where child classes can store pointers to themselves in the Atom parent such that they can be down and cross casted without RTTI. <br />
 */
BondType Virtual();

/**
 * Covalent() Bonds form the same pseudo-vtable as Virtual and can be cross casted without RTTI; however, they are distinct objects which end the virtual overrides of their parents. <br />
 * Use the Horizontal class to create Covalent Bonds.
 */
BondType Covalent();


/**
 * AutoDelete Bonds will delete whatever they've Bonded when they are destroyed. <br />
 * This is the only kind of bond with this behavior at the Bond level (you are obviously free to call delete on the Bonded object yourself). <br />
 */
BondType AutoDelete();

} //bond_type namespace
} //bio namespace
