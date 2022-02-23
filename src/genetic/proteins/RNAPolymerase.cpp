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

#include "bio/genetic/proteins/RNAPolymerase.h"
#include "bio/genetic/Plasmid.h"
#include "bio/genetic/RNA.h"
#include "bio/common/TypeName.h"

namespace bio {
namespace genetic {

RNAPolymerase::RNAPolymerase(molecular::Plasmid* toTranscribe)
	:
	molecular::Protein(chemical::PeriodicTable::Instance().GetNameFromType(*this))
{
	SetSource(toTranscribe);
	mc_rna = Define("RNA Binding Site");
}

RNAPolymerase::~RNAPolymerase()
{

}

Code RNAPolymerase::Activate()
{
	Expressor* expressor = GetEnvironment();
	BIO_SANITIZE(environment, ,
		return code::BadArgument1());
	RNA* boundRNA = RotateTo<RNA*>(mc_rna);
	BIO_SANITIZE(m_rna, ,
		return code::BadArgument2());

	bool shouldTranscribe = false;
	for (
		chemical::StructuralComponentImplementation< Gene* >::Contents::const_iterator gen = m_source->GetAll< Gene* >()->begin();
		gen != m_source->GetAll< Gene* >()->end();
		++gen
		)
	{
		shouldTranscribe = expressor->GetNumMatching< TranscriptionFactor >(gene->m_requiredTranscriptionFactors) == gene->m_requiredTranscriptionFactors.size();
		if (!shouldTranscribe)
		{
			continue;
		}
		boundRNA->Add< Gene* >(gen);
	}
}

void RNAPolymerase::FeedRNA(RNA* toTranscribeOnto)
{
	m_rna = toTranscribeOnto;
}

RNA* RNAPolymerase::GetTranscribedRNA()
{
	return m_rna;
}

} //genetic namespace
} //bio namespace
