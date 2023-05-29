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

#include "bio/neural/cell/Axon.h"
#include "bio/neural/cell/Neuron.h"

namespace bio {
namespace neural {

Axon::~Axon()
{

}

Code Axon::CacheProteins()
{
	mcProcessOutgoing = GetByName< molecular::Protein* >("ProcessOutgoing");
	mCallback = GetByName< molecular::Protein* >("Callback");
	mcDepotentiateSignal = GetByName< molecular::Protein* >("DepotentiateSignal");
	
	return Neurite::CacheProteins();
}

Code Axon::CreateDefaultProteins()
{
	Add< molecular::Protein* >(new molecular::Protein("ProcessOutgoing"));
	Add< molecular::Protein* >(new molecular::Protein("Callback"));
	Add< molecular::Protein* >(new molecular::Protein("DepotentiateSignal"));
	
	return Neurite::CreateDefaultProteins();
}

Code Axon::ProcessOutgoing()
{
	return (*mcProcessOutgoing)();
}

void Axon::Callback(const molecular::Molecule* arg)
{
	static Id bindingSite = IdPerspective::Instance().GetIdFromName("PostsynapticCallback");
	if (arg)
	{
		mCallback->RotateTo(bindingSite)->Bind(*arg);
	}
	(*mCallback)();
	mCallback->RotateTo(bindingSite)->Release();
}

Milliseconds Axon::GetTimeout() const
{
	return mTimeout;
}

void Axon::SetTimeout(Milliseconds lastFor)
{
	mTimeout = lastFor;
}

void Axon::ExtendTimeoutUntil(Timestamp timeToDepotentiate)
{
	SetTimeout(timeToDepotentiate - mPotentiateAt);
}

Timestamp Axon::GetTimeLastUpdated() const
{
	return mLastUpdated;
}

void Axon::Update(Timestamp addDatumAt)
{
	mLastUpdated = addDatumAt;
}

void Axon::DepotentiateSignal()
{
	(*mcDepotentiateSignal)();
}

const Neuron* Axon::GetPresynapticNeuron() const
{
	return mPresynapticNeuron;
}

Name Axon::GetPresynapticName() const
{
	return GetPresynapticNeuron()->GetName();
}

Id Axon::GetPresynapticId() const
{
	return GetPresynapticNeuron()->GetId();
}

} //neural namespace
} //bio namespace
