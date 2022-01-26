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

#include "bio/physical/Filterable.h"
#include "bio/physical/Filters.h"
#include "bio/physical/Macros.h"
#include "bio/physical/Symmetry.h"
#include "bio/physical/SymmetryTypes.h"

namespace bio {
namespace physical {

Filterable::Filterable()
	:
	Class(
		this,
		new Symmetry(
			"m_filter",
			symmetry_type::DefineVariable())),
	m_filter(filter::Default())
{
}

Filterable::Filterable(Filter filter)
	:
	Class(
		this,
		new Symmetry(
			"m_filter",
			symmetry_type::DefineVariable())),
	m_filter(filter)
{

}

Filterable::~Filterable()
{
}

void Filterable::SetFilter(Filter filter)
{
	m_filter = filter;
}

Filter Filterable::GetFilter() const
{
	return m_filter;
}

Symmetry* Filterable::Spin() const
{
	m_symmetry->AccessValue()->Set(m_filter);
	return Wave::Spin();
}

void Filterable::Reify(Symmetry* symmetry)
{
	BIO_SANITIZE(symmetry, ,
		return);
	m_filter = symmetry->GetValue();
}

void Filterable::InitializeImplementation(ByteStreams args)
{
	BIO_SANITIZE(args.size() == 1 && args[0].Is(m_filter), ,
		return);
	m_filter = args[0];
}

} //physical namespace
} //bio namespace
