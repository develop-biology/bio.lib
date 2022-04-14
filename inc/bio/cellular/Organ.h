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
#include "bio/cellular/Tissue.h"
#include "bio/genetic/Plasmid.h"
#include "bio/molecular/EnvironmentDependent.h"
#include "bio/chemical/structure/motif/LinearMotif.h"

namespace bio {
namespace cellular {

class Tissue;

class OrganSystem;

/**
 * An Organ is a collection of Tissues that can operate on its own.
 * Similar to how Plasmids are packages of Genes, Organs are packages of Tissues.
 * In this way, Organs are like pseudo-binaries that can be integrated into other binaries.
 *
 * Organs contain all of the DNA (Plasmids) they need to create all of their Cells.
 * This means you should new the Plasmids necessary for your Organ within the BuildMobilome method.
 *
 * Once your Organ is prepared, you can initialize it with SpecializeTissues() and run it with Peak. However, these will be done for you through the parent OrganSystem.
 */
class Organ :
	public Class< Organ >,
	public chemical::LinearMotif< genetic::Plasmid* >,
	public chemical::LinearMotif< Tissue* >,
	public molecular::EnvironmentDependent< OrganSystem >
{
public:

	/**
	 * Ensure virtual methods point to Class implementations.
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(cellular,
		Organ)

	/**
	 * Standard ctors.
	 */ BIO_DEFAULT_IDENTIFIABLE_CONSTRUCTORS(cellular,
		Organ,
		&OrganPerspective::Instance(),
		filter::Cellular())

	virtual ~Organ();

	/**
	 * new all necessary Plasmids.
	 * Does NOT distribute them. See SpecializeTissues for that.
	 * NOTE: we pronounce "mobilome" as "mobile-lee-ome" because it's more fun.
	 */
	virtual Code BuildMobilome()
	{
		//     CREATE YOUR PLASMIDS HERE!

		return code::NotImplemented();
	}

	/**
	 * new all Tissues.
	 * Does NOT Differentiate them. See SpecializeTissues for that.
	 */
	virtual Code GrowTissues()
	{
		//     CREATE YOUR TISSUES HERE!

		return code::NotImplemented();
	}

	/**
	 * Differentiate all Cells in all Tissues.
	 */
	virtual Code SpecializeTissues();
};


} //cellular namespace
} //bio namespace
