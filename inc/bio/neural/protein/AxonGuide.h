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

#include "bio/neural/common/Types.h"
#include "bio/neural/common/Class.h"
#include "bio/neural/common/Filters.h"
#include "bio/neural/macro/Macros.h"
#include "bio/molecular/Protein.h"

namespace bio {
namespace neural {

/**
 * AxonGuide Proteins assist Neuropils in creating Synapses. <br />
 * NOTE: This is a HUGE over simplification of the actual process. AxonGuide Proteins here are perhaps most representative of the receptors for the various axon guidance molecules & their concentration gradients. Its as if we assume the molecules to guide axon growth are always present & the Proteins we create are very selectively sensitive. <br />
 * You can create AxonGuide Proteins to connect Neurons in novel ways. For examples, check out the guide folder. <br />
 */
class AxonGuide:
	public neural::Class< AxonGuide >,
	public molecular::Protein
{
public:

	/**
	 * Ensure virtual methods point to Class implementations. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(neural, AxonGuide)

	BIO_DEFAULT_IDENTIFIABLE_CONSTRUCTORS(
		neural,
		AxonGuide,
		filter::Neural()
	)

	virtual ~AxonGuide();

	/**
	 * This should be overwritten to add whatever functionality is desired. <br />
	 * It is likely desirable, though not enforced to call Protein::Activate() at the end of any overwritten functions. This will cause all sub-Proteins to be Activated. <br />
	 * @return result of Activation.
	 */
	virtual Code Activate();

};

} //neural namespace
} //bio namespace
