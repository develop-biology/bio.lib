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

#include "bio/genetic/localization/Localization.h"
#include "bio/genetic/macros/Macros.h"
#include "bio/chemical/Substance.h"
#include "bio/common/ByteStream.h"

namespace bio {
namespace genetic {

Localization::Localization(
	Site site,
	Name name
)
	:
	physical::Class< Localization >(this),
	m_name(NULL),
	mc_method(NULL)
{
	SetNameOfSite(name);
	SetSite(site);
}

Localization::~Localization()
{
	if (m_name)
	{
		delete[] m_name;
		m_name = NULL;
	}
	if (mc_method)
	{
		delete mc_method;
		mc_method = NULL;
	}
}

chemical::Substance* Localization::ResolvePrevious(chemical::Substance* seekIn) const 
{
	BIO_SANITIZE(seekIn, ,
		return seekIn);

	//TODO: What if *this has been Modulated with something other than a Localization?
	Localization* previous = ForceCast< Localization* >(Demodulate()); 

	if (previous)
	{
		seekIn = previous->Seek(seekIn);
	}
	return seekIn;
}

chemical::Substance* Localization::Seek(chemical::Substance* seekIn) const 
{
	seekIn = ResolvePrevious(seekIn);

	BIO_SANITIZE(seekIn, ,
		return seekIn);

	if (m_site == LocalizationSitePerspective::InvalidId())
	{
		return seekIn;
	}

	BIO_SANITIZE(mc_method, ,
		return NULL)
	ByteStream newName(m_name);
	(const_cast< chemical::ExcitationBase* >(mc_method))->EditArg( 
		0,
		newName
	);
	ByteStream result;
	mc_method->CallDown(
		seekIn->AsWave(),
		result
	);
	chemical::Substance* extract = ChemicalCast< chemical::Substance* >(Cast< physical::Wave* >(result.IKnowWhatImDoing())); //This is about as safe as we can get right now. 
	BIO_SANITIZE(extract, ,
		return NULL)
	return extract;
}

void Localization::SetNameOfSite(Name name)
{
	if (m_name)
	{
		delete[] m_name;
	}
	string::CloneInto(
		name,
		m_name
	);
}

Name Localization::GetNameOfSite() const
{
	return m_name;
}

void Localization::SetSite(Site site)
{
	m_site = site;
	if (mc_method)
	{
		delete mc_method;
	}
	mc_method = LocalizationSitePerspective::Instance().GetNewObjectFromIdAs< chemical::ExcitationBase* >(m_site); 
}

Site Localization::GetSite() const
{
	return m_site;
}

} //genetic namespace
} //bio namespace
