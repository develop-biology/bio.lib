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

#include "Expressor.h"

namespace bio {
namespace genetic {

/**
 * The Genome is a system for managing Plasmids. <br />
 * This is the Biology package manager. <br />
 *
 * The Genome is a singleton to match the PlasmidPerspective it manages. <br />
 */
class GenomeImplementation :
	public Class< GenomeImplementation >,
	public Expressor,
	virtual public ThreadSafe
{
public:
	/**
	 * Ensure virtual methods point to Class implementations. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(genetic, GenomeImplementation)

	GenomeImplementation();

	virtual ~GenomeImplementation();

	/**
	 * Adds caching for Protein-backed methods. <br />
	 * See Expressor for docs. <br />
	 */
	virtual Code CacheProteins();

	/**
	 * Adds a Plasmid to the PlasmidPerspective. <br />
	 * @param toRegister
	 * @return the Id of the Registered Plasmid.
	 */
	virtual Id RegisterPlasmid(Plasmid* toRegister);

	/**
	 * Get a Plasmid! <br />
	 * @param plasmidId not const for ease of use with Bind.
	 * @return a shared Plasmid*. Should become const in a future release.
	 */
	virtual Plasmid* FetchPlasmid(Id plasmidId);

	/**
	 * Get a Plasmid! <br />
	 * @param plasmidName not const for ease of use with Bind.
	 * @return a shared Plasmid*. Should become const in a future release.
	 */
	virtual Plasmid* FetchPlasmid(Name plasmidName);

protected:
	molecular::Protein* mcRegisterPlasmid;
	molecular::Protein* mcFetchPlasmid;
	Id mcRegistrationSite;
	Id mcNameSite;
	Id mcIdSite;
	Id mcFetchSite;
};

BIO_SINGLETON(Genome, GenomeImplementation)

} //genetic namespace
} //bio namespace
