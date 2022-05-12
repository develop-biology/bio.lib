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

#include "bio/chemical/PeriodicTable.h"
#include "bio/chemical/Substance.h"

namespace bio {
namespace chemical {

/**
 * Elements track the Properties of items in the PeriodicTable. 
 * This is a Substance for ease of use but can be changed later. 
 * We also want to use Element as a Substance here as we can store the Properties of classes which are not Substances (e.g. Symmetry) 
 * NOTE, this is not chemical::Elementary<T>, which is used to define these Elements. 
 */
class Element :
	public Substance
{
public:
	Element()
		:
		m_type(NULL)
	{
	}

	virtual ~Element()
	{
	}

	/**
	 * Optional type association. 
	 * This can be used for casting, etc. 
	 */
	physical::Wave* m_type; 
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
	Element* element = ForceCast< Element* >(Perspective::GetTypeFromId(id)); 
	BIO_SANITIZE(element, ,
		return ret);
	LockThread();
	ret = element->GetAllAsVector< Property >();
	UnlockThread();
	return ret;
}

const Properties PeriodicTableImplementation::GetPropertiesOf(Name name) const
{
	return GetPropertiesOf(GetIdWithoutCreation(name));
}

AtomicNumber PeriodicTableImplementation::RecordPropertyOf(
	AtomicNumber id,
	Property property
)
{
	Properties properties;
	properties.Add(property);
	return RecordPropertiesOf(
		id,
		properties
	);
}

AtomicNumber PeriodicTableImplementation::RecordPropertyOf(
	Name name,
	Property property
)
{
	return RecordPropertyOf(
		GetIdFromName(name),
		property
	);
}

AtomicNumber PeriodicTableImplementation::RecordPropertiesOf(
	AtomicNumber id,
	Properties properties
)
{
	SmartIterator hdt = Find(id);
	BIO_SANITIZE_AT_SAFETY_LEVEL_2(hdt == this->m_hadits.end(), ,
		return InvalidId());

	LockThread();
	Element* element = ForceCast< Element* >(hdt->m_type); 
	if (!element)
	{
		element = new Element();
		hdt->m_type = element->AsWave();
	}
	element->Import< Property >(properties);
	UnlockThread();
	return id;
}

AtomicNumber PeriodicTableImplementation::RecordPropertiesOf(
	Name name,
	Properties properties
)
{
	return RecordPropertiesOf(
		GetIdFromName(name),
		properties
	);
}

const physical::Wave* PeriodicTableImplementation::GetTypeFromId(AtomicNumber id) const 
{
	Element* element = ForceCast< Element* >(Perspective::GetTypeFromId(id)); 
	BIO_SANITIZE(element, ,
		return NULL);
	return element->m_type;
}

bool PeriodicTableImplementation::AssociateType(
	AtomicNumber id,
	physical::Wave* type 
)
{
	Element* element = ForceCast< Element* >(Perspective::GetTypeFromId(id)); 
	BIO_SANITIZE(element, ,
		return false);
	LockThread();
	element->m_type = type;
	UnlockThread();
	return true;
}

bool PeriodicTableImplementation::DisassociateType(AtomicNumber id)
{
	Element* element = ForceCast< Element* >(Perspective::GetTypeFromId(id)); 
	BIO_SANITIZE(element, ,
		return false);
	LockThread();
	element->m_type = NULL;
	UnlockThread();
	return true;
}

} //chemical namespace
} //bio namespace
