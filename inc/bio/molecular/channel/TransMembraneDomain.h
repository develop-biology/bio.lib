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

#include "bio/molecular/Vesicle.h"

namespace bio {
namespace molecular {

/**
 * TransMembraneDomains define the API for accessing and modifying the inner, protected Molecules of Vesicles. <br />
 * The real-world term, "trans-membrane domain", is a little more specific than how we use it here. <br />
 * In the simplest case, a TransMembraneDomain is just an "open hole" in a Vesicle's "membrane". <br />
 * In the most complex case, a single TransMembraneDomain may do the work of many different proteins: similar to how we might refer to a complex quaternary structure as a single protein.<br />
 * Regardless of the complexity of use, a TransMembraneDomain is strictly defined as the interface between the inside and outside of a Vesicle. <br />
 * When defining how a Vesicle should communicate with the outside world or other Vesicles, please create a Pore or similar Surface which includes a TransMembraneDomain. <br />
 * Because friendship does not respect inheritance, this class is non-virtual. This is the ONLY API for defining access to protected Molecules. <br />
 */
class TransMembraneDomain
{
public:
	/**
	 * @param inner
	 */
	TransMembraneDomain(Vesicle* inner = NULL);

	
	/**
	 *
	 */
	~TransMembraneDomain();

	/**
	 * In case the Surface *this is a part of is moved, etc.
	 * @param inner
	 */
	void SetVesicle(Vesicle* inner);


	/**
	 * @return mInner
	 */
	Vesicle* GetVesicle();

	/**
	 * @return mInner
	 */
	const Vesicle* GetVesicle() const;

	
	/**
	 * Ingress (like "ingestion") moves the outer into *this. <br />
	 * *this will assume ownership of outer. <br />
	 * @param outer
	 */
	void Ingress(Molecule* outer);

	
	/**
	 * If you would like to Ingress multiple molecules at once, you may package them into a Vesicle and use this method. <br />
	 * This is essentially endocytosis; however the "cyto" bit isn't applicable at the molecular level. <br />
	 * @param outer
	 */
	void Ingress(Vesicle* outer);

	
	/**
	 * Egress copies Molecules in *this and provides them for export. <br />
	 * If the returned Molecule is not added to another Vesicle, you must delete it to avoid memory leaks. <br />
	 * @param moleculeName 
	 * @return a new Molecule from that of the given Name in *this or NULL if no such Molecule could be found. <br />
	 */
	Molecule* Egress(const Name& moleculeName);

	/**
	 * Egress copies Molecules in *this and provides them for export. <br />
	 * If the returned Molecule is not added to another Vesicle, you must delete it to avoid memory leaks. <br />
	 * @param moleculeName 
	 * @return a new Molecule from that of the given Name in *this or NULL if no such Molecule could be found.
	 */
	const Molecule* Egress(const Name& moleculeName) const;

	
	/**
	 * Egress copies Molecules in *this and provides them for export. <br />
	 * If the returned Molecule is not added to another Vesicle, you must delete it to avoid memory leaks. <br />
	 * NOTE: The Id here is given by the Vesicle's own Perspective (not a global singleton), and must be retrieved before hand through something like: MyVesicle.GetIdFromName("NameOfMyMolecule"). <br />
	 * @param moleculeId
	 * @return a new Molecule from that of the given Id in *this or NULL if no such Molecule could be found.
	 */
	Molecule* Egress(Id moleculeId);

	
	/**
	 * Egress copies Molecules in *this and provides them for export. <br />
	 * If the returned Molecule is not added to another Vesicle, you must delete it to avoid memory leaks. <br />
	 * NOTE: The Id here is given by the Vesicle's own Perspective (not a global singleton), and must be retrieved before hand through something like: MyVesicle.GetIdFromName("NameOfMyMolecule"). <br />
	 * @param moleculeId
	 * @return a new Molecule from that of the given Id in *this or NULL if no such Molecule could be found.
	 */
	const Molecule* Egress(Id moleculeId) const;


	/**
	 * Secrete moves Molecules in *this out, making them unavailable to the mInner Vesicle. <br />
	 * If the returned Molecule is not added to another Vesicle, you must delete it to avoid memory leaks. <br />
	 * NOTE: The Id here is given by the Vesicle's own Perspective (not a global singleton), and must be retrieved before hand through something like: MyVesicle.GetIdFromName("NameOfMyMolecule"). <br />
	 * @param moleculeName
	 * @return an existing Molecule with the given Id in *this or NULL if no such Molecule could be found.
	 */
	Molecule* Secrete(const Name& moleculeName);


	/**
	 * Secrete moves Molecules in *this out, making them unavailable to the mInner Vesicle. <br />
	 * If the returned Molecule is not added to another Vesicle, you must delete it to avoid memory leaks. <br />
	 * NOTE: The Id here is given by the Vesicle's own Perspective (not a global singleton), and must be retrieved before hand through something like: MyVesicle.GetIdFromName("NameOfMyMolecule"). <br />
	 * @param moleculeId
	 * @return an existing Molecule with the given Id in *this or NULL if no such Molecule could be found.
	 */
	Molecule* Secrete(Id moleculeId);


private:
	Vesicle* mInner;
};

} //molecular namespace
} //bio namespace
