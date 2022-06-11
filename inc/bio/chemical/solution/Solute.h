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

#include "bio/chemical/common/Types.h"
#include "bio/chemical/Substance.h"
#include "bio/physical/Periodic.h"

namespace bio {
namespace chemical {

class Fluctuation;

/**
 * Solutes mimic the effect of small molecules, hormones, etc. on overall behavior.
 * These are, in effect, global variables which effect system wide changes.
 * As an example, let's look at hunger. A "Hunger" Solute (e.g. "Ghrelin") may increase over time and be decreased by a "Food" or "Digestion" stimulus.
 * The term "Concentration" denotes the strength of the chemical signal, i.e. the value of the global variable.
 * We also have a min and max to describe this Organism's chemical limits and abstract away complex metabolic regulation.
 * "Fluctuations" allow a Solute's Concentration to be changed over time or in response to a stimulus.
 *
 * Solutes are Substances but are not Molecules, they only affect systems through their concentration, not their particular shape.
 *
 * IMPORTANT NOTE: THIS CLASS DOES NOT ACCOUNT FOR ROUNDING ERRORS!
 * If you start at 0.0f and then increment and decrement this class by the same value, do not expect the result to be 0.0f!
 */
class Solute :
	public chemical::Class< Solute >,
	public chemical::LinearMotif< Fluctuation* >,
	public chemical::Substance,
	public physical::Periodic
{
public:

	BIO_DISAMBIGUATE_ALL_CLASS_METHODS_FOR(chemical, Solute)

	BIO_DEFAULT_IDENTIFIABLE_CONSTRUCTORS_WITH_COMMON_CONSTRUCTOR(
		chemical,
		Solute,
		&SolutePerspective::Instance(),
		filter::Chemical
	)

	/**
	 * Get the Concentration of *this (i.e. its "value").
	 * @return the mConcentration of *this.
	 */
    virtual Concentration GetConcentration() const;

	/**
	 * Rather than activating a Fluctuation, you can just set the mConcentration of *this directly.
	 * @param newConcentration
	 */
	virtual void SetConcentration(Concentration newConcentration);

	/**
	 * Get the mMin of *this
	 * @return the mMin of *this.
	 */
	virtual Concentration GetMin() const;

	/**
	 * Get the mMax of *this.
	 * @return the mMax of *this.
	 */
	virtual Concentration GetMax() const;

	/**
	 * Set the mMin of *this.
	 * @param newMin
	 */
	virtual void SetMin(Concentration newMin);

	/**
	 * Set the mMax of *this.
	 * @param newMax
	 */
	virtual void SetMax(Concentration newMax);

	/**
	 * @return the Concentration of *this last time *this Peak()ed.
	 */
	virtual Concentration GetConcentrationAtLastPeak() const;

   /**
     * Applies active changes. See EnableFluctuation, above.
     * For more info see physical::Periodic.
     */
	virtual Code Peak();
	
protected:
    Concentration mConcentration;
    Concentration mConcentrationAtLastPeak;
    Concentration mMin, mMax;

	/**
	 * Make sure the Concentration of *this does not exceed mMin ~ mMax.
	 */
	virtual void Limit();

private:

	/**
	 *
	 */
	void CommonConstructor();
};

} //chemical namespace
} //bio namespace
