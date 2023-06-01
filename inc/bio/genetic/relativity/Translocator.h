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

#include "SignalPeptide.h"
#include "bio/physical/relativity/Perspective.h"
#include "bio/chemical/reaction/Excitation.h"
#include "bio/genetic/common/Types.h"

namespace bio {
namespace genetic {

/**
 * The Translocator is responsible for moving Genes to their appropriate Location within a genetic::Expressor (e.g. a Cell). <br />
 * 
 */
class TranslocatorImplementation :
	public physical::Perspective< Location >
{
public:

	/**
	 *
	 */
	TranslocatorImplementation();

	/**
	 *
	 */
	virtual ~TranslocatorImplementation();



	/**
	 * Store an Excitation as a Peptidase with the given epitope at the given location. <br />
	 * If an Excitation was previously Associated, it must be Disassociated before it can be changed <br />
	 * @param location
	 * @param epitope
	 * @param peptidase
	 * @return true if the association completed successfully else false
	 */
	bool AssociateSignalPeptidase(
		Location location,
		Epitope epitope,
		chemical::ExcitationBase* peptidase
	);

	/**
	 * Removes the Associated Excitation for the given epitope at the given location. <br />
	 * @param location
	 * @param epitope
	 * @return true if the association was removed else false.
	 */
	bool DisassociateSignalPeptidase(
		Location location,
		Epitope epitope
	);

	/**
	 * Get a previously Associated Excitation for the given epitope at the given location. <br />
	 * Make sure to delete the returned Excitation! <br />
	 * @param location
	 * @param epitope
	 * @return a new chemical::Excitation* or NULL
	 */
	chemical::ExcitationBase* GetPeptidase(
		Location location,
		Epitope epitope);

	/**
	 * Get a previously Associated Excitation for the given epitope at the given location. <br />
	 * Make sure to delete the returned Excitation! <br />
	 * @param location
	 * @param epitope
	 * @return a new chemical::Excitation* or NULL
	 */
	chemical::ExcitationBase* GetPeptidase(
		Location location,
		const Name& epitope);


protected:
	/**
	 * Create a SignalPeptide instead of any other kind of Brane. <br />
	 * @param id
	 * @param name
	 * @return a new SignalPeptide.
	 */
	virtual physical::Brane< Location >* CreateBrane(Location id, const Name& name);
};

BIO_SINGLETON(Translocator, TranslocatorImplementation)

} //genetic namespace
} //bio namespace
