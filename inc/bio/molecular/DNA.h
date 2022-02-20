/*
 * This file is a part of the Biology project by eons LLC.
 * Biology (aka Develop Biology) is a framework for approaching software
 * development from a natural sciences perspective.
 *
 * Copyright (C) 2021 Séon O'Shannon & eons LLC
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

#include "Molecule.h"
#include "bio/molecular/common/Types.h"
#include "bio/molecular/common/Class.h"

namespace bio {
namespace molecular {

/**
 * DNA allows Proteins to be stored as variables and be Transcribed and Translated later.
 * In order to improve debugging, Proteins store a pointer to their encoding DNA (similar to how a RISC-like mechanism might identify similar mRNA to that with produced the Protein in order to down-regulate its production; for more info, look into RNA Interference).
 * Because Proteins will point back to their parent DNA, we also provide a name and version for each DNA Molecule. This helps to identify Proteins that might be otherwise indistinguishable.
 * NOTE: genetic/Gene.h has most of the DNA implementation.
 *
 * Also note that, in reality, a single allele (Protein*) may result in the production of multiple proteins. That is true here as well; however, that logic is abstracted to a Protein's sub-proteins.
 * In other words if you want 1 DNA Molecule to produce several Proteins, you must Add<Protein*>() those to the Protein in *this.
 */
class DNA :
	virtual public Molecule,
	public Class<DNA>
{
public:

	/**
	 * @param name
	 * @param version
	 */
	DNA(Name name, Version version = 0);

	/**
	 * @param id
	 * @param version
	 */
	DNA(StandardDimension id, Version version = 0);

	/**
	 *
	 */
	virtual ~DNA();

	/**
	 * @return a new Protein from what *this encodes or NULL (e.g. if m_protein is NULL or Clone() somehow fails).
	 */
	Protein* Translate() const;

	/**
	 * @return the Id of the Protein *this encodes.
	 */
	StandardDimension GetProteinId() const;

	/**
	 * @return the m_version of *this.
	 */
	Version GetVersion();

	/**
	 * Set the Version of *this.
	 * @param newVersion
	 */
	void SetVersion(Version newVersion);

protected:
	Protein* m_protein;
	Version m_version;
};

} //cellular namespace
} //bio namespace
