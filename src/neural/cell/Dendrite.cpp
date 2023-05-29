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

#include "bio/neural/cell/Dendrite.h"
#include "bio/neural/cell/Neuron.h"

namespace bio {
namespace neural {

Dendrite::~Dendrite()
{

}

Code Dendrite::CacheProteins()
{
	mcPrepareForPotentiation = GetByName< molecular::Protein* >("PrepareForPotentiation");
	mcProcessPotentiation = GetByName< molecular::Protein* >("ProcessPotentiation");
	mcPostPotentiation = GetByName< molecular::Protein* >("PostPotentiation");

	mcPrepareForDepotentiation = GetByName< molecular::Protein* >("PrepareForDepotentiation");
	mcProcessDepotentiation = GetByName< molecular::Protein* >("ProcessDepotentiation");
	mcPostDepotentiation = GetByName< molecular::Protein* >("PostDepotentiation");

	mcReReady = GetByName< molecular::Protein* >("ReReady");

	return Neurite::CacheProteins();
}

Code Dendrite::CreateDefaultProteins()
{
	Add< molecular::Protein* >(new molecular::Protein("PrepareForPotentiation"));
	Add< molecular::Protein* >(new molecular::Protein("ProcessPotentiation"));
	Add< molecular::Protein* >(new molecular::Protein("PostPotentiation"));

	Add< molecular::Protein* >(new molecular::Protein("PrepareForDepotentiation"));
	Add< molecular::Protein* >(new molecular::Protein("ProcessDepotentiation"));
	Add< molecular::Protein* >(new molecular::Protein("PostDepotentiation"));

	Add< molecular::Protein* >(new molecular::Protein("ReReady"));

	return Neurite::CreateDefaultProteins();
}

bool Dendrite::ShouldBePotentiated() const
{
	// BIO_LOG_DEBUG("%s...", GetName().AsCharString());
	if (!Has< State >(state::Ready()))
	{
		// BIO_LOG_DEBUG(" not ready");
		return false;
	}

	if (Has< State >(state::Potentiated()))
	{
		// BIO_LOG_DEBUG(" not added as it is already added");
		return false;
	}

	if (IsZero() && !Has< Feature >(feature::PossibleZeroValue()))
	{
		// BIO_LOG_DEBUG(" not added as its value is zero");
		return false;
	}

	Timestamp now = physical::GetCurrentTimestamp();

	//FIXME: bug when GetCurrentTimestamp = UINT_MAX
	if (now < mPotentiateAt)
	{
		// BIO_LOG_DEBUG(" will not be added until %lums from now", (mPotentiateAt - now));
		return false;
	}
	return true;
}

bool Dendrite::ShouldBeDepotentiated() const
{
	if (!Has< State >(state::Potentiated()))
	{
		return false;
	}

	//New data are available.
	if (Has< State >(state::Ready()))
	{
		return true;
	}

	return false;
}

void Dendrite::Potentiated()
{
	if (Has< State >(state::Potentiated()) && !Has< Feature >(feature::MultiplePotentiations()))
	{
		BIO_LOG_WARN(
			"%s readded",
			GetName().AsCharString());
	}
	else
	{
		BIO_LOG_DEBUG("%s added",
			GetName().AsCharString())
		PrepareForDepotentiation();
	}
	Add< State >(state::Potentiated());
	mLastPotentiated = physical::GetCurrentTimestamp();
	NoLongerReady();
}

void Dendrite::Depotentiated()
{
	if (!Has< State >(state::Potentiated()) && !Has< Feature >(feature::MultipleDepotentiations()))
	{
		BIO_LOG_WARN(
			"%s removed while not added",
			GetName().AsCharString());
	}
	else
	{
		BIO_LOG_DEBUG("%s removed",
			GetName().AsCharString());
	}
	Remove< State >(state::Potentiated());
}

Timestamp Dendrite::PrepareForPotentiation(Timestamp whenToPotentiate)
{
	static Id bindingSite = IdPerspective::Instance().GetIdFromName("WhenToPotentiate");
	mcPrepareForPotentiation->RotateTo(bindingSite)->Bind(whenToPotentiate);
	(*mcPrepareForPotentiation)();
	Timestamp ret = mcPrepareForPotentiation->RotateTo(bindingSite)->Probe< Timestamp >();
	mcPrepareForPotentiation->RotateTo(bindingSite)->Release();
	return ret;
}

void Dendrite::PrepareForDepotentiation()
{
	(*mcPrepareForDepotentiation)();
}

void Dendrite::ReReady()
{
	(*mcReReady)();
}

Timestamp Dendrite::GetTimeToAdd() const
{
	return mPotentiateAt;
}

Code Dendrite::ProcessPotentiation()
{
	return (*mcProcessPotentiation)();
}

Code Dendrite::ProcessDepotentiation()
{
	return (*mcProcessDepotentiation)();
}

Code Dendrite::PostPotentiation()
{
	return (*mcPostPotentiation)();
}

Code Dendrite::PostDepotentiation()
{
	return (*mcPostDepotentiation)();
}

void Dendrite::NoLongerReady()
{
	Remove< State >(state::Ready());
	mPotentiateAt = 0; //FIXME: this could be incrementing the timestamp.
}


const Neuron* Dendrite::GetPostsynapticNeuron() const
{
	return mPostsynapticNeuron;
}

Name Dendrite::GetPostsynapticName() const
{
	BIO_SANITIZE(GetPostsynapticNeuron(), ,return "NO PRESYNAPTIC NEURON")
	return GetPostsynapticNeuron()->GetName();
}

Id Dendrite::GetPostsynapticId() const
{
	BIO_SANITIZE(GetPostsynapticNeuron(), ,return IdPerspective::InvalidId())
	return GetPostsynapticNeuron()->GetId();
}

} //neural namespace
} //bio namespace
