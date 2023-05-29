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

#include "bio/neural/common/Types.h"

namespace bio {
namespace feature {

/**
 * Prevents *this from being removed due to expiration time. <br />
 * NOTE: this has no effect on DepotentiateSignal calls. <br />
 * By default, Synapses will NOT expire after a timeout. <br />
 */
Feature Timeout();

/**
 * The following methods control whether or not a datum with value 0 should be allowed to propagate through *this. <br />
 * It is up to the methods within *this (or children) to respect this. <br />
 * Zeros are NOT allowed by default. <br />
 */
Feature PossibleZeroValue();

/**
 * If your Synapse defies the typical addition / removal structure (changes applied by removing old and adding new), then this Feature will allow your Synapse to bypass the is-already-added check imposed by the target Neuron. <br />
 * This is useful for synapses that build up over time. <br />
 */
Feature MultiplePotentiations();

/**
 * Inverse of AllowMultiplePotentiations. <br />
 * If your Synapse defies the typical addition / removal structure (changes applied by removing old and adding new), then this method will allow your Synapse to bypass the is-already-added check imposed by the target Neuron. <br />
 * This is useful for synapses that build up over time. <br />
 */
Feature MultipleDepotentiations();

} //feature namespace
} //bio namespace
