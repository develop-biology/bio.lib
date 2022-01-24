
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

#include "bio/genetic/Gene.h"
#include "bio/genetic/Properties.h"

namespace bio {
namespace genetic {

Gene::Gene(
	const TranscriptionFactors& requiredTranscriptionFactors,
	cellular::Protein* protein,
	StandardDimensions localization,
	Position position = BOTTOM,
	const StandardDimension optionalInsertionArg = 0,
	bool transferSubProteins = false)
	:
	m_requiredTranscriptionFactors(requiredTranscriptionFactors)
	m_allele(protein),
	m_localization(localization),
	m_position(position),
	m_optionalInsertionParameter(optionalInsertionArg),
	m_transferSubProteins(transferSubProteins)
{
	Add<Property>(chemical::DNA());
	Add<Property>(chemical::Genetic());
}

Gene::~Gene()
{
}

} //namespace genetic
} //namespace bio