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
#include "bio/genetic/macro/Macros.h"
#include "bio/chemical/Substance.h"
#include "bio/common/ByteStream.h"

namespace bio {
namespace genetic {

Localization::Localization(
	Location location,
	const Name& name
)
	:
	physical::Class< Localization >(this),
	mcMethod(NULL)
{
	SetNameOfLocation(name);
	SetLocation(location);
}

Localization::~Localization()
{
	if (mcMethod)
	{
		delete mcMethod;
		mcMethod = NULL;
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

	BIO_SANITIZE(seekIn, , return seekIn);

	if (mLocation == Translocator::InvalidId())
	{
		return seekIn;
	}

	BIO_SANITIZE(mcMethod, , return NULL)
	ByteStream newName(mName);
	(const_cast< chemical::ExcitationBase* >(mcMethod))->EditArg(
		0,
		newName
	);
	ByteStream result;
	mcMethod->CallDown(
		seekIn->AsWave(),
		result
	);
	chemical::Substance* extract = ChemicalCast< chemical::Substance* >(Cast< physical::Wave* >(result.DirectAccess())); //This is about as safe as we can get right now. 
	BIO_SANITIZE(extract, , return NULL)
	return extract;
}

void Localization::SetNameOfLocation(const Name& name)
{
	mName = name;
}

Name Localization::GetNameOfLocation() const
{
	return mName;
}

void Localization::SetLocation(Location location)
{
	mLocation = location;
	if (mcMethod)
	{
		delete mcMethod;
		mcMethod = NULL;
	}
	mcMethod = SafelyAccess< Translocator >()->GetPeptidase(mLocation, "Move");
}

Location Localization::GetLocation() const
{
	return mLocation;
}

} //genetic namespace
} //bio namespace
