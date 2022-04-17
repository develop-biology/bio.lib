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

#include "Molecule.h"
#include "bio/molecular/common/Types.h"
#include "bio/molecular/common/Class.h"

namespace bio {
namespace molecular {

class Protein;

/**
 * DNA allows Proteins to be stored as variables and be Transcribed and Translated later. <br />
 * In order to improve debugging, Proteins store a pointer to their encoding DNA (similar to how a RISC-like mechanism might identify similar mRNA to that with produced the Protein in order to down-regulate its production; for more info, look into RNA Interference). <br />
 * Because Proteins will point back to their parent DNA, we also provide a name and version for each DNA Molecule. This helps to identify Proteins that might be otherwise indistinguishable. <br />
 * NOTE: genetic/Gene.h has most of the DNA interface. <br />
 *
 * Also note that, in reality, a single allele (Protein*) may result in the production of multiple proteins. That is true here as well; however, that logic is abstracted to a Protein's sub-proteins. <br />
 * In other words if you want 1 DNA Molecule to produce several Proteins, you must Add<Protein*>() those to the Protein in *this. <br />
 */
class DNA :
	public Class< DNA >,
	virtual public Molecule
{
public:

	/**
	 * Ensure virtual methods point to Class implementations. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(molecular,
		DNA)

	/**
	 * Standard ctors. <br />
	 * These are easy to use but require setting the Version after instantiation. <br />
	 * For example: <br />
	 * 		DNA myAllele = DNA("MyGene"); <br />
	 * 		myAllele.SetVersion(1.0); <br />
	 */ BIO_DEFAULT_IDENTIFIABLE_CONSTRUCTORS_WITH_CTOR_COMMON(molecular,
		DNA,
		&DNAPerspective::Instance(),
		filter::Molecular())

	/**
	 *
	 */
	virtual ~DNA();

	/**
	 * See classes in the genetic namespace for proper Translation of Protein. <br />
	 * @return the Protein in *this.
	 */
	virtual Protein* GetProtein(); <br />

	/**
	 * See classes in the genetic namespace for proper Translation of Protein. <br />
	 * @return the Protein in *this.
	 */
	virtual const Protein* GetProtein() const; <br />

	/**
	 * Change what *this encodes. <br />
	 * @param protein
	 */
	virtual void SetProtein(Protein* protein); <br />

	/**
	 * @return the Id of the Protein *this encodes.
	 */
	virtual StandardDimension GetProteinId() const;

	/**
	 * @return the m_version of *this.
	 */
	virtual Version GetVersion();

	/**
	 * Set the Version of *this. <br />
	 * @param newVersion
	 */
	virtual void SetVersion(Version newVersion);

protected:
	Protein* m_protein; <br />
	Version m_version;

private:
	/**
	 * common constructor code. <br />
	 */
	void CtorCommon();
};

} //molecular namespace
} //bio namespace
