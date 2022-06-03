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
#include "bio/api/macro/Macros.h"
#include "bio/molecular/Vesicle.h"

namespace bio {
namespace api {

/**
 * A FinalVesicle removes all the inheritance machinery from Vesicle. <br />
 * Use this when you want to create your own Vesicles but not allow anyone else to override Biology methods when inheriting from them. <br />
 */
class FinalVesicle :
	public Final< bio::molecular::Vesicle >
{
public:

	BIO_CONSTRUCTORS(FinalVesicle, bio::Final< bio::molecular::Vesicle >)

	virtual ~FinalVesicle()	{}

	BIO_FINAL_VESICLE_METHODS
};

} //api namespace
} //bio namespace
