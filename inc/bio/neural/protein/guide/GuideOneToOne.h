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

#include "bio/neural/protein/AxonGuide.h"

namespace bio {
namespace neural {

class GuideOneToOne:
	public molecular::Class< GuideOneToOne >,
	public AxonGuide
{
public:

	/**
	 * Disambiguates all class methods. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(molecular, GuideOneToOne)

	BIO_DEFAULT_IDENTIFIABLE_CONSTRUCTORS(
		molecular,
		GuideOneToOne,
		filter::Neural()
	)

	/**
	 *
	 */
	virtual ~GuideOneToOne();

	/**
	 * This should be overwritten to add whatever functionality is desired. <br />
	 * It is likely desirable, though not enforced to call Protein::Activate() at the end of any overwritten functions. This will cause all sub-Proteins to be Activated. <br />
	 * @return result of Activation.
	 */
	virtual Code Activate();
};

} // namespace neural
} // namespace bio
