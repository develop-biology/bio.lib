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
 * TransMembraneDomains define the API for accessing and modifying the interior, protected Molecules of Vesicles. <br />
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
	 * @param interior
	 */
	TransMembraneDomain(Vesicle* interior = NULL);

	
	/**
	 *
	 */
	~TransMembraneDomain();

	/**
	 * In case the Surface *this is a part of is moved, etc.
	 * @param interior
	 */
	void SetVesicle(Vesicle* interior);


	/**
	 * @return mInterior
	 */
	Vesicle* GetVesicle();

	/**
	 * @return mInterior
	 */
	const Vesicle* GetVesicle() const;

	
	/**
	 * Ingress (like "ingestion") moves the external into *this. <br />
	 * If the external Molecule is already Dissolved in another Solution, *this will cause that Solution to Efflux the given Molecule so that it may be Influxed into *this. <br />
	 * Otherwise, the external Molecule will be Dissolved in *this. <br />
	 * @param external
	 */
	void IngressMolecule(Molecule* external);

	
	/**
	 * If you would like to Ingress multiple Molecules at once, you may provide the Solution those Molecules are Dissolved in. <br />
	 * This is essentially endocytosis; however the "cyto" bit isn't applicable at the molecular level. <br />
	 * @param external
	 */
	void IngressSolution(chemical::Solution* external);

	
	/**
	 * Egress copies Molecules in *this and provides them for export. <br />
	 * If the returned Molecule is not added to another Vesicle, you must delete it to avoid memory leaks. <br />
	 * NOTE: Egress is non-const. You must have write access to a Vesicle in order to read it's protected members. Also, any modifications made to the returned Solute will likely be Mixed back into *this. <br />
	 * @param soluteName 
	 * @return a Solute containing a Dissolved Substance with the given Id Effluxed out of the mInterior Vesicle.
	 */
	chemical::Solute& Egress(const Name& soluteName);
	
	/**
	 * Egress copies Molecules in *this and provides them for export. <br />
	 * If the returned Molecule is not added to another Vesicle, you must delete it to avoid memory leaks. <br />
	 * NOTE: Egress is non-const. You must have write access to a Vesicle in order to read it's protected members. Also, any modifications made to the returned Solute will likely be Mixed back into *this. <br />
	 * @param soluteId
	 * @return a Solute containing a Dissolved Substance with the given Id Effluxed out of the mInterior Vesicle.
	 */
	chemical::Solute& Egress(const Id& soluteId);

	/**
	 * Secrete moves Molecules in *this out, making them unavailable to the mInterior Vesicle. <br />
	 * If the returned Molecule is not added to another Vesicle, you must delete it to avoid memory leaks. <br />
	 * NOTE: you cannot Secrete a Molecule which has a Concentration > 1 (or 0). Removing a Molecule which is referenced by other Solutions is not currently supported. <br /> 
	 * @param soluteName
	 * @return the Clone of the Solute with the matching Id & which has been Erased from *this.
	 */
	chemical::Solute* Secrete(const Name& soluteName);


	/**
	 * Secrete moves Molecules in *this out, making them unavailable to the mInterior Vesicle. <br />
	 * If the returned Molecule is not added to another Vesicle, you must delete it to avoid memory leaks. <br />
	 * NOTE: you cannot Secrete a Molecule which has a Concentration > 1 (or 0). Removing a Molecule which is referenced by other Solutions is not currently supported. <br />
	 * @param soluteId
	 * @return the Clone of the Solute with the matching Id & which has been Erased from *this.
	 */
	chemical::Solute* Secrete(const Id& soluteId);


private:
	Vesicle* mInterior;
};

} //molecular namespace
} //bio namespace
