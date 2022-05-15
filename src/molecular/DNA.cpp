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
	mProtein = NULL;
	mVersion = 0.0f;
}

DNA::~DNA()
{
	if (mProtein)
	{
		delete mProtein;
		mProtein = NULL;
	}
}

Protein* DNA::GetProtein()
{
	return mProtein;
}

const Protein* DNA::GetProtein() const
{
	return mProtein;
}

StandardDimension DNA::GetProteinId() const
{
	BIO_SANITIZE(mProtein, ,
		return ProteinPerspective::InvalidId());
	return mProtein->GetId();
}

Version DNA::GetVersion()
{
	return mVersion;
}

void DNA::SetVersion(Version newVersion)
{
	mVersion = newVersion;
}

void DNA::SetProtein(Protein* protein)
{
	mProtein = protein;
}

} //molecular namespace
} //bio namespace
