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

#include "bio/chemical/solution/Fluctuation.h"

namespace bio {
namespace chemical {

/**
 * ChangeOverTime changes the Concentration of a Solute by a given amount per millisecond.
 */
class ChangeOverTime :
	public chemical::Class< ChangeOverTime >,
	public chemical::Fluctuation
{
public:

	BIO_DISAMBIGUATE_ALL_CLASS_METHODS_FOR(chemical, ChangeOverTime)

	BIO_DEFAULT_IDENTIFIABLE_CONSTRUCTORS_WITH_COMMON_CONSTRUCTOR(
		chemical,
		ChangeOverTime,
		&FluctuationPerspective::Instance(),
		filter::Chemical
	)

	/**
	 * Changes the Concentration of a Solute by a given amount per millisecond.
	 * @param solute
	 * @return a Code describing the result of effecting change (e.g. code::Success()).
	 */
	virtual Code Affect(Solute* solute);

	/**
	 * Get the amount *this will change a given Solute by per millisecond.
	 * @return the mRate of *this.
	 */
	virtual Concentration GetRate() const;

	/**
	 * Set the amount *this will change a given Solute by per millisecond.
	 * @param rate
	 */
	virtual void SetRate(Concentration rate);

protected:
	Concentration mRate;
};

} //chemical namespace
} //bio namespace
