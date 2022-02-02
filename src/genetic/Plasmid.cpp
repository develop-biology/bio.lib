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

#include "bio/genetic/Plasmid.h"
#include "bio/genetic/Filters.h"
#include "bio/physical/Codes.h"
#include "bio/physical/Types.h"
#include "bio/cellular/Cell.h"
#include "bio/molecular/Protein.h"
#include <cstring>

using namespace bio;
using namespace cellular;
using namespace genetic;

Plasmid::Plasmid(Name name, PlasmidVersion version, log::Engine* logEngine) :
	genetic::Class<Plasmid>(this, filter::Genetic(), logEngine),
	m_version(version)
{
    BIO_LOG_DEBUG("Created %s v%u", GetName(), m_version);
}

Plasmid::~Plasmid()
{

}

virtual void ImportAll(const Plasmid& other)
{
	chemical::Substance::ImportAll(other);
	Import<Gene*>(other);
}


PlasmidVersion GetVersion()
{
	return m_version;
}

void Plasmid::SetVersion(PlasmidVersion newVersion)
{
    m_version = newVersion;
}
