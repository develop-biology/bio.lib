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

#include "bio/chemical/structure/motif/DependentMotif.h"
#include "bio/cellular/Tissue.h"
#include "bio/circulatory/common/Class.h"
#include "bio/circulatory/common/Types.h"
#include "bio/circulatory/macro/Macros.h"
#include "bio/circulatory/common/Filters.h"
#include "bio/circulatory/solution/Blood.h"
#include "bio/circulatory/cell/BloodVessel.h"

namespace bio{
namespace circulatory {

/**
 * Vasculature is a Tissue which contains all linked BloodVessels. <br />
 * Adding 2 BloodVessels to the same Vasculature will cause changes made to one to appear in the other. <br />
 * You may have multiple Vasculatures in the same way you may have multiple Hearts. Doing so will create distinct regions of synchronicity in your Organism. For example, if your Organism models a population of bacteria, each bacterium could have its own Vasculature and interact with its environment in a manner disparate from its neighbors. <br />
 */
class Vasculature :
	public circulatory::Class< Vasculature >,
	public chemical::DependentMotif< BloodVessel*, Vasculature*>,
	public cellular::Tissue
{
public:

	/**
	 * Ensure virtual methods point to Class implementations. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(circulatory, Vasculature)

	/**
	 * Standard constructors. <br />
	 */
	BIO_DEFAULT_IDENTIFIABLE_CONSTRUCTORS(
		circulatory,
		Vasculature,
		filter::Circulatory()
	)

	/**
	 *
	 */
	virtual ~Vasculature();

	/**
	 * @return
	 */
	virtual Code Crest();

protected:
	Blood mBlood;
};

} //circulatory namespace
} //bio namespace
