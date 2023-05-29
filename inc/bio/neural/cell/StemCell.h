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

#include "bio/neural/common/Class.h"
#include "bio/neural/common/Types.h"
#include "bio/neural/common/Codes.h"
#include "bio/neural/common/Filters.h"
#include "bio/neural/common/States.h"
#include "bio/neural/common/Features.h"
#include "bio/neural/common/Potentials.h"
#include "bio/neural/macro/Macros.h"
#include "bio/neural/MembranePotential.h"
#include "bio/cellular/Cell.h"

namespace bio {
namespace neural {

/**
 * neural::StemCells are undifferentiated cells that form the base class for all other neural::Cells. <br />
 * These cells are useful for filtering by through neural::Affinity. <br />
 */
class StemCell:
	public neural::Class< StemCell >,
	public cellular::Cell,
	public Covalent< chemical::UnorderedMotif< Feature > >,
	public Covalent< chemical::LinearMotif< MembranePotential* > >
{
public:

	/**
	 * Disambiguate all Class methods. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(neural, StemCell)

	BIO_DEFAULT_IDENTIFIABLE_CONSTRUCTORS(
		neural,
		StemCell,
		filter::Neural(),
	)

	/**
	 *
	 */
	virtual ~StemCell();

	/**
	 * Use this method to populate any member variable Protein*s. <br />
	 * You'll want to do this to speed up your code by bypassing the dynamic execution provided by genetic::Expressor. <br />
	 */
	virtual Code CacheProteins();

	/**
	 * If you use CacheProteins, you'll likely want to create your default Proteins here. <br />
	 * This will prevent dereferencing null or garbage pointers when using your cached Proteins. <br />
	 */
	virtual Code CreateDefaultProteins();

	/**
	 * Potentials are key values within a neural::Cell. <br />
	 * They are defined in Potentials.h. <br />
	 * Resetting potentials of *this, when there are no changes affecting it, should effectively remove any rounding errors that have accumulated. <br />
	 * You must implement this for each potential you create. <br />
	 * This will only be called if there is nothing affecting the potential and if the potential should be reset (see SetPotentialReset(), below). <br />
	 * @param potential
	 */
	virtual Code Reset(const Potential& potential)
	{
		if (potential == PotentialPerspective::InvalidId())
			return code::BadArgument1();
	} //just to avoid compiler warnings.


	/**
	 * Calls CheckForReset on all Potentials. <br />
	 */
	void ResetAllPossiblePotentials();

};

} //neural namespace
} //bio namespace
