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

#include "bio/cellular/common//Types.h"
#include "bio/chemical/structure/motif/LinearMotif.h"

namespace bio {

/**
 * Potentials are mutable components of neural::Cells. <br />
 * These are changed during operation. <br />
  */
BIO_ID_WITH_PERSPECTIVE(Potential, Id::Type)

/**
 * Feature flags. <br />
 * These are not TranscriptionFactors and should only deal with configuration, rather than purpose. <br />
 */
BIO_ID_WITH_PERSPECTIVE(Feature, Id::Type)

namespace neural {

BIO_ID_WITH_PERSPECTIVE(FiringCondition, uint8_t)

/**
 * Forward declarations to share across classes
 */
class Brain;

class Neuron;
typedef chemical::LinearMotif< Neuron* >::Contents Neurons;

class Neuropil;
typedef chemical::LinearMotif< Neuropil* >::Contents Neuropils;

class Dendrite;
typedef chemical::LinearMotif< Dendrite* >::Contents Dendrites;

class Axon;
typedef chemical::LinearMotif< Axon* >::Contents Axons;

class Synapse;
typedef chemical::LinearMotif< Synapse* >::Contents Synapses;

class MembranePotential;
typedef chemical::LinearMotif< MembranePotential* >::Contents MembranePotentials;

} //neural namespace
} //bio namespace
