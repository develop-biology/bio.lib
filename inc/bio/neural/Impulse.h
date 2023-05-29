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
#include "bio/neural/common/FiringConditions.h"
#include "bio/neural/macro/Macros.h"
#include "bio/molecular/Protein.h"
#include "bio/chemical/common/Class.h"

namespace bio {
namespace neural {

/**
 * Firing Condition Impulses are simple function pointer wrappers that allow different functions to be called upon different triggers. <br />
 * firing_condition::Impulses can be enabled/disabled for ease of use. <br />
 * firing_condition::Impulses should specify the RETURN value for their caller, DetermineAndCallImpulseTriggers(), via the bool shouldBeActive. <br />
 * Impulses use Name instead of Id as they need to be created before Proteins can be added to them. <br />
 */
class Impulse:
	public chemical::Class< Impulse >
{
public:

	/**
	 * Disambiguate all class methods. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(chemical, Impulse)

	/**
	 * @param trigger
	 * @param impulseName
	 * @param caller
	 * @param shouldBeActive
	 */
	Impulse(
		FiringCondition trigger = firing_condition::None(),
		Name impulseName = "",
		Neuron* caller = NULL,
		bool shouldBeActive = true
	);

	/**
	 *
	 */
	virtual ~Impulse();

	/**
	 * @return the FiringCondition that triggers *this Impulse
	 */
	FiringCondition GetTrigger() const;

	/**
	 * @return the Protein that implements *this Impulse
	 */
	molecular::Protein* GetImpulseProtein();

	/**
	 * @param caller the Neuron that triggered *this Impulse.
	 */
	void SetCaller(Neuron* caller);

	/**
	 * @return whether or not the caller should be active.
	 */
	bool Send();

protected:
	FiringCondition mTrigger;
	molecular::Protein* mImpulse;
	Neuron* mCaller;
	bool mShouldBeActive;
};

typedef Arrangement< Impulse > Impulses;

} //neural namespace
} //bio namespace
