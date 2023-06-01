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

#pragma once

#include "TypeName.h"
#include "IsPointer.h"
#include "IsReference.h"
#include "IsConst.h"
#include "RemovePointer.h"
#include "RemoveReference.h"
#include "RemoveConst.h"

namespace bio {
namespace type {

/**
 * Converts the T symbol to a string after remove all decorations (*, &, const, etc.) from it. <br />
 * @tparam T
 * @return Just T as a string.
 */
template < typename T >
BIO_CONSTEXPR static ImmutableString NakedTypeName()
{
	if BIO_CONSTEXPR(IsPointer< T >())
	{
		return NakedTypeName< typename RemovePointer< T >::Type >();
	}
	else if BIO_CONSTEXPR(IsReference< T >())
	{
		return NakedTypeName< typename RemoveReference< T >::Type >();
	}
	else if BIO_CONSTEXPR(IsConst< T >())
	{
		return NakedTypeName< typename RemoveConst< T >::Type >();
	}
	else
	{
		BIO_STATIC_ASSERT(!IsConst< T >());
		BIO_STATIC_ASSERT(!IsReference< T >());
		BIO_STATIC_ASSERT(!IsPointer< T >());
	}
	
	return TypeName< T >();
}

/**
 * Converts the T symbol to a string after remove all decorations (*, &, const, etc.) from it.. <br />
 * @tparam T
 * @param t used for auto type determination.
 * @return Just T as a string.
 */
template < typename T >
BIO_CONSTEXPR static ImmutableString NakedTypeName(const T t)
{
	return NakedTypeName< T >();
}

} //type namespace
} //bio namespace
