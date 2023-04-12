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

#include "bio/api/internal/Final.h"
#include "bio/api/internal/CellularForwarder.h"
#include "bio/api/macro/Macros.h"
#include "bio/cellular/Cell.h"

namespace bio {
namespace api {

/**
 * A FinalCell removes all the inheritance machinery from Cell. <br />
 * Use this when you want to create your own Cells but not allow anyone else to override Biology methods when inheriting from them. <br />
 * We have left Crest() as virtual here (i.e. redefined it) so that you can still use Cell as intended.
 */
class FinalCell :
	public Final< CellularForwarder< FinalCell, bio::cellular::Cell > >
{
public:

	BIO_CONSTRUCTORS_WITH_COMMON_CONSTRUCTOR(
		FinalCell,
		BIO_SINGLE_ARG(Final< CellularForwarder< FinalCell, bio::cellular::Cell > >)
	)

	virtual ~FinalCell() {}

	/**
	 * Crest()s occur at Periodic::mIntervals. <br />
	 * Define your main Periodic logic here. <br />
	 * This method must be fast: <br />
	 *	* do not read slow hardware here <br />
	 *	* do not block for a long time <br />
	 *	* do not sleep <br />
	 * If derived classes must do slow work to oscillate, that slow logic MUST BE placed in a separate thread. <br />
	 * This method would then get the data stored by that thread and returns the data *quickly*. <br />
	 * MAKE SURE that the thread never causes a long mutex wait as a side-effect in this Crest method. <br />
	 * <br />
	 * This method is forwarded here from the CellularForwarder. <br />
	 * All sub-cellular objects in *this will Crest after *this, automatically. <br />
	 */
	virtual Code Crest()
	{

		//     YOUR CODE GOES HERE!

		return code::NotImplemented();
	}

	BIO_FINAL_CELL_METHODS

private:
	void CommonConstructor()
	{
		this->mT.SetCrestFunction(&FinalCell::Crest, this);
	}
};

} //api namespace
} //bio namespace
