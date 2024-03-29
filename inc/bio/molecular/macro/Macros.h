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

#include "bio/chemical/macro/Macros.h"

/**
 * Get all virtual methods defined by molecular::Class. <br />
 * @return function signatures for use in BIO_DISAMBIGUATE_REQUIRED_CLASS_METHODS
 */
#define BIO_GET_REQUIRED_CLASS_METHODS_FOR_molecular()                         \
    BIO_GET_REQUIRED_CLASS_METHODS_FOR_chemical()

/**
 * Get all virtual methods defined by molecular::Class. <br />
 * @return function signatures for use in BIO_DISAMBIGUATE_OPTIONAL_CLASS_METHODS
 */
#define BIO_GET_OPTIONAL_CLASS_METHODS_FOR_molecular()                         \
    BIO_GET_OPTIONAL_CLASS_METHODS_FOR_chemical(),                             \
    (                                                                          \
        template< typename ENVIRONMENT >                                       \
		void SetEnvironment(ENVIRONMENT environment),                          \
        template SetEnvironment< ENVIRONMENT >(environment)                    \
    ),                                                                         \
    (                                                                          \
        template< typename ENVIRONMENT >                                       \
		ENVIRONMENT GetEnvironment(),                                          \
        template GetEnvironment< ENVIRONMENT >()                               \
    ),                                                                         \
    (                                                                          \
        template< typename ENVIRONMENT >                                       \
		const ENVIRONMENT GetEnvironment() const,                              \
        template GetEnvironment< ENVIRONMENT >()                               \
    )
