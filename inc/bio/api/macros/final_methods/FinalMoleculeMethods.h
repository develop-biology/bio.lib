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

#define BIO_FINAL_MOLECULE_METHODS                                             \
template < typename T >                                                        \
Id Use(                                                         \
	Name varName,                                                              \
	T* varPtr                                                                  \
)                                                                              \
{                                                                              \
	return this->mT.template Use< T >(                                         \
		varName,                                                               \
		varPtr                                                                 \
	);                                                                         \
}                                                                              \
                                                                               \
Id Define(Name varName)                                         \
{                                                                              \
	return this->mT.Define(varName);                                           \
}                                                                              \
                                                                               \
template < typename T >                                                        \
Id Define(Name varName)                                         \
{                                                                              \
	return this->mT.template Define< T >(varName);                             \
}                                                                              \
                                                                               \
template < typename T >                                                        \
Id Define(                                                      \
	Name varName,                                                              \
	const T& assignment                                                        \
)                                                                              \
{                                                                              \
	return this->mT.template Define< T >(                                      \
		varName,                                                               \
		assignment                                                             \
	);                                                                         \
}                                                                              \
                                                                               \
bio::molecular::Surface* RotateTo(Id surfaceId)                 \
{                                                                              \
	return this->mT.RotateTo(surfaceId);                                       \
}                                                                              \
                                                                               \
const bio::molecular::Surface* RotateTo(Id surfaceId) const     \
{                                                                              \
	return this->mT.RotateTo(surfaceId);                                       \
}                                                                              \
                                                                               \
template < typename T >                                                        \
T RotateTo(Id surfaceId) const                                  \
{                                                                              \
	return this->mT.template RotateTo< T >(surfaceId);                         \
}                                                                              \
                                                                               \
bio::molecular::Surface* RotateTo(Name surfaceName)                            \
{                                                                              \
	return this->mT.RotateTo(surfaceName);                                     \
}                                                                              \
                                                                               \
const bio::molecular::Surface* RotateTo(Name surfaceName) const                \
{                                                                              \
	return this->mT.RotateTo(surfaceName);                                     \
}                                                                              \
                                                                               \
template < typename T >                                                        \
T RotateTo(Name surfaceName) const                                             \
{                                                                              \
	return this->mT.template RotateTo< T >(surfaceName);                       \
}                                                                              \
                                                                               \
bool DuplicateFrom(                                                            \
	bio::molecular::Molecule* source,                                          \
	Name surface                                                               \
)                                                                              \
{                                                                              \
	return this->mT.DuplicateFrom(                                             \
		source,                                                                \
		surface                                                                \
	);                                                                         \
}                                                                              \
                                                                               \
bool TransferFrom(                                                             \
	bio::molecular::Molecule* source,                                          \
	Name surface                                                               \
)                                                                              \
{                                                                              \
	return this->mT.TransferFrom(                                              \
		source,                                                                \
		surface                                                                \
	);                                                                         \
}                                                                              \
                                                                               \
bio::molecular::Surface* operator()(Id surfaceId)               \
{                                                                              \
	return this->mT.operator()(surfaceId);                                     \
}                                                                              \
                                                                               \
const bio::molecular::Surface* operator()(Id surfaceId) const   \
{                                                                              \
	return this->mT.operator()(surfaceId);                                     \
}                                                                              \
                                                                               \
template < typename T >                                                        \
bio::molecular::Surface* operator()(Id surfaceId)               \
{                                                                              \
	return this->mT.operator()(surfaceId);                                     \
}                                                                              \
                                                                               \
bio::molecular::Surface* operator()(Name name)                                 \
{                                                                              \
	return this->mT.operator()(name);                                          \
}                                                                              \
                                                                               \
const bio::molecular::Surface* operator()(Name name) const                     \
{                                                                              \
	return this->mT.operator()(name);                                          \
}                                                                              \
                                                                               \
template < typename T >                                                        \
bio::molecular::Surface* operator()(Name surfaceName)                          \
{                                                                              \
	return this->mT.operator()(surfaceName);                                   \
}                                                                              \
                                                                               \
bio::molecular::Molecule* operator<<(bio::molecular::Surface* source)          \
{                                                                              \
	return this->mT.operator<<(source);                                        \
}                                                                              \
                                                                               \
bio::molecular::Surface* operator>>(bio::molecular::Surface* target)           \
{                                                                              \
	return this->mT.operator>>(target);                                        \
}                                                                              \
                                                                               \
bio::molecular::Molecule* operator<<(bio::molecular::Molecule* source)         \
{                                                                              \
	return this->mT.operator<<(source);                                        \
}                                                                              \
                                                                               \
bio::molecular::Molecule* operator>>(bio::molecular::Molecule* target)         \
{                                                                              \
	return this->mT.operator>>(target);                                        \
}
