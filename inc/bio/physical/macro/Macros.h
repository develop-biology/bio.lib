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
#include "bio/common/macro/Macros.h"

/**
 * Get all virtual methods defined by physical::Class. <br />
 * @return function signatures for use in BIO_DISAMBIGUATE_REQUIRED_CLASS_METHODS
 */
#define BIO_GET_REQUIRED_CLASS_METHODS_FOR_physical()                          \
    (                                                                          \
		virtual ::bio::physical::Wave* Clone() const,                          \
		Clone()                                                                \
	),                                                                         \
    (                                                                          \
		::bio::physical::Wave* AsWave(),                                       \
		AsWave()                                                               \
	),                                                                         \
    (                                                                          \
		const ::bio::physical::Wave* AsWave() const,                           \
		AsWave()                                                               \
	),                                                                         \
	(                                                                          \
		operator ::bio::physical::Wave*(),                                     \
		operator ::bio::physical::Wave*()                                      \
	)

/**
 * Get all optional virtual methods defined by physical::Class. <br />
 * @return function signatures for use in BIO_DISAMBIGUATE_OPTIONAL_CLASS_METHODS
 */
#define BIO_GET_OPTIONAL_CLASS_METHODS_FOR_physical()                          \
	(                                                                          \
		virtual ::bio::physical::Wave* Modulate(::bio::physical::Wave* signal),\
		Modulate(signal)                                                       \
	),                                                                         \
	(                                                                          \
		virtual ::bio::physical::Wave* Demodulate(),                           \
		Demodulate()                                                           \
	),                                                                         \
	(                                                                          \
		virtual const ::bio::physical::Wave* Demodulate() const,               \
		Demodulate()                                                           \
	)


/**
 * To make defining return codes easier, use this macro to define the function body of your Code Function(). <br />
 * This will assign a value to a string that is identical to your FunctionName e.g. SafelyAccess<CodePerspective>()->GetNameFromId(Success()) would give "Success" <br />
 * REMINDER: Your Code Function()s should be in the ::bio::code namespace. <br />
*/
#define BIO_CODE_FUNCTION_BODY(functionName)                                   \
BIO_ID_FUNCTION_BODY(                                                          \
    functionName,                                                              \
    ::bio::CodePerspective::Instance(),                                        \
    ::bio::Code)

/**
 * To make defining States easier, use this macro to define the function body of your State Function(). <br />
 * This will assign a value to a string that is identical to your FunctionName e.g. SafelyAccess<StatePerspective>()->GetNameFromId(Value()) would give "Value". <br />
 * REMINDER: Your State Function()s should be in the ::bio::state namespace. <br />
 */
#define BIO_STATE_FUNCTION_BODY(functionName)                                  \
BIO_ID_FUNCTION_BODY(                                                          \
    functionName,                                                              \
    ::bio::StatePerspective::Instance(),                                       \
    ::bio::State)


/**
 * To make defining Properties easier, use this macro to define the function body of your Property Function(). <br />
 * This will assign a value to a string that is identical to your FunctionName e.g. SafelyAccess<PropertyPerspective>()->GetNameFromId(Value()) would give "Value". <br />
 * REMINDER: Your Property Function()s should be in the ::bio::property namespace. <br />
 */
#define BIO_PROPERTY_FUNCTION_BODY(functionName)                               \
BIO_ID_FUNCTION_BODY(                                                          \
    functionName,                                                              \
    ::bio::PropertyPerspective::Instance(),                                    \
    ::bio::Property)

/**
 * To make defining SymmetryTypes easier, use this macro to define the function body of your SymmetryType Function(). <br />
 * This will assign a value to a string that is identical to your FunctionName e.g. SafelyAccess<SymmetryTypePerspective>()->GetNameFromId(Value()) would give "Value". <br />
 * REMINDER: Your SymmetryType Function()s should be in the ::bio::symmetry_type namespace. <br />
 */
#define BIO_SYMMETRY_TYPE_FUNCTION_BODY(functionName)                          \
BIO_ID_FUNCTION_BODY(                                                          \
    functionName,                                                              \
    ::bio::SymmetryTypePerspective::Instance(),                                \
    ::bio::SymmetryType)

/**
 * To make defining Filters easier, use this macro to define the function body of your Filter Function(). <br />
 * This will assign a value to a string that is identical to your FunctionName e.g. SafelyAccess<FilterPerspective>()->GetNameFromId(Value()) would give "Value". <br />
 * REMINDER: Your Filter Function()s should be in the ::bio::filter namespace. <br />
 */
#define BIO_FILTER_FUNCTION_BODY(functionName)                                 \
BIO_ID_FUNCTION_BODY(                                                          \
    functionName,                                                              \
    ::bio::FilterPerspective::Instance(),                                      \
    ::bio::Filter)

/**
 * To make defining Interferences easier, use this macro to define the function body of your Interference Function(). <br />
 * This will assign a value to a string that is identical to your FunctionName e.g. SafelyAccess<InterferencePerspective>()->GetNameFromId(Value()) would give "Value". <br />
 * REMINDER: Your Interference Function()s should be in the ::bio::interference namespace. <br />
 */
#define BIO_SUPERPOSITION_FUNCTION_BODY(functionName)                          \
BIO_ID_FUNCTION_BODY(                                                          \
    functionName,                                                              \
    ::bio::SuperpositionPerspective::Instance(),                               \
    ::bio::Superposition)
