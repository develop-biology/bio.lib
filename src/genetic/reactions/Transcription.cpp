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

#include "bio/chemical/PeriodicTable.h"
#include "bio/chemical/common/SymmetryTypes.h"
#include "bio/chemical/reactions/TranscribeGene.h"
#include "bio/genetic/Expressor.h"
#inclued "bio/genetic/Gene.h"
#include "bio/genetic/common/Codes.h"

namespace bio {
namespace genetic {

TranscribeGene::TranscribeGene() :
	chemical::Reaction(PeriodicTable::Instance().GetNameFromType(*this)),
	physical::Class(this, new physical::Symmetry(PeriodicTable::Instance().GetNameFromType(*this),symmetry_type::Operation()))
{
	Require<Gene>();
	Require<Expressor>();
}

TranscribeGene::~TranscribeGene()
{
}

Products TranscribeGene::Process(chemical::Substances& reactants)
{
	Gene* gene = reactants[0];
	Expressor* expressor = reactants[1];

	BIO_SANITIZE_AT_SAFETY_LEVEL_2(expressor->GetNumMatching<TranscriptionFactor>(gene->m_requiredTranscriptionFactors) == gene->m_requiredTranscriptionFactors.size(), ,
		return code::GeneNotTranscribed());

	return reactants;
}

} //genetic namespace
} //bio namespace
