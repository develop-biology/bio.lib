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
#include "bio/molecular/Protein.h"
#include "bio/genetic/common/Types.h"
#include "bio/genetic/common/Filters.h"
#include "bio/genetic/macros/Macros.h"
#include "RNA.h"

namespace bio {
namespace genetic {

class Plasmid;

/**
 * genetic::Expressors contain the logic for storing and querying TranscriptionFactors. <br />
 * NOTE: While TranscriptionFactors are used to control the translation of Plasmid dna into Proteins, Proteins themselves can only be Folded within a Cell, and thus are not included here. <br />
 */
class Expressor :
	public genetic::Class< Expressor >,
	public chemical::UnorderedMotif< TranscriptionFactor >,
	public chemical::LinearMotif< Plasmid* >, 
	public chemical::LinearMotif< molecular::Protein* >, 
	virtual public molecular::Vesicle
{
public:

	/**
	 * Ensure virtual methods point to Class implementations. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(genetic,
		Expressor)

	/**
	 * Standard ctors. <br />
	 */ BIO_DEFAULT_IDENTIFIABLE_CONSTRUCTORS(genetic,
		Expressor,
		&molecular::VesiclePerspective::Instance(),
		filter::Genetic())

	/**
	 *
	 */
	virtual ~Expressor();

	/**
	 * Use this method to populate any member variable Protein* or StandardDimension Ids. 
	 * You'll want to do this to speed up your code by bypassing the dynamic execution provided by Molecule. <br />
	 * Use StandardDimension Ids when the Protein* might change (e.g. the Protein may be Transferred in or out of *this). 
	 * Use a Protein* if you know & will enforce a static set of Proteins which will not be Transferred. 
	 */
	virtual void CacheProteins()
	{
		//   YOUR CODE HERE
	}

	/**
	 * Calls molecular::Protein::Activate() for a molecular::Protein of the given id. <br />
	 * If your Proteins are called often (e.g. on a clock), you may consider caching a pointer to the molecular::Protein and invoking that directly, rather than through this method. <br />
	 * For example, mc_myFavoriteProtein = GetByName<molecular::Protein*>("MyFavoriteProteinName") <br />
	 * If you feel like going through the extra work, you can also wrap those cached calls with #if BIO_MEMORY_OPTIMIZE_LEVEL >= 1 ... #endif so that the downstream user of your code can prefer cache to processing (see common/macros/OptimizeMacros.h for details). <br />
	 * @param proteinId
	 * @return the result of activation or code::BadArgument1 if no such molecular::Protein exists within *this.
	 */
	Code Activate(StandardDimension proteinId);

	/**
	 * Ease of use wrapper around Activate(Id). <br />
	 * @param proteinName
	 * @return the result of Activate after resolving the given name to an Id.
	 */
	Code Activate(Name proteinName);

	/**
	 * Ease of use wrapper around Activate(Id) <br />
	 * This only works is the molecular::Protein's name matches its type (i.e. were constructed with name=PeriodicTable::Instance().GetNameFromType(*this)), which is true of all official Biology Proteins. <br />
	 * NOTE: This method is preferred due to its clarity and will eventually be optimized to go straight from type to Id (e.g. T::GetId()) without the slowdown of intermediate string comparisons & extraneous lookups. <br />
	 * @tparam T
	 * @return the result of Activate after resolving the given type to an Id.
	 */
	template < typename T >
	Code Activate()
	{
		return Activate(TypeName< T >);
	}


	/**
	 * Inserts the molecular::Protein encoded by the mRNA into *this at the correct location. <br />
	 * This essentially encapsulates the Translation and localization process into a single function. <br />
	 * Post-translational modifications can be made by changing the chemical::Properties and/or chemical::States (e.g. Enabled()) of a Transcribed molecular::Protein; additionally, you may create your own system of modifying the Proteins in yourPlasmids. <br />
	 * Multiple Translations of the same mRNA will cause the previously Translated Protein to be removed. <br />
	 * @param mRNA encoded Gene* to be expressed.
	 * @return status of DNA::Translation + localization within *this.
	 */
	virtual Code Translate(const RNA* mRNA); 

	/**
	 * Transcribes all Genes from all Plasmids in *this, iff *this has the necessary TranscriptionFactors for each Gene, populating m_transcriptome. <br />
	 * Then, Translates all mRNA from the m_transcriptome into Proteins. <br />
	 * @return whether or not *this should be functional.
	 */
	virtual Code ExpressGenes();

	/**
	 * Adding RNA to the m_transcriptome will cause the encoded Genes to be Expressed in *this, yielding a Translated Protein. <br />
	 * @param toExpress
	 * @return Whether or not the Gene was added successfully.
	 */
	virtual Code AddToTranscriptome(const RNA* toExpress); 

protected:
	Transcriptome m_transcriptome;
};

} //molecular namespace
} //bio namespace
