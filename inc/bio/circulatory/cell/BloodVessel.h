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

#include "bio/cellular/Cell.h"
#include "bio/circulatory/common/Class.h"
#include "bio/circulatory/common/Types.h"
#include "bio/circulatory/macro/Macros.h"
#include "bio/circulatory/common/Filters.h"

namespace bio {
namespace circulatory {

class Vasculature;

/**
 * BloodVessels form the interface between a Tissue (or any other collection of Vesicles) and the Circulatory system. <br />
 * Solutes may be moved into and out of a BloodVessel, which allows them to be homogenized with all other BloodVessels which share a Vasculature. <br />
 * BloodVessels use Blood as their Solution and are thread safe. <br />
 */
class BloodVessel :
	public circulatory::Class< BloodVessel >,
	public cellular::Cell,
	public chemical::EnvironmentDependent< Vasculature* >
{
public:

	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(circulatory, BloodVessel)

	/**
	 * Standard constructors. <br />
	 */
	BIO_DEFAULT_IDENTIFIABLE_CONSTRUCTORS(
		circulatory,
		BloodVessel,
		filter::Circulatory()
	)

	/**
	 *
	 */
	virtual ~BloodVessel();

	/**
	 * @return
	 */
	virtual Code Crest();
};

} //circulatory namespace
} //bio namespace
