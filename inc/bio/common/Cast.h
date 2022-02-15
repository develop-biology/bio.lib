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

#pragma once

/**
This file includes all global functions for the bio namespace.
*/

namespace bio {

/**
 * Simple casting wrapper.
 * It continues to function when RTTI is disabled.
 * @tparam TO
 * @tparam FROM Dynamically determined template type.
 * @param toCast a FROM.
 * @return toCast as a TO
 */
template <typename TO, typename FROM>
TO Cast(FROM toCast)
{
	#ifdef RTTI_ENABLED
	return dynamic_cast<TO>(toCast);
	#else
	return (TO)(toCast);
	#endif
}

/**
 * DANGEROUS!!!
 *
 * DO NOT USE THIS UNLESS YOU KNOW WHAT YOU ARE DOING!
 *
 * THIS DOES NO ERROR CHECKING AND WILL BREAK YOUR CODE IF MISUSED!
 *
 * Example use case:
 * void* toCast = new MyClass(); //void* might be an ambiguous base class but MyClass is the same for toCast and casted.
 * MyClass* casted = ForceCast<MyClass*>(toCast);
 * @tparam TO
 * @tparam FROM Dynamically determined template type.
 * @param toCast a FROM.
 * @return toCast as a TO, regardless of what it was before.
 */
template <typename TO, typename FROM>
TO ForceCast(FROM toCast)
{
	return (TO)(void*)(toCast);
}

/**
 * DANGEROUS!!!
 * Same as ForceCast, except uses a dummy parameter to automatically determine an intermediate type.
 * @tparam TO Dynamically determined by autoTo dummy parameter.
 * @tparam FROM Dynamically determined template type.
 * @param toCast a FROM.
 * @param, autoTo dummy.
 * @return toCast as a TO, regardless of what it was before.
 */
template <typename TO_FINAL, typename TO_1, typename FROM>
TO_FINAL DoubleForceCast(FROM toCast, TO_1* autoTo)
{
	return (TO_FINAL)(TO_1)(void*)(toCast);
}

} //bio namespace
