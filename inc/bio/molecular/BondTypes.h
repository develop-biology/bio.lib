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

#include "Types.h"

namespace bio {
namespace bond_type {

/**
 * An Manage Bond should be used for basic types by a Surface.
 * This will instruct the Surface to take ownership of the Managed Wave*, tying the Wave*s life cycle to the Managing Surface. In other words, Managed objects will be deleted by the Manager (the Atom with the Bond).
 */
BondType Manage();

/**
 * A Use Bond serves the same purpose as a Manage Bond in that it is intended to instruct Surfaces on how to handle the Bonded Wave.
 * However, Use Bonds will not be deleted when the User is destroyed.
 */
BondType Use();

} //bond_type namespace
} //bio namespace