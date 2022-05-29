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
#include "bio/api/macros/Macros.h"
#include "bio/cellular/Organelle.h"

namespace bio {
namespace api {

/**
 * A FinalOrganelle removes all the inheritance machinery from Organelle. <br />
 * Use this when you want to create your own Organelles but not allow anyone else to override Biology methods when inheriting from them. <br />
 * We have left Peak() as virtual here (i.e. redefined it) so that you can still use Organelles as intended.
 */
class FinalOrganelle :
	public Final< CellularForwarder< FinalOrganelle, bio::cellular::Organelle, bio::cellular::OrganellePerspective > >
{
public:

	BIO_CONSTRUCONSTRUCTORS_WITH_COMMON_CONSTRUCTOR(FinalOrganelle, BIO_SINGLE_ARG(Final< CellularForwarder< FinalOrganelle, bio::cellular::Organelle, bio::cellular::OrganellePerspective > >))

	virtual ~FinalOrganelle() {}

	/**
	 * Peak()s occur at Periodic::mIntervals. <br />
	 * Define your main Periodic logic here. <br />
	 * This method must be fast: <br />
	 *	* do not read slow hardware here <br />
	 *	* do not block for a long time <br />
	 *	* do not sleep <br />
	 * If derived classes must do slow work to oscillate, that slow logic MUST BE placed in a separate thread. <br />
	 * This method would then get the data stored by that thread and returns the data *quickly*. <br />
	 * MAKE SURE that the thread never causes a long mutex wait as a side-effect in this Peak method. <br />
	 * <br />
	 * This method is forwarded here from the CellularForwarder. <br />
	 * All sub-cellular objects in *this will Peak after *this, automatically. <br />
	 */
	virtual Code Peak()
	{

		//     YOUR CODE GOES HERE!

		return code::NotImplemented();
	}

	BIO_FINAL_ORGANELLE_METHODS

private:
	void CommonConstructor()
	{
		this->mT.SetPeakFunction(&FinalOrganelle::Peak, this);
	};
};

} //api namespace
} //bio namespace
