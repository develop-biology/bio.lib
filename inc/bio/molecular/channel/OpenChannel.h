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
	 */
	OpenChannel(
		const Name& name,
		Vesicle* environment = NULL
	);

	virtual ~OpenChannel();

	/**
	 * Per TransMembraneDomain. <br />
	 * Ingress (like "ingestion") moves the outer into *this. <br />
	 * *this will assume ownership of outer. <br />
	 * @param outer
	 */
	virtual void Ingress(Molecule* outer);


	/**
	 * If you would like to Ingress multiple molecules at once, you may package them into a Vesicle and use this method. <br />
	 * This is essentially endocytosis; however the "cyto" bit isn't applicable at the molecular level. <br />
	 * @param outer
	 */
	virtual void Ingress(Vesicle* outer);


	/**
	 * Egress copies Molecules in *this and provides them for export. <br />
	 * If the returned Molecule is not added to another Vesicle, you must delete it to avoid memory leaks. <br />
	 * @param moleculeName
	 * @return a new Molecule from that of the given Name in *this or NULL if no such Molecule could be found. <br />
	 */
	virtual Molecule* Egress(const Name& moleculeName);

	/**
	 * Egress copies Molecules in *this and provides them for export. <br />
	 * If the returned Molecule is not added to another Vesicle, you must delete it to avoid memory leaks. <br />
	 * @param moleculeName
	 * @return a new Molecule from that of the given Name in *this or NULL if no such Molecule could be found.
	 */
	virtual const Molecule* Egress(const Name& moleculeName) const;


	/**
	 * Egress copies Molecules in *this and provides them for export. <br />
	 * If the returned Molecule is not added to another Vesicle, you must delete it to avoid memory leaks. <br />
	 * NOTE: The Id here is given by the Vesicle's own Perspective (not a global singleton), and must be retrieved before hand through something like: MyVesicle.GetIdFromName("NameOfMyMolecule"). <br />
	 * @param moleculeId
	 * @return a new Molecule from that of the given Id in *this or NULL if no such Molecule could be found.
	 */
	virtual Molecule* Egress(const Id& moleculeId);


	/**
	 * Egress copies Molecules in *this and provides them for export. <br />
	 * If the returned Molecule is not added to another Vesicle, you must delete it to avoid memory leaks. <br />
	 * NOTE: The Id here is given by the Vesicle's own Perspective (not a global singleton), and must be retrieved before hand through something like: MyVesicle.GetIdFromName("NameOfMyMolecule"). <br />
	 * @param moleculeId
	 * @return a new Molecule from that of the given Id in *this or NULL if no such Molecule could be found.
	 */
	virtual const Molecule* Egress(const Id& moleculeId) const;


	/**
	 * Secrete moves Molecules in *this out, making them unavailable to the mInner Vesicle. <br />
	 * If the returned Molecule is not added to another Vesicle, you must delete it to avoid memory leaks. <br />
	 * NOTE: The Id here is given by the Vesicle's own Perspective (not a global singleton), and must be retrieved before hand through something like: MyVesicle.GetIdFromName("NameOfMyMolecule"). <br />
	 * @param moleculeName
	 * @return an existing Molecule with the given Id in *this or NULL if no such Molecule could be found.
	 */
	virtual Molecule* Secrete(const Name& moleculeName);


	/**
	 * Secrete moves Molecules in *this out, making them unavailable to the mInner Vesicle. <br />
	 * If the returned Molecule is not added to another Vesicle, you must delete it to avoid memory leaks. <br />
	 * NOTE: The Id here is given by the Vesicle's own Perspective (not a global singleton), and must be retrieved before hand through something like: MyVesicle.GetIdFromName("NameOfMyMolecule"). <br />
	 * @param moleculeId
	 * @return an existing Molecule with the given Id in *this or NULL if no such Molecule could be found.
	 */
	virtual Molecule* Secrete(const Id& moleculeId);
};
} //molecular namespace
} //bio namespace
