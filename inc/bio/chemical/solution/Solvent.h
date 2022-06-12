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

#include "Solute.h"

namespace bio {
namespace chemical {

/**
 * Solvents contain Solutes. That is all.
 * NOTE: in real-world chemistry, [chemical] gives the concentration of "chemical" in some solvent. We replicate that here with the [] operator, the only difference is that the caller must specify which Solvent they are talking about.
 * For example, Cytoplasm["Glucose"] would give the Concentration of the Glucose Solute within the Cytoplasm Solvent.
 * This is close to what you should expect from a programming view point, where [] is the "index of" operator: Cytoplasm["Glucose"] is the same as Cytoplasm->GetByName< Solute* >("Glucose")->GetConcentration().
 * NOTE: Solutes need to have Periodic::CheckIn()s. Solvents do not do that. See circulatory::Vessel for that.
 */
class Solvent :
	public chemical::Class< Solvent >,
	public chemical::LinearMotif< Concentration >
{
public:

	BIO_DISAMBIGUATE_ALL_CLASS_METHODS_FOR(chemical, Solvent)

	BIO_DEFAULT_IDENTIFIABLE_CONSTRUCTORS_WITH_COMMON_CONSTRUCTOR(
		chemical,
		Solvent,
		&SolventPerspective::Instance(),
		filter::Chemical
	)

	/**
	 * operator wrappers around GetBy____< Concentration > 
	 * @param soluteId
	 * @return GetBy____< Concentration >
	 */
	virtual Concentration operator[](const Id& soluteId);

	/**
	 * operator wrappers around GetBy____< Concentration > 
	 * @param soluteId
	 * @return GetBy____< Concentration >
	 */
	virtual const Concentration operator[](const Id& soluteId) const;
	
	/**
	 * operator wrappers around GetBy____< Concentration > 
	 * @param soluteName
	 * @return GetBy____< Concentration >
	 */
	virtual Concentration operator[](const Name& soluteName);

	/**
	 * operator wrappers around GetBy____< Concentration > 
	 * @param soluteName
	 * @return GetBy____< Concentration >
	 */
	virtual const Concentration operator[](const Name& soluteName) const;

};

} //chemical namespace
} //bio namespace
