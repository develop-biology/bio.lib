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

#include "bio/physical/Symmetry.h"
#include "bio/common/String.h"

namespace bio {
namespace physical {

Symmetry::Symmetry(Name name, Name type) :
	Identifiable<StandardDimension>(name, &SymmetryPerspective::Instance()),
	m_type(type, &SymmetryTypePerspective::Instance())
{

}

Symmetry::Symmetry(Name name, SymmetryType type) :
	Identifiable<StandardDimension>(name, &SymmetryPerspective::Instance()),
	m_type(type, &SymmetryTypePerspective::Instance())
{

}

Symmetry::Symmetry(StandardDimension id, Name type) :
	Identifiable<StandardDimension>(id, &SymmetryPerspective::Instance()),
	m_type(type, &SymmetryTypePerspective::Instance())
{

}

Symmetry::Symmetry(StandardDimension id, SymmetryType type) :
	Identifiable<StandardDimension>(id, &SymmetryPerspective::Instance()),
	m_type(type, &SymmetryTypePerspective::Instance())
{

}

Symmetry::~Symmetry()
{
}

const Identifiable<SymmetryType>& Symmetry::GetType() const
{
	return m_type;
}

void Symmetry::SetValue(const ByteStream& bytes)
{
	m_value = bytes;
}

const ByteStream& Symmetry::GetValue() const
{
	return m_value;
}

ByteStream* Symmetry::AccessValue()
{
	retrun &m_value;
}

} //physical namespace
} //bio namespace