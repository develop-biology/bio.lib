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

#include "bio/chemical/Substance.h"

namespace bio {
namespace chemical {

Substance::Substance()
	:
	chemical::Class<Substance>(this)
{
	CtorCommon();
}

Substance::Substance(
	Name name,
	physical::Perspective< StandardDimension >* perspective,
	Filter filter
)
	:
	chemical::Class<Substance>(
		this,
		filter
	)
{
	physical::Identifiable< StandardDimension >::Initialize(
		name,
		perspective
	);
	CtorCommon();
}

Substance::Substance(
	Id id,
	physical::Perspective< StandardDimension >* perspective,
	Filter filter
)
	:
	chemical::Class<Substance>(
		this,
		filter
	)
{
	physical::Identifiable< StandardDimension >::Initialize(
		id,
		perspective
	);
	CtorCommon();
}

Substance::Substance(
	const typename StructuralComponent< Property >::Contents& properties,
	const typename StructuralComponent< State >::Contents& states
)
	:
	chemical::Class<Substance>(this),
	PropertyStructure(properties),
	StateStructure(states)
{
	CtorCommon();
}

Substance::~Substance()
{
}

void Substance::Enable()
{
	Add< State >(state::Enabled());
}

void Substance::Disable()
{
	Remove< State >(state::Enabled());
}

bool Substance::IsEnabled() const
{
	return Has< State >(state::Enabled());
}

void Substance::CtorCommon()
{
	Enable();
}

bool Substance::ProbeFor(Property property)
{
	return Has< Property >(property);
}

bool Substance::ProbeFor(Properties properties)
{
	return HasAll< Property >(properties);
}

PropertyStructure::PropertyStructure()
{

}

PropertyStructure::PropertyStructure(const StructuralComponentImplementation< Property >::Contents& properties)
	:
	StructuralComponent<Property>(properties)
{

}

PropertyStructure::~PropertyStructure()
{

}

StateStructure::StateStructure()
{

}

StateStructure::StateStructure(const StructuralComponentImplementation< State >::Contents& states) :
	StructuralComponent< State >(states)
{

}

StateStructure::~StateStructure()
{

}
} //chemical namespace
} //bio namespace
