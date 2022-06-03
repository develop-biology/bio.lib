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

#include "bio/genetic/Expressor.h"
#include "bio/genetic/Plasmid.h"
#include "bio/genetic/common/Codes.h"

namespace bio {
namespace genetic {

Expressor::~Expressor()
{

}

Code Expressor::Activate(const Id& proteinId)
{
	Code ret = code::Success();
	molecular::Protein* toActivate = GetById< molecular::Protein* >(proteinId);
	BIO_SANITIZE(toActivate, ,
		return false);
	return toActivate->Activate();
}

Code Expressor::Activate(const Name& proteinName)
{
	return Activate(SafelyAccess<molecular::ProteinPerspective>()->GetIdWithoutCreation(proteinName));
}

Code Expressor::ExpressGenes()
{
	Plasmid* plasmid;
	Code ret = code::Success();
	for (
		SmartIterator dna = GetAll< Plasmid* >()->Begin();
		!dna.IsAfterEnd();
		++dna
		)
	{
		plasmid = dna;
		if (AddToTranscriptome(plasmid->TranscribeFor(this)) != code::Success() && ret == code::Success())
		{
			ret = code::TranscriptionError();
		}
	}
	for (
		SmartIterator rna = mTranscriptome.Begin();
		!rna.IsAfterEnd();
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
	mTranscriptome.Add(toExpress);
	return code::Success();
}

Code Expressor::Translate(const RNA* mRNA)
{
	BIO_SANITIZE(mRNA, ,
		code::BadArgument1())

	Code ret = code::Success();

	Gene* gene;
	for (
		SmartIterator rna = mTranscriptome.Begin();
		!rna.IsAfterEnd();
		++rna
		)
	{
		for (
			SmartIterator gen = rna.As< const RNA* >()->GetAll< Gene* >()->Begin();
			!gen.IsAfterEnd();
			++gen
			)
		{
			gene = gen;
			if (!gene->mInsertion.Seek(this))
			{
				ret = code::UnknownError();
			}
		}
	}
	return ret;
}

} //molecular namespace
} //bio namespace
