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

#include "bio/neural/tissue/Neuropil.h"
#include "bio/neural/cell/Neuron.h"
#include "bio/neural/protein/AxonGuide.h"

namespace bio {
namespace neural {

Neuropil::~Neuropil()
{

}

Code Neuropil::CacheProteins()
{
	return Tissue::CacheProteins();
}

Code Neuropil::CreateDefaultProteins()
{
	return Tissue::CreateDefaultProteins();
}

Synapse* Neuropil::Connect(
	Neuron* presynapticNeuron,
	Neuron* postsynapticNeuron,
	const Synapse* synapse
)
{
	BIO_SANITIZE(presynapticNeuron && postsynapticNeuron, , return NULL)
	return presynapticNeuron->ConnectTo(postsynapticNeuron, synapse);
}

Synapse* Neuropil::Connect(
	const Name& presynapticNeuron,
	const Name& postsynapticNeuron,
	const Name& synapse
)
{
	return Connect(
		GetByName< cellular::Cell* >(presynapticNeuron)->As< Neuron* >(),
		GetByName< cellular::Cell* >(postsynapticNeuron)->As< Neuron* >(),
		GetByName< cellular::Cell* >(synapse)->As< Synapse* >()
	);
}

Synapses Neuropil::Connect(
	const Synapse* synapse,
	AxonGuide* guide,
	const Affinity* presynapticNeuronAffinity,
	const Affinity* postsynapticNeuronAffinity
)
{
	return Connect(
		this,
		this,
		synapse,
		guide,
		presynapticNeuronAffinity,
		postsynapticNeuronAffinity
	);
}

Synapses Neuropil::CreateDendritesFrom(
	Neuron* presynapticNeuron,
	const Synapse* synapse,
	AxonGuide* guide,
	const Affinity* postsynapticNeuronAffinity
)
{
	Neuropil intermediate;
	intermediate.Add< cellular::Cell* >(presynapticNeuron);
	return Connect(
		&intermediate,
		this,
		synapse,
		guide,
		NULL,
		postsynapticNeuronAffinity
	);
}

Synapses Neuropil::CreateDendritesFrom(
	Neuropil* presynapticNeuropil,
	const Synapse* synapse,
	AxonGuide* guide,
	const Affinity* presynapticNeuronAffinity,
	const Affinity* postsynapticNeuronAffinity
)
{
	return Connect(
		presynapticNeuropil,
		this,
		synapse,
		guide,
		presynapticNeuronAffinity,
		postsynapticNeuronAffinity
	);
}

Synapses Neuropil::CreateAxonsTo(
	Neuron* postsynapticNeuron,
	const Synapse* synapse,
	AxonGuide* guide,
	const Affinity* presynapticNeuronAffinity
)
{
	Neuropil intermediate;
	intermediate.Add< cellular::Cell* >(postsynapticNeuron);
	return Connect(
		this,
		&intermediate,
		synapse,
		guide,
		presynapticNeuronAffinity,
		NULL
	);
}

Synapses Neuropil::CreateAxonsTo(
	Neuropil* postsynapticNeuropil,
	const Synapse* synapse,
	AxonGuide* guide,
	const Affinity* presynapticNeuronAffinity,
	const Affinity* postsynapticNeuronAffinity
)
{
	return Connect(
		this,
		postsynapticNeuropil,
		synapse,
		guide,
		presynapticNeuronAffinity,
		postsynapticNeuronAffinity
	);
}

Synapses Neuropil::Connect(
	Neuropil* presynapticNeuropil,
	Neuropil* postsynapticNeuropil,
	const Synapse* synapse,
	AxonGuide* guide,
	const Affinity* presynapticNeuronAffinity,
	const Affinity* postsynapticNeuronAffinity
) const
{
	BIO_SANITIZE(presynapticNeuropil && postsynapticNeuropil && synapse && guide, , return NULL)
	
	static Id presynapticNeuropilBindingSite = IdPerspective::Instance().GetIdFromName("PresynapticNeuropil");
	static Id postsynapticNeuropilBindingSite = IdPerspective::Instance().GetIdFromName("PostsynapticNeuropil");
	static Id synapseBindingSite = IdPerspective::Instance().GetIdFromName("Synapse");
	static Id presynapticNeuronAfiinityBindingSite = IdPerspective::Instance().GetIdFromName("PresynapticNeuronAffinity");
	static Id postsynapticNeuronAffinityBindingSite = IdPerspective::Instance().GetIdFromName("PostsynapticNeuronAffinity");
	static Id synapsesBindingSite = IdPerspective::Instance().GetIdFromName("Synapses");
	
	guide->RotateTo(presynapticNeuropilBindingSite)->Bind(*presynapticNeuropil);
	guide->RotateTo(postsynapticNeuropilBindingSite)->Bind(*postsynapticNeuropil);
	guide->RotateTo(synapseBindingSite)->Bind(*synapse);
	
	if (presynapticNeuronAffinity)
		guide->RotateTo(presynapticNeuronAfiinityBindingSite)->Bind(*presynapticNeuronAffinity);
	if (postsynapticNeuronAffinity)
		guide->RotateTo(postsynapticNeuronAffinityBindingSite)->Bind(*postsynapticNeuronAffinity);

	(*guide)();
	
	Synapses ret = guide->RotateTo(synapsesBindingSite)->Probe< Synapses >();

	guide->RotateTo(presynapticNeuropilBindingSite)->Release();
	guide->RotateTo(postsynapticNeuropilBindingSite)->Release();
	guide->RotateTo(synapseBindingSite)->Release();
	if (presynapticNeuronAffinity)
		guide->RotateTo(presynapticNeuronAfiinityBindingSite)->Release();
	if (postsynapticNeuronAffinity)
		guide->RotateTo(postsynapticNeuronAffinityBindingSite)->Release();
	guide->RotateTo(synapsesBindingSite)->Release();

	return ret;
}

} //neural namespace
} //bio namespace
