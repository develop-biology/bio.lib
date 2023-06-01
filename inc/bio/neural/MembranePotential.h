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

#include "bio/neural/common/Types.h"
#include "bio/neural/common/Filters.h"
#include "bio/neural/macro/Macros.h"
#include "bio/molecular/Protein.h"
#include "bio/chemical/common/Class.h"

namespace bio {
namespace neural {

class StemCell;

/**
 * Potentials are simple classes which track how much of a given feature a StemCell is currently using. <br>
 * When a StemCell's use of a particular feature drops to 0, it can be safely reset. <br>
 * This system prevents rounding errors from accumulating over time. <br>
 * Each MembranePotential comes with a Protein which will be Activated in order to reset the Potential. <br>
 * See StemCell.h for more information. <br>
 */
class MembranePotential:
	public chemical::Class< MembranePotential >,
	public chemical::EnvironmentDependent< StemCell* >
{
public:
	/**
	 * Disambiguate all class methods. <br>
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(chemical, MembranePotential)

	BIO_DEFAULT_IDENTIFIABLE_CONSTRUCTORS(chemical,
		MembranePotential,
		filter::Neural())

	/**
	 * 
	 */
	virtual ~MembranePotential();

	/**
	 * @return the mPotential of *this.
	 */
	unsigned int GetPotential() const;

	/**
	 * Set the mPotential of *this.
	 * @param potential the new mPotential.
	 */
	void SetPotential(unsigned int potential);

	/**
	 * Increment the mPotential of *this.
	 */
	void IncrementPotential();

	/**
	 * Decrement the mPotential of *this.
	 */
	void DecrementPotential();

	/**
	 * @return whether or not *this should be reset.
	 */
	bool ShouldReset() const;

	/**
	 * Set whether or not *this should be reset.
	 * @param should whether or not *this should be reset.
	 */
	void SetReset(bool should);

	/**
	 * @return the Protein that implements *this Reset.
	 */
	molecular::Protein* GetResetProtein();

	/**
	 * Set the Protein that implements *this Reset.
	 * @param protein the Protein that implements *this Reset.
	 */
	void SetResetProtein(molecular::Protein* protein);

	/**
	 * Will RecruitChaperones for & Fold the Protein that implements *this Reset. <br />
	 * @param environment the StemCell* that *this is dependent on.
	 */
	void SetEnvironment(StemCell* environment);

	/**
	 * PROTEIN BASED. <br />
	 * @return Success() if *this was reset.
	 */
	virtual Code Reset();

protected:
	unsigned int mPotential;
	bool mShouldReset;
	molecular::Protein* mResetProtein;
};

} //neural namespace
} //bio namespace
