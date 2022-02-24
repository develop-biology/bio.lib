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

#include "bio/genetic/Localization.h"
#include "bio/genetic/macros/Macros.h"
#include "bio/chemical/reaction/Excitation.h"
#include "bio/chemical/Substance.h"
#include "bio/common/ByteStream.h"

namespace bio {
namespace genetic {

Localization::Localization(
	LocalizationSite site,
	Name name
)
	:
	m_site(site),
	m_name(name),
	m_next(NULL)
{

}

Localization::~Localization()
{

}

/*static*/ chemical::Substance* Localization::Seek(
	const Localization* locale,
	chemical::Substance* seekIn,
	bool* donePtr
)
{
	BIO_SANITIZE(locale && seekIn, ,
		return seekIn);

	if (locale->m_site == LocalizationSitePerspective::InvalidId())
	{
		if(donePtr)
		{
			*donePtr = true;
		}
		return seekIn;
	}

	chemical::ExcitationBase* extractionMethod = LocalizationSitePerspective::Instance().GetNewObjectFromIdAs< chemical::ExcitationBase* >(locale->m_site);
	BIO_SANITIZE(extractionMethod,, return NULL);
	ByteStream newName(locale->m_name);
	extractionMethod->EditArg(0, newName);
	ByteStream result;
	extractionMethod->CallDown(seekIn->AsWave(), result);
	chemical::Substance* extract = ChemicalCast<chemical::Substance*>(Cast<physical::Wave*>(result.IKnowWhatImDoing())); //This is about as safe as we can get right now.
	BIO_SANITIZE(extract,,return NULL);

	if (donePtr)
	{
		return Seek(locale->m_next, extract, donePtr);
	}
	return extract;
}

} //genetic namespace
} //bio namespace
