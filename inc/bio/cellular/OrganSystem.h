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

#include "bio/cellular/common/Class.h"
#include "bio/cellular/common/Types.h"
#include "bio/cellular/common/Filters.h"
#include "bio/cellular/macros/Macros.h"
#include "bio/cellular/Organ.h"
#include "bio/genetic/Plasmid.h"
#include "bio/chemical/structure/LinearStructuralComponent.h"

namespace bio {
namespace cellular {

class Organ;

/**
 * OrganSystems are very similar to Organs. The only difference is that they contain logic for combining multiple Organs.
 * For example, the heart relies on vasculature to move blood around the body. This requires a Heart Organ as well as integrations into all other Tissues in order to supply the BloodStream. In order to accommodate this functionality, we must invasively add functionality to other Organs.
 * Because the modification of existing Organs can lead to more errors and make debugging more difficult, it is not recommended that you take advantage of OrganSystem machinery unless you absolutely need to. However, if you do need to make system-wide modifications, these methods are available to you.
 */
class OrganSystem :
	Class< OrganSystem >,
	public chemical::LinearStructuralComponent< Organ* >
{
public:

	/**
	 * Ensure virtual methods point to Class implementations.
	 */
	BIO_DISAMBIGUATE_CLASS_METHODS(cellular,
		OrganSystem)

	/**
	 * Standard ctors.
	 */ BIO_DEFAULT_IDENTIFIABLE_CONSTRUCTORS(cellular,
		OrganSystem,
		&OrganSystemPerspective::Instance(),
		filter::Cellular())


	/**
	 *
	 */
	virtual ~OrganSystem();

	/**
	 * Create all Organs and, if necessary, link them.
	 * Call the parent method (OrganSystem::Organogenesis()) when done to initialize all Organs (calls Organ::BuildMobilome() and Organ::SpecializeTissues()).
	 */
	virtual Code Organogenesis();
};

} //cellular namespace
} //bio namespace
