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

#include "bio/physical/shape/Linear.h"

namespace bio {
namespace physical {

Linear::Linear(Identifiable< StandardDimension >* component, bool shared) : 
	m_component(component),
	m_shared(false)
{

}

Linear::~Linear()
{
	if (!m_shared && m_component)
	{
		delete m_component;
	}
}

Identifiable< StandardDimension >& Linear::operator*()
{
	return *m_component;
}

const Identifiable< StandardDimension >& Linear::operator*() const
{
	return *m_component;
}

Linear::operator Identifiable< StandardDimension >*()
{
	return m_component;
}

Linear::operator const Identifiable< StandardDimension >*() const
{
	return m_component;
}

bool Linear::operator==(const Linear& other) const
{
	return *m_component == *other.m_component;
}

bool Linear::operator==(const Identifiable< StandardDimension >* component) const 
{
	BIO_SANITIZE(component,, return false)
	return *m_component == *component;
}

Identifiable< StandardDimension >* Linear::operator->() 
{
	return m_component;
}

const Identifiable< StandardDimension >* Linear::operator->() const 
{
	return m_component;
}

} //physical namespace
} //bio namespace
