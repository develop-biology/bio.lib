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

namespace bio {

namespace physical {
class Wave;
}

/**
 * ChemicalCasts use chemical Bonds to change one object into anther. <br />
 * See Atom::As<>() for more info.<br />
 * @tparam TO a Wave* (not a ** or &).
 * @tparam FROM also a Wave*
 * @param toCast
 * @return The TO that is bonded to FROM or 0.
 */
template < typename TO, typename FROM >
TO ChemicalCast(FROM toCast)
{
	BIO_STATIC_ASSERT(type::IsPointer< FROM >())
	return toCast->AsAtom()->template As< TO >();
}

/**
 * Ease of use method for Cloning. <br />
 * @tparam T
 * @return a Clone of T casted to back to T.
 */
template < typename T >
T CloneAndCast(const T& toClone)
{
	//Dereference here might be dangerous & need sanitization.
	::bio::physical::Wave* clone = toClone->Clone()->AsWave();
	return ChemicalCast< T >(clone);
}


} //bio namespace
