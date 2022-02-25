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

#include "bio/chemical/structure/LinearStructuralComponent.h"
#include "bio/genetic/common/Types.h"
#include "bio/genetic/common/Class.h"
#include "Gene.h"

namespace bio {
namespace genetic {

class Expressor;

/**
 * The purpose of a Plasmid is to group logically similar Proteins into a single unit that can be easily distributed and applied to Biological projects.
 * Essentially, a Plasmid is a library; the code it stores is simply restricted to Proteins, Molecules, and other Biological classes for the purpose of integrating with the Biology framework.
 * You could create your own Plasmid that would make function calls to your own C++ library or other system. In that Plasmid, you would want to define a set of Proteins and Genes that would make such calls. Those Genes would then be Expressed in Cells and you would have bridged your framework and Biology.
 *
 * Another benefit of modularizing code with through systems like this is that it allows updates, patches, and features to be rolled out incrementally, without disrupting service.
 * For example, if your friend creates their MyFavoritePlasmid, which contains your friend's MyFavoriteProtein and you would like the same behavior but done in a different way (e.g. a security patch or extra api call), you can create a stand-in replacement for your friend's MyFavoriteProtein and override just that one function while still using the rest of friend's MyFavoritePlasmid. You can then share your Plasmid with your friend and they can upgrade their systems by including it!
 *
 * A brief bit of history: this whole framework was originally built when the sales lead wanted features to show investors that were in direct conflict with what the technical lead requested. Instead of forcing management to reconcile, Seon just developed a system to meet both needs simultaneously.
 *
 * A lot happens on Plasmid creation. The first Protein *this will produce is RNATranscriptase, which is used for Transcribing the rest of the Genes in *this. This process will produce all Proteins used by the full system. If the log::Engine is supplied in the Plasmid constructor, any messages produced during the Protein creation process will be logged; otherwise, these messages will be ignored and the logEngine will be specified when the Proteins are Folded in their Cell.
 * Regarding Writer SetLogEngine, Plasmids do not forward any such calls onto their Genes, as they assume the Proteins will not use logs in their constructors and that the log engine will be set by the cellular::Protein or cellular::Cell they are added to (prior to initialization). This will be addressed in a future release.
 *
 */
class Plasmid :
	public genetic::Class<Plasmid>,
	public chemical::LinearStructuralComponent< Gene* >,
	public molecular::DNA,
	virtual public ThreadSafe
{
public:

	/**
	 * Ensure virtual methods point to Class implementations.
	 */
	BIO_DISAMBIGUATE_CLASS_METHODS(genetic,
		Plasmid)

	/**
	 * Standard ctors.
	 * These are easy to use but require setting member variables manually.
	 */
	BIO_DEFAULT_IDENTIFIABLE_CONSTRUCTORS_WITH_CTOR_COMMON(genetic,
		Plasmid,
		&PlasmidPerspective::Instance(),
		filter::Genetic())

	/**
	 *
	 */
	virtual ~Plasmid();

	/**
	 * RNA polymerase is responsible for transcribing DNA into the mRNA that is then translated into protein.
	 * Similarly, here, RNAPolymerase is responsible for Transcribing Genes.
	 * If you would like your own custom unpacking system for your Plasmid, override this method.
	 * If you don't need anything fancy, leave this as is.
	 * @return A Protein* that will carry out the Transcription of *this.
	 */
	virtual molecular::Protein* GetRNAPolymerase();
	virtual const molecular::Protein* GetRNAPolymerase() const;

	/**
	 * Transcribes *this in the context of the given Expressor.
	 * This does not alter either the expressor given nor *this but, instead creates an new RNA* that can be added to the expressor's Transcriptome.
	 * Uses the RNAPolymerase from *this.
	 * @param expressor
	 * @return a new RNA* to be Expressed.
	 */
	virtual RNA* TranscribeFor(Expressor* expressor) const;

private:
	/**
	 * common constructor code.
	 */
	void CtorCommon();
};

} //genetic namespace
} //bio namespace
