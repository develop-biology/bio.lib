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

namespace bio {
namespace chemical {

class Solute;

/**
 * Fluctuations change the Concentration of Solutes.
 */
class Fluctuation :
	public chemical::Class< Fluctuation >,
	public chemical::Substance
{
public:

	BIO_DISAMBIGUATE_ALL_CLASS_METHODS_FOR(chemical, Fluctuation)

	BIO_DEFAULT_IDENTIFIABLE_CONSTRUCTORS_WITH_COMMON_CONSTRUCTOR(
		chemical,
		Fluctuation,
		&FluctuationPerspective::Instance(),
		filter::Chemical
	)

	/**
	 * Affecting a Solute changes the Solute's Concentration.
	 * @param solute
	 * @return a Code describing the result of effecting change (e.g. code::Success()).
	 */
	virtual Code Affect(Solute* solute)
	{

		//YOUR CODE HERE.

		return code::NotImplemented();
	}
};

} //chemical namespace
} //bio namespace
