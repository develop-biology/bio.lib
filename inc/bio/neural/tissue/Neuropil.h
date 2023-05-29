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
#include "bio/neural/common/Class.h"
#include "bio/neural/common/Filters.h"
#include "bio/neural/macro/Macros.h"
#include "bio/cellular/Tissue.h"

namespace bio {
namespace neural {

class AxonGuide;

/**
 * Neuropils are simply Tissues which hold Neurons & Synapses. <br />
 * They have a number of useful methods for creating and managing Neurons & Synapses. <br />
 * You are not required to use a Neuropil over a Tissue, but it is recommended. <br />
 */
class Neuropil:
	public neural::Class< Neuropil >,
	public cellular::Tissue
{
public:

	/**
	 * Ensure virtual methods point to Class implementations. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(neural, Neuropil)

	BIO_DEFAULT_IDENTIFIABLE_CONSTRUCTORS(
		neural,
		Neuropil,
		filter::Neural()
	)

	/**
	 *
	 */
	virtual  ~Neuropil();

	/**
	 * Use this method to populate any member variable Protein*s. <br />
	 * You'll want to do this to speed up your code by bypassing the dynamic execution provided by genetic::Expressor. <br />
	 */
	virtual Code CacheProteins();

	/**
	 * If you use CacheProteins, you'll likely want to create your default Proteins here. <br />
	 * This will prevent dereferencing null or garbage pointers when using your cached Proteins. <br />
	 */
	virtual Code CreateDefaultProteins();

	virtual Synapse* Connect(
		Neuron* presynapticNeuron,
		Neuron* postsynapticNeuron,
		const Synapse* synapse
	);

	virtual Synapse* Connect(
		const Name& presynapticNeuron,
		const Name& postsynapticNeuron,
		const Name& synapse
	);
	
	virtual Synapses Connect(
		Neuropil* presynapticNeuropil,
		Neuropil* postsynapticNeuropil,
		const Synapse* synapse,
		AxonGuide* guide,
		const Affinity* presynapticNeuronAffinity = NULL,
		const Affinity* postsynapticNeuronAffinity = NULL
	) const;

	virtual Synapses Connect(
		const Synapse* synapse,
		AxonGuide* guide,
		const Affinity* presynapticNeuronAffinity = NULL,
		const Affinity* postsynapticNeuronAffinity = NULL
	);

	virtual Synapses CreateDendritesFrom(
		Neuron* presynapticNeuron,
		const Synapse* synapse,
		AxonGuide* guide,
		const Affinity* postsynapticNeuronAffinity = NULL
	);

	virtual Synapses CreateDendritesFrom(
		Neuropil* presynapticNeuropil,
		const Synapse* synapse,
		AxonGuide* guide,
		const Affinity* presynapticNeuronAffinity = NULL,
		const Affinity* postsynapticNeuronAffinity = NULL
	);

	virtual Synapses CreateAxonsTo(
		Neuron* postsynapticNeuron,
		const Synapse* synapse,
		AxonGuide* guide,
		const Affinity* presynapticNeuronAffinity = NULL
	);

	virtual Synapses CreateAxonsTo(
		Neuropil* postsynapticNeuropil,
		const Synapse* synapse,
		AxonGuide* guide,
		const Affinity* presynapticNeuronAffinity = NULL,
		const Affinity* postsynapticNeuronAffinity = NULL
	);

};

} //neural namespace
} //bio namespace
