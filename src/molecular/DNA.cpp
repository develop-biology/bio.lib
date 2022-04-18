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

#include "bio/molecular/DNA.h"
#include "bio/molecular/Protein.h"
#include "bio/molecular/common/Filters.h"
#include "bio/molecular/common/Codes.h"
#include "bio/molecular/common/Types.h"

namespace bio {
namespace molecular {

void DNA::CtorCommon()
{
	m_protein = NULL;
	m_version = 0.0f;
}

DNA::~DNA()
{
	if (m_protein)
	{
		delete m_protein;
		m_protein = NULL;
	}
}

Protein* DNA::GetProtein() 
{
	return m_protein;
}

const Protein* DNA::GetProtein() const 
{
	return m_protein;
}

StandardDimension DNA::GetProteinId() const
{
	BIO_SANITIZE(m_protein, ,
		return ProteinPerspective::InvalidId());
	return m_protein->GetId();
}

Version DNA::GetVersion()
{
	return m_version;
}

void DNA::SetVersion(Version newVersion)
{
	m_version = newVersion;
}

void DNA::SetProtein(Protein* protein) 
{
	m_protein = protein;
}

} //molecular namespace
} //bio namespace
