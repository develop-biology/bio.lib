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

#include "bio/genetic/protein/RNAPolymerase.h"
#include "bio/genetic/Expressor.h"
#include "bio/genetic/Plasmid.h"
#include "bio/genetic/RNA.h"
#include "bio/common/type/TypeName.h"

namespace bio {
namespace genetic {

RNAPolymerase::RNAPolymerase(Plasmid* toTranscribe)
	:
	genetic::Class< RNAPolymerase >(this, "RNA Polymerase", filter::Genetic()),
	molecular::Protein(SafelyAccess< chemical::PeriodicTable >()->GetNameFromType(*this))
{
	SetSource(toTranscribe);
	mcRna = Define("RNA Binding Site");
}

RNAPolymerase::~RNAPolymerase()
{

}

Code RNAPolymerase::Activate()
{
	Expressor* expressor = ChemicalCast< Expressor* >(this->template GetEnvironment< molecular::Vesicle* >());

	BIO_SANITIZE(expressor, , return code::BadArgument1());

	RNA* boundRNA = RotateTo(mcRna)->As< RNA* >();

	BIO_SANITIZE(mcRna, , return code::BadArgument2());

	Gene* gene;
	bool shouldTranscribe = false;
	for (
		SmartIterator gen = mSource->GetAll< Gene* >()->Begin();
		!gen.IsAfterEnd();
		++gen
		)
	{
		gene = gen;
		shouldTranscribe = expressor->HasAll< TranscriptionFactor >(
			gene->GetAll< TranscriptionFactor >());

		if (!shouldTranscribe)
		{
			continue;
		}
		boundRNA->Add< Gene* >(gene);
	}
	return code::Success();
}
} //genetic namespace
} //bio namespace
