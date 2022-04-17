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

#include "ConstructorMacros.h"
#include "ExcitationMacros.h"
#include "bio/chemical/common/BondTypes.h"
#include "bio/physical/macros/Macros.h"

/**
 * Get all virtual methods defined by chemical::Class. <br />
 * @return function signatures for use in BIO_DISAMBIGUATE_ALL_CLASS_METHODS
 */
#define BIO_GET_REQUIRED_CLASS_METHODS_FOR_chemical()                          \
    BIO_GET_REQUIRED_CLASS_METHODS_FOR_physical(),                             \
    (                                                                          \
        virtual ::bio::Properties GetProperties() const,                       \
        GetProperties()                                                        \
    ),                                                                         \
    (                                                                          \
        virtual ::bio::chemical::Atom* AsAtom(),                               \ <br />
        AsAtom()                                                               \
    ),                                                                         \
    (                                                                          \
        template< typename AS > operator AS(),                                 \
        template As< AS >()                                                    \
    ),                                                                         \
    (                                                                          \
        virtual const ::bio::chemical::Atom* AsAtom() const,                   \ <br />
        AsAtom()                                                               \
    )
/**
 * Get all virtual methods defined by chemical::Class. <br />
 * @return function signatures for use in BIO_DISAMBIGUATE_ALL_CLASS_METHODS
 */
#define BIO_GET_OPTIONAL_CLASS_METHODS_FOR_chemical()                          \
    BIO_GET_OPTIONAL_CLASS_METHODS_FOR_physical(),                             \
    (                                                                          \
        virtual Code Attenuate(const ::bio::physical::Wave* other),            \ <br />
        Attenuate(other)                                                       \
    ),                                                                         \
    (                                                                          \
        virtual Code Disattenuate(const ::bio::physical::Wave* other),         \ <br />
        Disattenuate(other)                                                    \
    )

/**
To make defining BondTypes easier, use this macro to define the function body of your BondType Function().
This will assign a value to a string that is identical to your FunctionName e.g. BondTypePerspective::Instance().GetNameFromId(Value()) would give "Value".
*/
#define BIO_CHEMICAL_BOND_TYPE_FUNCTION_BODY(functionName)                     \
BIO_ID_FUNCTION_BODY(                                                          \
    functionName##BondType,                                                    \
    ::bio::BondTypePerspective::Instance(),                                    \
    ::bio::BondType)
