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

#include "bio/physical/Filterable.h"
#include "bio/physical/common/Filters.h"
#include "bio/physical/macro/Macros.h"
#include "bio/physical/symmetric/Symmetry.h"
#include "bio/physical/common/SymmetryTypes.h"

namespace bio {
namespace physical {

Filterable::Filterable()
	:
	Class(
		this,
		new Symmetry(
			"mFilter",
			symmetry_type::DefineVariable())),
	mFilter(filter::Default())
{
}

Filterable::Filterable(Filter filter)
	:
	Class(
		this,
		new Symmetry(
			"mFilter",
			symmetry_type::DefineVariable())),
	mFilter(filter)
{

}

Filterable::~Filterable()
{
}

void Filterable::SetFilter(Filter filter)
{
	mFilter = filter;
}

Filter Filterable::GetFilter() const
{
	return mFilter;
}

Symmetry* Filterable::Spin() const
{
	mSymmetry->AccessValue()->Set(mFilter);
	return Wave::Spin();
}

Code Filterable::Reify(Symmetry* symmetry)
{
	BIO_SANITIZE(symmetry, ,
		return code::BadArgument1());
	mFilter = symmetry->GetValue();
	return code::Success();
}

void Filterable::InitializeImplementation(ByteStreams& args)
{
	BIO_SANITIZE(args[args.GetEndIndex()].Is(mFilter), , return);
	SetFilter(args[args.GetEndIndex()]);
}

} //physical namespace
} //bio namespace
