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

#include "bio/chemical/PeriodicTable.h"
#include "bio/chemical/Substance.h"

namespace bio {
namespace chemical {

/**
 * Elements track the Properties of items in the PeriodicTable.
 * This is a Substance for ease of use but can be changed later.
 * NOTE, this is not chemical::Element<T>, which is used to define these Elements.
 */
class Element :
	public Substance
{
public:
	Element()
	{
	}

	virtual ~Element()
	{
	}
};

PeriodicTableImplementation::PeriodicTableImplementation()
{

}

PeriodicTableImplementation::~PeriodicTableImplementation()
{

}

const Properties PeriodicTableImplementation::GetPropertiesOf(AtomicNumber id) const
{
	Properties ret;
	Element* element = Cast< Element* >(GetTypeFromId(id));
	BIO_SANITIZE(element, ,
		return ret);
	ret = &element->GetAll< Property >();
	return ret;
}

const Properties PeriodicTableImplementation::GetPropertiesOf(Name name) const
{
	return GetPropertiesOf(GetIdFromName(name));
}

void PeriodicTableImplementation::RecordPropertyOf(
	AtomicNumber id,
	Property property
)
{
	Properties properties;
	properties.push_back(property);
	RecordPropertiesOf(
		id,
		properties
	);
}

void PeriodicTableImplementation::RecordPropertyOf(
	Name name,
	Property property
)
{
	RecordPropertyOf(
		GetIdFromName(name),
		property
	);
}

void PeriodicTableImplementation::RecordPropertiesOf(
	AtomicNumber id,
	Properties properties
)
{
	typename Hadits::iterator hdt = Find(id);
	if (hdt == this->m_hadits.end())
	{
		return;
	}

	Element* element = Cast< Element* >(hdt->type);
	if (!element)
	{
		element = new Element();
		hdt->type = element; //just to be sure Cast didn't pull a fast one on us.
	}
	element->Import< Property >(properties);
}

void PeriodicTableImplementation::RecordPropertiesOf(
	Name name,
	Properties properties
)
{
	RecordPropertiesOf(
		GetIdFromName(name),
		properties
	);
}

} //chemical namespace
} //bio namespace
