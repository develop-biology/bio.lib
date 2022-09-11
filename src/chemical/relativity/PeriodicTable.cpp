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

#include "bio/chemical/relativity/PeriodicTable.h"
#include "bio/chemical/Substance.h"

namespace bio {
namespace chemical {

PeriodicTableImplementation::PeriodicTableImplementation()
{

}

PeriodicTableImplementation::~PeriodicTableImplementation()
{

}

AtomicNumber PeriodicTableImplementation::GetIdFromName(const Name& name)
{
	if (name == InvalidName())
	{
		return InvalidId();
	}

	AtomicNumber ret = GetIdWithoutCreation(name);
	if (ret)
	{
		return ret;
	}

	ret = mNextId++;
	mBranes.Add(
		new Element(
			ret,
			name
		));

	return ret;
}

const Properties& PeriodicTableImplementation::GetPropertiesOf(AtomicNumber id) const
{
	Properties* ret;
	Element* element = GetBraneAs< Element* >(id);
	BIO_SANITIZE(element, , return ret)
	return element->mProperties;
}

const Properties& PeriodicTableImplementation::GetPropertiesOf(const Name& name) const
{
	return GetPropertiesOf(GetIdWithoutCreation(name));
}

AtomicNumber PeriodicTableImplementation::RecordPropertyOf(
	AtomicNumber id,
	const Property& property
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
	const Property& property
)
{
	return RecordPropertyOf(
		GetIdFromName(name),
		property
	);
}

AtomicNumber PeriodicTableImplementation::RecordPropertiesOf(
	AtomicNumber id,
	const Properties& properties
)
{
	SmartIterator brn = Find(id);
	if (!brn.IsValid())
	{
		return InvalidId();
	}
	
	Element* element = GetBraneAs< Element* >(id);
	if (!element)
	{
		return InvalidId();
	}
	element->mProperties->Import(properties);
	return id;
}

AtomicNumber PeriodicTableImplementation::RecordPropertiesOf(
	const Name& name,
	const Properties& properties
)
{
	return RecordPropertiesOf(
		GetIdFromName(name),
		properties
	);
}

} //chemical namespace
} //bio namespace
