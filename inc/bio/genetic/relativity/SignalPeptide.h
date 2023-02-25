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

#include "bio/common/container/Arrangement.h"
#include "bio/physical/string/TypedBrane.h"
#include "bio/genetic/common/Types.h"

namespace bio {
namespace genetic {

/**
 * SignalPeptides correlate Location names with their numeric ids. They also store any number of Affinities representing what action to be taken at the Location. <br />
 */
class SignalPeptide :
	public physical::Brane< Location >
{
public:

	/**
	 * @param id
	 * @param name
	 */
	SignalPeptide(
		const Location& id,
		const Name& name
	)
		:
		physical::TypedBrane< Location >(
			id,
			name,
			NULL
		)
	{

	}

	/**
	 *
	 */
	virtual ~SignalPeptide()
	{

	}

	/**
	 * A real-world peptidase is responsible for removing a signal peptide when it reaches the appropriate location. <br /> 
	 * Here, we treat Peptidases as the means by which a SignalPeptide effects the placement of a Gene. The type associated with each Affinity is a different Excitation which may be performed in order to move the associated Gene into the next (and eventually final) Location. <br />
	 */
	physical::TypedPerspective< Affinity > mPeptidases;
};

} //genetic namespace
} //bio namespace
