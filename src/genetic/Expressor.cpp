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

#include "bio/genetic/Expressor.h"
#include "bio/genetic/common/Codes.h"
#include "bio/genetic/reactions/ExpressPlasmid.h"

namespace bio {
namespace genetic {

Expressor::Expressor(const Expressor& other)
	:
	chemical::StructuralComponent< TranscriptionFactor >(other),
	chemical::LinearStructuralComponent< const Plasmid* >(other),
	chemical::LinearStructuralComponent< Protein* >(),
	molecular::Vesicle(other),
	m_transcriptome()
{

}

Expressor::~Expressor()
{

}

Code Expressor::Activate(StandardDimension proteinId)
{
	ret = code::Success();
	Protein* toActivate = GetById< Protein* >(
		proteinId,
		false
	);
	BIO_SANITIZE(toActivate, ,
		return false);
	return toActivate->Activate();
}

Code Expressor::Activate(Name proteinName)
{
	return Activate(molecular::ProteinPerspective::Instance().IdFromName(proteinName));
}

Code Expressor::ExpressGenes()
{
	Code ret = code::Success();
	for (
		chemical::StructuralComponentImplementation< const Plasmid* >::Contents::const_iterator dna = GetAll< const Plasmid* >()->begin();
		dna != GetAll< const Plasmid* >()->end();
		++dna
		)
	{
		if (AddToTranscriptome((*dna)->TranscribeFor(this)) != code::Success() && ret == code::Success())
		{
			ret = code::TranscriptionError();
		}
	}
	for (
		chemical::StructuralComponentImplementation< Gene* >::Contents::const_iterator rna = m_transcriptome.GetAll< Gene* >()->begin();
		rna != m_transcriptome.GetAll< Gene* >()->end();
		++rna
		)
	{
		if (Translate(*rna) != code::Success() && ret == code::Success())
		{
			ret = code::TranslationError();
		}
	}
	return ret;
}

Code Expressor::AddToTranscriptome(const RNA* toExpress)
{
	BIO_SANITIZE(toExpress, ,
		return code::BadArgument1());
	m_transcriptome.push_back(toExpress);
	return code::Success();
}

Code Expressor::Translate(const RNA* mRNA)
{
	BIO_SANITIZE(mRNA, ,
		code::BadArgument1());


	for (
		chemical::StructuralComponentImplementation< Gene* >::Contents::const_iterator rna = mRNA->GetAll< Gene* >()->begin();
		rna != m_transcriptome.GetAll< Gene* >()->end();
		++rna
		)
	{
		for (
			Names::const_iterator nam = (*rna)->m_localization.begin();
			nam != m_localization.end();
			++nam
			)
		{
			Surface*
		}
	}
}

} //molecular namespace
} //bio namespace
