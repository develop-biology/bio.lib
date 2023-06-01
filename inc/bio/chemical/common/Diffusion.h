/*
 * This file is a part of the Biology project by eons LLC.
 * Biology (aka Develop Biology) is a framework for approaching software
 * development from a natural sciences perspective.
 *
 * Copyright (C) 2023 Séon O'Shannon & eons LLC
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
namespace diffusion {

namespace time {

/**
 * Do not Diffuse.
 */
DiffusionTime Never();

/**
 * Diffuse only upon Solute Destruction.
 * Useful for short-lived Solutes. <br />
 */
DiffusionTime Destruction();

/**
 * Diffuse regularly. <br />
 * Useful for long-lived Solutes. <br />
 */
DiffusionTime Interval();

} //time namespace

namespace effort {

/**
 * Diffuse against the Concentration gradient, toward the source of the originally Dissolved Substance. <br />
 * This syncs changes in downstream, Effluxed Solutes back upstream. <br />
 */
DiffusionEffort Active();

/**
 * Diffuse with the Concentration gradient, toward Effluxed Solutes. <br />
 * This syncs changes in the upstream Dissolved Substance toward downstream copies. <br />
 */
DiffusionEffort Passive();

/**
 * Diffuse both with and against the Concentration gradient. <br />
 * This syncs changes in the upstream Dissolved Substance toward downstream copies, and vice versa. <br />
 */
DiffusionEffort ActiveAndPassive();

} //effort namespace

} //diffusion namespace
} //bio namespace
