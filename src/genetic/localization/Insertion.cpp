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

#include "bio/genetic/localization/Insertion.h"
#include "bio/genetic/macros/Macros.h"
#include "bio/chemical/Substance.h"
#include "bio/common/ByteStream.h"

namespace bio {
namespace genetic {

Insertion::Insertion(
	chemical::Substance* toInsert,
	Site site,
	const Name& name
)
	:
	physical::Class< Insertion >(this),
	Localization(
		0,
		name
	),
	mToInsert(toInsert)
{
	SetSite(site); //virtual means nothing to ctors; do it again.
}

Insertion::~Insertion()
{

}

chemical::Substance* Insertion::Seek(chemical::Substance* insertIn) const
{
	insertIn = ResolvePrevious(insertIn);

	BIO_SANITIZE(insertIn && mToInsert, ,
		return insertIn);

	if (mSite == InsertionSitePerspective::InvalidId())
	{
		return insertIn;
	}

	BIO_SANITIZE(mc_method, ,
		return NULL)
	ByteStream insertion(mToInsert);
	(const_cast< chemical::ExcitationBase* >(mc_method))->EditArg(
		0,
		insertion
	);
	ByteStream result;
	mc_method->CallDown(
		insertIn->AsWave(),
		result
	);
	chemical::Substance* insert = ChemicalCast< chemical::Substance* >(Cast< physical::Wave* >(result.DirectAccess())); //This is about as safe as we can get right now. 
	BIO_SANITIZE(insert, ,
		return NULL)
	return insert;
}

void Insertion::SetSite(Site site)
{
	//Assume Sites & Sites always match exactly.
	mSite = site;
	mc_method = SafelyAccess<InsertionSitePerspective>()->GetNewObjectFromIdAs< chemical::ExcitationBase* >(mSite);
}

void Insertion::InsertThis(chemical::Substance* toInsert)
{
	mToInsert = toInsert;
}

chemical::Substance* Insertion::GetWhatWillBeInserted()
{
	return mToInsert;
}

} //genetic namespace
} //bio namespace
