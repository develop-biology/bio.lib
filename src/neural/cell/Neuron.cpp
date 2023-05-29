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

#include "bio/neural/cell/Neuron.h"
#include "bio/neural/cell/Synapse.h"

namespace bio {
namespace neural {

void Neuron::CommonConstructor()
{
	CreateImpulse(
		firing_condition::RisingEdge(),
		"RisingEdge"
	);
	CreateImpulse(
		firing_condition::Continuing(),
		"ContinuingImpulse"
	);
	CreateImpulse(
		firing_condition::FallingEdge(),
		"FallingEdge",
		false
	);
}

Neuron::~Neuron()
{
	BIO_LOG_DEBUG(
		"Destroying %s",
		GetName().AsCharString());
}


Code Neuron::CreateDefaultProteins()
{
	Add< molecular::Protein* >(new molecular::Protein("PreCrest"));
	Add< molecular::Protein* >(new molecular::Protein("PreSend"));
	Add< molecular::Protein* >(new molecular::Protein("PostSend"));
	Add< molecular::Protein* >(new molecular::Protein("Learn"));

	return StemCell::CreateDefaultProteins();
}

Code Neuron::CacheProteins()
{
	mcPreCrest = GetByName< molecular::Protein* >("PreCrest");
	mcPreSend = GetByName< molecular::Protein* >("PreSend");
	mcPostSend = GetByName< molecular::Protein* >("PostSend");
	mcLearn = GetByName< molecular::Protein* >("Learn");

	UpdateImpulseCallers();

	return StemCell::CacheProteins();
}

Code Neuron::Apoptose()
{
	return StemCell::Apoptose();
}

Code Neuron::Crest()
{
	return StemCell::Crest();
}

void Neuron::PreCrest()
{
	(*mcPreCrest)();
}

bool Neuron::CheckIn()
{
	// BIO_LOG_DEBUG("Cresting %s", GetName());
	Timestamp now = physical::GetCurrentTimestamp();
	if (GetInterval() + mLastCrestTimestamp > now)
	{
		return false;
	}
	mLastCrestTimestamp = now;

	PreCrest();
	ProcessDendrites();
	return StemCell::CheckIn();
}

void Neuron::UpdateImpulseCallers()
{
	for (
		SmartIterator imp = GetAll< Impulse* >()->End();
		!imp.IsBeforeBeginning();
		--imp
		)
	{
		imp.As< Impulse* >()->SetCaller(this);
	}
}

void Neuron::CreateImpulse(
	const FiringCondition& trigger,
	const Name& impulseName,
	bool shouldBeActive
)
{
	if (GetById< Impulse* >(IdPerspective::Instance().GetIdFromName(impulseName)))
	{
		return;
	}
	Impulse* toAdd = new Impulse(
		trigger,
		impulseName,
		this,
		shouldBeActive
	);
	Add< Impulse* >(toAdd);
}

bool Neuron::DetermineAndCallImpulseTriggers()
{
	bool ret = false;

	if (!DetermineImpulseTriggers(ExciteTrigger()))
	{
		return ret;
	}
	
	Impulse* impulse;

	for (
		SmartIterator imp = GetAll< Impulse* >()->Begin();
		!imp.IsAfterEnd();
		++imp
	) {
		impulse = imp.As< Impulse* >();
		if (GetReasonForFiring() == impulse->GetTrigger())
		{
			//TODO: is this the correct return value?
			ret = impulse->Send();
		}
	}

	return ret;
}

FiringCondition Neuron::GetReasonForFiring() const
{
	return mFiringReason;
}

void Neuron::StartAtRisingEdge()
{
	mContinuingImpulse = false;
}

bool Neuron::IsPersisting(const Timestamp now) const
{
	return now < mLastActive + mPersistFor;
}

void Neuron::PersistFor(const Milliseconds ms)
{
	mPersistFor = ms;
}

void Neuron::PersistUntil(const Timestamp time)
{
	mPersistFor = time - physical::GetCurrentTimestamp();
}

void Neuron::ActiveNow()
{
	Timestamp now = physical::GetCurrentTimestamp();
	if (now > mLastActive)
	{
		mLastActive = now;
	}
}

void Neuron::ActiveUntil(Timestamp time)
{
	mLastActive = time;
}

Timestamp Neuron::GetTimeLastActive() const
{
	if (IsActive())
	{
		return physical::GetCurrentTimestamp();
	}
	return mLastActive;
}

bool Neuron::IsActive() const
{
	return Has< State >(state::Active());
}

void Neuron::Learn()
{
	(*mcLearn)();
}

Synapse* Neuron::ConnectTo(
	Neuron* target,
	const Synapse* synapseType
)
{
	BIO_SANITIZE(target && synapseType, , return NULL)
	
	Synapse* synapse;
	
	for (
		SmartIterator syn = GetAll< Axon* >()->End();
		!syn.IsBeforeBeginning();
		--syn
		)
	{
		synapse = ChemicalCast< Synapse* >(syn.As< Axon* >());
		if (synapse->GetPostsynapticNeuron() == target && synapse->IsId(synapseType->GetId()))
		{
			BIO_LOG_ERROR(
				"%s failed to create a synapse due to: already exists",
				GetName().AsCharString())
			return *syn;
		}
	}
	Synapse* out = synapseType->ConfigureFor(
		this,
		target
	);
	BIO_SANITIZE(out, , return NULL)

	Add< Axon* >(out);

	BIO_LOG_DEBUG(
		"%s created new synapse (%s) to %s",
		GetName().AsCharString(),
		out->GetName().AsCharString(),
		out->GetPostsynapticName().AsCharString());
	return out;
}

void Neuron::RequestProcessingOf(Dendrite* source)
{
	if (source->ShouldBeDepotentiated())
	{
		BIO_LOG_DEBUG(
			"%s: removing %s",
			GetName().AsCharString(),
			source->GetName().AsCharString());
		DepotentiateDendrite(source);
	}
	if (source->ShouldBePotentiated())
	{
		BIO_LOG_DEBUG(
			"%s: adding %s",
			GetName().AsCharString(),
			source->GetName().AsCharString());
		PotentiateDendrite(source);
	}
}

void Neuron::PotentiateDendrite(Dendrite* dendrite)
{
	if (dendrite->Has< State >(state::Potentiated()) && !dendrite->Has< Feature >(feature::MultiplePotentiations()))
	{
		BIO_LOG_WARN(
			"%s: cannot add already added synapse %s",
			GetName().AsCharString(),
			dendrite->GetName().AsCharString());
		return;
	}

	const Code process = dendrite->ProcessPotentiation();
	if (process != code::Success() && process != code::NoErrorNoSuccess())
	{
		BIO_LOG_WARN(
			"%s: did not successfully add %s",
			GetName().AsCharString(),
			dendrite->GetName().AsCharString());
		return;
	}

	if (dendrite->Has< State >(state::Potentiated()))
	{
		//TODO: Is this too much overhead to account for rounding errors?

		MembranePotential* potential;

		for (
			SmartIterator pot = dendrite->GetAll< MembranePotential* >()->End();
			!pot.IsBeforeBeginning();
			--pot
		) {
			potential = GetById< MembranePotential* >(pot.As< MembranePotential* >()->GetId());
			BIO_SANITIZE(potential, , continue)
			potential->IncrementPotential();
		}
	}

	//TODO: do we care about the return code?
	dendrite->PostPotentiation();

	//BIO_LOG_DEBUG("%s: Unprocessed data from %s processed" ame(), toProcess->GetDatum()->GetOwnerInfo().GetName().AsCharString().c_str());
}

void Neuron::DepotentiateDendrite(Dendrite* dendrite)
{
	if (!dendrite->Has< State >(state::Potentiated()) && !dendrite->Has< Feature >(feature::MultipleDepotentiations()))
	{
		BIO_LOG_WARN(
			"%s: cannot remove unadded synapse %s",
			GetName().AsCharString(),
			dendrite->GetName().AsCharString());
		return;
	}

	const Code process = dendrite->ProcessDepotentiation();
	if (process != code::Success() && process != code::NoErrorNoSuccess())
	{
		BIO_LOG_WARN(
			"%s: did not successfully remove %s",
			GetName().AsCharString(),
			dendrite->GetName().AsCharString());
		return;
	}

	if (!dendrite->Has< State >(state::Potentiated()))
	{
		//TODO: Is this too much overhead to account for rounding errors?

		MembranePotential* potential;

		for (
			SmartIterator pot = dendrite->GetAll< MembranePotential* >()->End();
			!pot.IsBeforeBeginning();
			--pot
		) {
			potential = GetById< MembranePotential* >(pot.As< MembranePotential* >()->GetId());
			BIO_SANITIZE(potential, , continue)
			potential->DecrementPotential();
		}
		ResetAllPossiblePotentials();
	}

	//TODO: do we care about the return code?
	dendrite->PostDepotentiation();

	//BIO_LOG_DEBUG("%s: Processed data from %s processed" ame(), toProcess->GetDatum()->GetOwnerInfo().GetName().AsCharString().c_str());
}

void Neuron::PreSend()
{
	(*mcPreSend)();
}

Code Neuron::Transmit(Affinity* selection)
{
	PreSend();

	Code ret = code::Success();

	Axons axons;
	if (selection)
	{
		axons = GetAllLike< Axon* >(selection);
	}
	else
	{
		axons = GetAll< Axon* >();
	}

	for (
		SmartIterator axn = axons.Begin();
		!axn.IsAfterEnd();
		++axn
	) {
		Code returned = axn.As< Axon* >()->ProcessOutgoing();
		if (returned != code::Success() && returned != code::NoErrorNoSuccess())
		{
			BIO_LOG_WARN(
				"%s: did not successfully send %s",
				GetName().AsCharString(),
				axn.As< Axon* >()->GetName().AsCharString());

			ret = code::UnknownError(); //only unknown because we're lazy & don't want to elaborate.
		}
	}

	PostSend();

	return ret;
}

void Neuron::PostSend()
{
	(*mcPostSend)();
}

Code Neuron::DepotentiateSentData(Affinity* selection)
{
	Axons axons;
	if (selection)
	{
		axons = GetAllLike< Axon* >(selection);
	}
	else
	{
		axons = GetAll< Axon* >();
	}

	for (
		SmartIterator axn = axons.Begin();
		!axn.IsAfterEnd();
		++axn
	) {
		axn.As< Axon* >()->DepotentiateSignal();
	}
}

bool Neuron::DetermineImpulseTriggers(bool trigger)
{
	bool persistance = IsPersisting(physical::GetCurrentTimestamp());
	if (trigger || persistance)
	{
		if (!trigger)
		{
			BIO_LOG_DEBUG(
				"%s is active, only due to persistence",
				GetName().AsCharString());
		}
		if (mContinuingImpulse)
		{
			mFiringReason = firing_condition::Continuing();
		}
		else
		{
			mFiringReason = firing_condition::RisingEdge();
			mContinuingImpulse = true;
		}
		return true;
	}
	else if (mContinuingImpulse)
	{
		mFiringReason = firing_condition::FallingEdge();
		mContinuingImpulse = false;
		return true;
	}
	return false;
}

Code Neuron::ProcessDendrite(Dendrite* dendrite)
{
	BIO_SANITIZE(dendrite, , return code::BadArgument1())
	if (dendrite->ShouldBeDepotentiated())
	{
		BIO_LOG_DEBUG(
			"%s: removing %s",
			GetName().AsCharString(),
			dendrite->GetName().AsCharString()
		)
		DepotentiateDendrite(dendrite);
	}
	if (dendrite->ShouldBePotentiated())
	{
		BIO_LOG_DEBUG(
			"%s: adding %s",
			GetName().AsCharString(),
			dendrite->GetName().AsCharString()
		)
		PotentiateDendrite(dendrite);
	}
}

void Neuron::ProcessDendrites(Affinity* selection)
{
	Dendrites dendrites;
	if (selection)
	{
		dendrites = GetAllLike< Dendrite* >(selection);
	}
	else
	{
		dendrites = GetAll< Dendrite* >();
	}
	BIO_SANITIZE(dendrites.Size(), , return)

	unsigned int selectionsFound;
	for (
		SmartIterator den = dendrites.Begin();
		!den.IsAfterEnd();
		++den
		)
	{
		ProcessDendrite(den.As< Dendrite* >());
	}
}

} //namespace neural
} //namespace bio
