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
 * Elements track the Properties of items in the PeriodicTable. <br />
 * Elements should be Substances; however, creating a Substance (or indeed any Atom with Bonds) requires locking the PeriodicTable. Thus, when the PeriodicTable creates new Elements, it locks itself and halts. <br />
 * Instead of making Elements full Substances, we just steal UnorderedMotif< Property > so that we can store the Properties of classes, including those which are not Substances (e.g. Symmetry). <br />
 * NOTE: this is not chemical::Elementary<T>, which is used to define these Elements. <br />
 */
class Element :
	public physical::Class< Element >,
	public Arrangement< Property >
{
public:

	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(physical, Element)

	Element()
		:
		physical::Class< Element >(this),
		mType(NULL)
	{
	}

	Element(Properties* properties)
		:
		physical::Class< Element >(this),
		Arrangement< Property >(properties),
		mType(NULL)
	{
	}

	virtual ~Element()
	{
	}

	/**
	 * Optional type association. 
	 * This can be used for casting, etc. 
	 */
	physical::Wave* mType;
};

PeriodicTableImplementation::PeriodicTableImplementation()
{

}

PeriodicTableImplementation::~PeriodicTableImplementation()
{

}

const Properties PeriodicTableImplementation::GetPropertiesOf(AtomicNumber id) const
{
	Properties* ret;
	Element* element = ForceCast< Element* >(Perspective::GetTypeFromId(id));
	BIO_SANITIZE(element,
		,
		return ret
	)
	ret = Cast< Properties* >(element);
	return *ret;
}

const Properties PeriodicTableImplementation::GetPropertiesOf(const Name& name) const
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
	const Name& name,
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
	SmartIterator brn = Find(id);
	BIO_SANITIZE_AT_SAFETY_LEVEL_1(brn.IsValid(), ,
		return InvalidId());

	Brane* brane = brn;
	Element* element = ForceCast< Element* >(brane->mType);
	if (!element)
	{
		element = new Element(&properties);
		brane->mType = element->AsWave();
	}
	return id;
}

AtomicNumber PeriodicTableImplementation::RecordPropertiesOf(
	const Name& name,
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
	BIO_SANITIZE(element,
		,
		return NULL
	)
	return element->mType;
}

bool PeriodicTableImplementation::AssociateType(
	AtomicNumber id,
	physical::Wave* type
)
{
	Element* element = ForceCast< Element* >(Perspective::GetTypeFromId(id));
	BIO_SANITIZE(element,
		,
		return false
	)
	element->mType = type;
	return true;
}

bool PeriodicTableImplementation::DisassociateType(AtomicNumber id)
{
	Element* element = ForceCast< Element* >(Perspective::GetTypeFromId(id));
	BIO_SANITIZE(element,
		,
		return false
	)
	element->mType = NULL;
	return true;
}

} //chemical namespace
} //bio namespace
