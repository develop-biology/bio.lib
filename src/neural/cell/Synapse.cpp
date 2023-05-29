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

#include "bio/neural/cell/Synapse.h"
#include "bio/neural/cell/Neuron.h"

namespace bio {
namespace neural {

Synapse::~Synapse()
{
}

Code Synapse::CacheProteins()
{
	mcAdditionalConfiguration = GetByName< molecular::Protein* >("AdditionalConfiguration");
	
	Axon::CacheProteins();
	Dendrite::CacheProteins();
	
	return code::Success();
}

Code Synapse::CreateDefaultProteins()
{
	Add< molecular::Protein* >(new molecular::Protein("AdditionalConfiguration"));
	
	Axon::CreateDefaultProteins();
	Dendrite::CreateDefaultProteins();
	
	return code::Success();
}

Synapse* Synapse::ConfigureFor(
	const Neuron* presynapticNeuron,
	Neuron* postsynapticNeuron
) const
{
	BIO_SANITIZE(presynapticNeuron && postsynapticNeuron, , return)
	Synapse* ret = ChemicalCast< Synapse* >(Clone());
	ret->mPresynapticNeuron = presynapticNeuron;
	ret->mPostsynapticNeuron = postsynapticNeuron;
	ret->mPostsynapticNeuronId = mPostsynapticNeuron->GetId();
	ret->AdditionalConfiguration(
		presynapticNeuron,
		postsynapticNeuron
	);
	postsynapticNeuron->Add< Dendrite* >(ret);
	return ret;
}

void Synapse::AdditionalConfiguration(
	const Neuron* presynapticNeuron,
	Neuron* postsynapticNeuron
)
{
	BIO_SANITIZE(presynapticNeuron && postsynapticNeuron, , return)
	static Id presynapticBindingSite = IdPerspective::Instance().GetIdFromName("PresynapticNeuron");
	static Id postsynapticBindingSite = IdPerspective::Instance().GetIdFromName("PostsynapticNeuron");
	mcAdditionalConfiguration->RotateTo(presynapticBindingSite)->Bind(*presynapticNeuron);
	mcAdditionalConfiguration->RotateTo(postsynapticBindingSite)->Bind(*postsynapticNeuron);
	(*mcAdditionalConfiguration)();
	mcAdditionalConfiguration->RotateTo(presynapticBindingSite)->Release();
	mcAdditionalConfiguration->RotateTo(postsynapticBindingSite)->Release();
}

void Synapse::Update(Timestamp whenToPotentiate)
{
	Axon::Update(whenToPotentiate);
	Add< State >(state::Ready());
	mPotentiateAt = whenToPotentiate;

	//NOTE: Depotentiation time may be extended later or other data edits may occur
	//This is better than GetTimeToAdd == GetCurrentTimestamp.
	//FIXME: bug when GetCurrentTimestamp = UINT_MAX
	if (GetTimeToAdd() <= GetTimeLastUpdated())
	{
		GetPostsynapticNeuron()->RequestProcessingOf(this);
	}
}

bool Synapse::ShouldBePotentiated() const
{
	if (!Dendrite::ShouldBePotentiated())
	{
		return false;
	}

	Timestamp now = physical::GetCurrentTimestamp();

	if (Has< Feature >(feature::Timeout()) && now >= mPotentiateAt + mTimeout)
	{
		// BIO_LOG_DEBUG(" not added as it expired %lums ago", (now - (mLastUpdated + mTimeout)));
		return false;
	}
	BIO_LOG_DEBUG("    will be added!");
	return true;
}

bool Synapse::ShouldBeDepotentiated() const
{
	if (!Has< State >(state::Potentiated()))
	{
		return false;
	}
	if (Dendrite::ShouldBeDepotentiated())
	{
		return true;
	}

	if (Has< Feature >(feature::Timeout()))
	{
		Timestamp now = physical::GetCurrentTimestamp();
		return now >= mLastPotentiated + mTimeout;
	}
	return false;
}

} //namespace neural
} //namespace bio
