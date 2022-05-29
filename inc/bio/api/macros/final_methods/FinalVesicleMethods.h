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

#define BIO_FINAL_VESICLE_METHODS                                              \
BIO_FINAL_MOLECULE_METHODS                                                     \
	bio::molecular::Molecule* operator[](StandardDimension moleculeId)         \
{                                                                              \
return this->mT.operator[](moleculeId);                                        \
}                                                                              \
                                                                               \
const bio::molecular::Molecule* operator[](StandardDimension moleculeId) const \
{                                                                              \
return this->mT.operator[](moleculeId);                                        \
}                                                                              \
                                                                               \
template < typename T >                                                        \
bio::molecular::Molecule* operator[](StandardDimension moleculeId)             \
{                                                                              \
return this->mT.template operator[]< T >(moleculeId);                          \
}                                                                              \
                                                                               \
bio::molecular::Molecule* operator[](Name moleculeName)                        \
{                                                                              \
	return this->mT.operator[](moleculeName);                                  \
}                                                                              \
                                                                               \
const bio::molecular::Molecule* operator[](Name moleculeName) const            \
{                                                                              \
	return this->mT.operator[](moleculeName);                                  \
}                                                                              \
                                                                               \
template < typename T >                                                        \
bio::molecular::Molecule* operator[](Name moleculeName)                        \
{                                                                              \
	return this->mT.template operator[]< T >(moleculeName);                    \
}                                                                              \
                                                                               \
bio::molecular::Vesicle* operator<<=(bio::molecular::Vesicle* source)          \
{                                                                              \
	return this->mT.operator<<=(source);                                       \
}                                                                              \
                                                                               \
bio::molecular::Vesicle* operator>>=(bio::molecular::Vesicle* target)          \
{                                                                              \
	return this->mT.operator>>=(target);                                       \
}
