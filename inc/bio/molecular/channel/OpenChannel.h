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

#include "Pore.h"

namespace bio {
namespace molecular {

/**
 * OpenChannels provide direct, pass-through access to all Molecules in a Vesicle. <br />
 * When creating your own Channels, you may want to derive from this and specify when the Channel is "closed", add some monitoring / logging, etc. <br />
 */
class OpenChannel :
	public Class< OpenChannel >,
	public Pore
{
public:

	/**
	 * Ensure virtual methods point to Class implementations. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(molecular, OpenChannel)

	/**
	 * Don't use this.
	 */
	OpenChannel();

	/**
	 * @param name
	 * @param environment
	 */
	OpenChannel(
		const Name& name,
		Vesicle* environment = NULL
	);

	/**
	 *
	 */
	virtual ~OpenChannel();

	/**
	 * Per TransMembraneDomain; see that class for details. <br />
	 * @param external
	 */
	virtual void IngressMolecule(Molecule* external);


	/**
	 * Per TransMembraneDomain; see that class for details. <br />e
	 * @param external
	 */
	virtual void IngressSolution(chemical::Solution* external);


	/**
	 * Per TransMembraneDomain; see that class for details. <br />
	 * @param soluteName
	 * @return a Solute containing a Dissolved Substance with the given Id Effluxed out of the mInterior Vesicle.
	 */
	virtual chemical::Solute& Egress(const Name& soluteName);

	/**
	 * Per TransMembraneDomain; see that class for details. <br />r
	 * @param soluteId
	 * @return a Solute containing a Dissolved Substance with the given Id Effluxed out of the mInterior Vesicle.
	 */
	virtual chemical::Solute& Egress(const Id& soluteId);

	/**
	 * Per TransMembraneDomain; see that class for details. <br />
	 * @param soluteName
	 * @return the Clone of the Solute with the matching Id & which has been Erased from *this.
	 */
	virtual chemical::Solute* Secrete(const Name& soluteName);


	/**
	 * Per TransMembraneDomain; see that class for details. <br />
	 * @param soluteId
	 * @return the Clone of the Solute with the matching Id & which has been Erased from *this.
	 */
	virtual chemical::Solute* Secrete(const Id& soluteId);
};
} //molecular namespace
} //bio namespace
