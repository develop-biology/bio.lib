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

#include "bio/chemical/structure/StructuralComponent.h"
#include "bio/molecular/Vesicle.h"
#include "bio/molecular/Protein.h"
#include "bio/genetic/common/Types.h"

namespace bio {
namespace genetic {

/**
 * genetic::Expressors contain the logic for storing and querying TranscriptionFactors.
 * NOTE: While TranscriptionFactors are used to control the translation of Plasmid dna into Proteins, Proteins themselves can only be Folded within a Cell, and thus are not included here.
*/
class Expressor :
	virtual public molecular::Vesicle,
	public chemical::StructuralComponent<TranscriptionFactor>,
	public chemical::LinearStructuralComponent<const Plasmid*>,
	public chemical::LinearStructuralComponent<molecular::Protein*>
{
public:
	/**
	 *
	 */
	Expressor();

	/**
	 * @param name
	 */
	Expressor(Name name);

	/**
	 * @param other
	 */
	Expressor(const Expressor& other);


	/**
	 *
	 */
	virtual ~Expressor();


	/**
	 * Required method from Wave. See that class for details.
	 * @return a new copy of *this.
	 */
	virtual Expressor* Clone() const;


	/**
	 * 2 Expressor are equal if they have the same TranscriptionFactors, Molecules, Vesicles, Properties, and States
	 * Neither Plasmids nor Proteins are considered in Expressor equality.
	 * @param other
	 * @return whether or not other is equivalent to *this.
	 */
	virtual bool operator==(const Expressor& other) const;


	/**
	 * Calls Import<>() with all Bonded StructuralComponents.
	 * @param other
	 */
	virtual void ImportAll(const Expressor& other);


	/**
	 * Calls molecular::Protein::operator() for a molecular::Protein of the given id.
	 * If your Proteins are called often (e.g. on a clock), you may consider caching a pointer to the molecular::Protein and invoking that directly, rather than through this method.
	 * For example, mc_myFavoriteProtein = GetByName<molecular::Protein*>("MyFavoriteProteinName")
	 * If you feel like going through the extra work, you can also wrap those cached calls with #if BIO_MEMORY_OPTIMIZE_LEVEL >= 1 ... #endif so that the downstream user of your code can prefer cache to processing (see Optimize.h for details).
	 * @param proteinId
	 * @return the result of activation or code::BadArgument1 if no such molecular::Protein exists within *this.
	 */
	Code Activate(StandardDimension proteinId);

	/**
	 * Ease of use wrapper around Activate(Id).
	 * @param proteinName
	 * @return the result of Activate after resolving the given name to an Id.
	 */
	Code Activate(Name proteinName);

	/**
	 * Ease of use wrapper around Activate(Id)
	 * This only works is the molecular::Protein's name matches its type (i.e. were constructed with name=PeriodicTable::Instance().GetNameFromType(*this)), which is true of all official Biology Proteins.
	 * NOTE: This method is preferred due to its clarity and will eventually be optimized to go straight from type to Id (e.g. T::GetId()) without the slowdown of intermediate string comparisons & extraneous lookups.
	 * @tparam T
	 * @return the result of Activate after resolving the given type to an Id.
	 */
	template <typename T>
	Code Activate()
	{
		return Activate(TypeName<T>);
	}


	/**
	 * Inserts the molecular::Protein encoded by the mRNA into *this at the correct location.
	 * This essentially encapsulates the Translation and localization process into a single function.
	 * Post-translational modifications can be made by changing the chemical::Properties and/or chemical::States (e.g. Enabled()) of a Transcribed molecular::Protein; additionally, you may create your own system of modifying the Proteins in yourPlasmids.
	 * Multiple Translations of the same gene will cause the previously Translated molecular::Protein to be removed.
	 * @param mRNA (aka a Gene, no need for DNA->RNA semantics, we just change the word and keep the type the same).
	 * @return status of Protein Translation (e.g. code::Success()).
	 */
	virtual Code Translate(const Gene* mRNA) = 0;

	/**
	 * Transcribes all Genes from all Plasmids in *this, iff *this has the necessary TranscriptionFactors for each Gene.
	 * @return status of total expression.
	 */
	virtual Code BeginExpressing();

private:
	/**
	 * Common constructor code.
	 */
	void CtorCommon();
};

} //molecular namespace
} //bio namespace
