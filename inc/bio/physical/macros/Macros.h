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


//@formatter:off

#include "IdAndPerspectiveMacros.h"
#include "ClassMethodMacros.h"
#include "bio/common/macros/Macros.h"
#include "bio/physical/Perspective.h"

/**
 * Get all virtual methods defined by physical::Class. <br />
 * @return function signatures for use in BIO_DISAMBIGUATE_REQUIRED_CLASS_METHODS
 */
#define BIO_GET_REQUIRED_CLASS_METHODS_FOR_physical()                          \
    (                                                                          \
		virtual ::bio::physical::Wave* Clone() const,                          \ <br />
		Clone()                                                                \
	),                                                                         \
    (                                                                          \
		virtual ::bio::physical::Wave* AsWave(),                               \ <br />
		AsWave()                                                               \
	),                                                                         \
    (                                                                          \
		virtual const ::bio::physical::Wave* AsWave() const,                   \ <br />
		AsWave()                                                               \
	),                                                                         \
	(                                                                          \
		virtual operator ::bio::physical::Wave*(),                             \
		operator ::bio::physical::Wave*()                                      \
	)

/**
 * Get all optional virtual methods defined by physical::Class. <br />
 * @return function signatures for use in BIO_DISAMBIGUATE_OPTIONAL_CLASS_METHODS
 */
#define BIO_GET_OPTIONAL_CLASS_METHODS_FOR_physical()                          \
	(                                                                          \
		virtual ::bio::physical::Wave* Modulate(::bio::physical::Wave* signal),\ <br />
		Modulate(signal)                                                       \
	),                                                                         \
	(                                                                          \
		virtual ::bio::physical::Wave* Demodulate(),                           \ <br />
		Demodulate()                                                           \
	),                                                                         \
	(                                                                          \
		virtual const ::bio::physical::Wave* Demodulate() const,               \ <br />
		Demodulate()                                                           \
	)


/**
 * To make defining return codes easier, use this macro to define the function body of your Code Function(). <br />
 * This will assign a value to a string that is identical to your FunctionName e.g. CodePerspective::Instance().GetNameFromId(Success()) would give "Success" <br />
 * REMINDER: Your Code Function()s should be in the ::bio::code namespace. <br />
*/
#define BIO_CODE_FUNCTION_BODY(functionName)                                   \
BIO_ID_FUNCTION_BODY(                                                          \
    functionName##Code,                                                        \
    ::bio::CodePerspective::Instance(),                                        \
    ::bio::Code)

/**
 * To make defining States easier, use this macro to define the function body of your State Function(). <br />
 * This will assign a value to a string that is identical to your FunctionName e.g. StatePerspective::Instance().GetNameFromId(Value()) would give "Value". <br />
 * REMINDER: Your State Function()s should be in the ::bio::state namespace. <br />
 */
#define BIO_STATE_FUNCTION_BODY(functionName)                                  \
BIO_ID_FUNCTION_BODY(                                                          \
    functionName##State,                                                       \
    ::bio::StatePerspective::Instance(),                                       \
    ::bio::State)


/**
 * To make defining Properties easier, use this macro to define the function body of your Property Function(). <br />
 * This will assign a value to a string that is identical to your FunctionName e.g. PropertyPerspective::Instance().GetNameFromId(Value()) would give "Value". <br />
 * REMINDER: Your Property Function()s should be in the ::bio::property namespace. <br />
 */
#define BIO_PROPERTY_FUNCTION_BODY(functionName)                               \
BIO_ID_FUNCTION_BODY(                                                          \
    functionName##Property,                                                    \
    ::bio::PropertyPerspective::Instance(),                                    \
    ::bio::Property)

/**
 * To make defining SymmetryTypes easier, use this macro to define the function body of your SymmetryType Function(). <br />
 * This will assign a value to a string that is identical to your FunctionName e.g. SymmetryTypePerspective::Instance().GetNameFromId(Value()) would give "Value". <br />
 * REMINDER: Your SymmetryType Function()s should be in the ::bio::symmetry_type namespace. <br />
 */
#define BIO_SYMMETRY_TYPE_FUNCTION_BODY(functionName)                          \
BIO_ID_FUNCTION_BODY(                                                          \
    functionName##SymmetryType,                                                \
    ::bio::SymmetryTypePerspective::Instance(),                                \
    ::bio::SymmetryType)

/**
 * To make defining Filters easier, use this macro to define the function body of your Filter Function(). <br />
 * This will assign a value to a string that is identical to your FunctionName e.g. FilterPerspective::Instance().GetNameFromId(Value()) would give "Value". <br />
 * REMINDER: Your Filter Function()s should be in the ::bio::filter namespace. <br />
 */
#define BIO_FILTER_FUNCTION_BODY(functionName)                                 \
BIO_ID_FUNCTION_BODY(                                                          \
    functionName##Filter,                                                      \
    ::bio::FilterPerspective::Instance(),                                      \
    ::bio::Filter)

