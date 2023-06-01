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

#include "bio/neural/Impulse.h"
#include "bio/neural/cell/Neuron.h"
#include "bio/neural/common/Filters.h"

namespace bio {
namespace neural {

Impulse::Impulse(
	FiringCondition trigger,
	Name impulseName,
	Neuron* caller,
	bool shouldBeActive
)
	:
	chemical::Class< Impulse >(this, impulseName, filter::Neural()),
	mTrigger(trigger),
	mImpulse(NULL),
	mCaller(caller),
	mShouldBeActive(shouldBeActive)
{
}

Impulse::~Impulse()
{
}

FiringCondition Impulse::GetTrigger() const
{
	return mTrigger;
}

molecular::Protein* Impulse::GetImpulseProtein()
{
	return mImpulse;
}

void Impulse::SetCaller(Neuron* caller)
{
	BIO_SANITIZE(caller, , return)
	mCaller = caller;
	mImpulse = caller->GetById< molecular::Protein* >(GetId());
}

bool Impulse::Send()
{
	BIO_SANITIZE(mImpulse && mCaller, , return false)
	if (mImpulse->IsEnabled())
	{
		(*mImpulse)();
	}
	return mShouldBeActive;
}

} //neural namespace
} //bio namespace
