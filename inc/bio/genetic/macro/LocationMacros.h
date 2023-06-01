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

/**
 * This is not for you. <br />
 */
#define BIO_TRANSLOCATION_FUNCTION(location, epitope, type, function, argTypeTuple, argTuple) \
    bool g##location##epitope##Registered =                                    \
        SafelyAccess< ::bio::genetic::Translocator >()                         \
			->AssociateSignalPeptidase(                                        \
            ::bio::genetic::Translocator::Instance().GetIdFromName(#location), \
            ::bio::EpitopePerspective::Instance().GetIdFromName(#epitope),     \
            (                                                                  \
                new BIO_EXCITATION_CLASS(                                      \
                    ::bio::chemical::LinearMotif< type >,                      \
                    type,                                                      \
                    BIO_EXPAND_TUPLE argTypeTuple                              \
                )(                                                             \
                    &::bio::chemical::LinearMotif< type >::function,           \
                    BIO_EXPAND_TUPLE argTuple                                  \
                )                                                              \
            )                                                                  \
        );

/**
 * Ease of use method of defining all kinds of sites at once. <br />
 * This will automatically define peptidases (chemical::Excitation*) for the following affinities at your Location: <br />
 * * "Move" <br />
 * * "Insert" <br />
 */
#define BIO_LOCATION_FUNCTION_BODY(functionName, type)                         \
    namespace location {                                                       \
        BIO_ID_FUNCTION_BODY(                                                  \
            functionName,                                                      \
            ::bio::genetic::Translocator::Instance(),                          \
            ::bio::Location)                                                   \
 		BIO_TRANSLOCATION_FUNCTION(                                            \
		    functionName,                                                      \
			Move,                                                              \
			type,                                                              \
			GetByNameImplementation,                                           \
			(const Name&),                                                     \
			(NULL))                                                            \
        BIO_TRANSLOCATION_FUNCTION(                                            \
		    functionName,                                                      \
			Insert,                                                            \
			type,                                                              \
			AddImplementation,                                                 \
			(type),                                                            \
			(NULL))                                                            \
    }
