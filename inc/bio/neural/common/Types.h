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

#include "bio/common/Types.h"
#include "bio/common/NameTracker.h"

namespace bio {

BIO_ID_WITH_TRACKER(Neuron, uint16_t)

/**
Synapses are stored in the Brain and applied to neurons.
There can be up to 65535 different kinds of synapses.
NOTE: that this does not prevent you from configuring a synapse after it has been created (e.g. adding a epitope from one synapse to another). However, it is better practice to configure all synapses before they are used.
*/
BIO_ID_WITH_TRACKER(Synapse, uint16_t)

/**
Aspects are mutable components of a Neuron.
These are changed during operation (as opposed to other components, like Synapses)
*/
BIO_ID_WITH_TRACKER(Aspect, uint16_t)

/**
Forward declarations to share across classes
*/
class Brain;
class Neuron;
typedef std::vector<Neuron*> Neurons;
typedef std::vector<const Neuron*> ConstNeurons;
class Synapse;
typedef std::vector<Synapse*> Synapses;

} //bio namespace
