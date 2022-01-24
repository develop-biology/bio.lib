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

#include "bio/common/Macros.h"

/**
 * For ease of use for defining Singleton Perspectives.
 */
#define BIO_PERSPECTIVE_SINGLETON(className, dimension)                            \
BIO_SINGLETON(className, bio::physical::Perspective<dimension>)

/**
 * Ease of use for defining Ids.
 * For more on DIMENSIONs, Ids, etc., see Perspective.h and Identifiable.h
 */
#define BIO_ID(className, pluralName, dimension)                                   \
typedef dimension className;                                                       \
typedef std::vector<className> pluralName;

/**
 * Define a BIO_ID with "classNames" as plural (e.g. StandardDimensions)
 */
#define BIO_ID(className, dimension)                                               \
BIO_ID(className, className##s, dimension)

/**
 * To make defining ids easier, use this macro to define the function body of your Id Function().
 * This will assign a value to a string that is identical to your FunctionName e.g. MyPerspective::Instance().GetNameFromId(Value()) would give "Value".
 * Necessitates that functionName be a part of any namespaces are already specified (e.g. using namespace somewhere above a call to this macro).
 */
#define BIO_ID_FUNCTION_BODY(functionName, perspective, dimension)                 \
dimension g_##functionName = (perspective).GetIdFromName(#functionName);           \
dimension functionName()                                                           \
{                                                                              \
    return g_##functionName;                                                   \
}

/**
 * This is the preferred design pattern if using singletons and a custom dimension
 */
#define BIO_ID_WITH_PERSPECTIVE(className, dimension)                              \
BIO_ID(className, dimension)                                                       \
BIO_PERSPECTIVE_SINGLETON(className##Perspective, className);

/**
 * For when the plural of className isn't "classNames" (e.g. Properties or Axes)
 */
#define BIO_ID_WITH_PERSPECTIVE(className, pluralName, dimension)                  \
BIO_ID(className, pluralName, dimension)                                           \
BIO_PERSPECTIVE_SINGLETON(className##Perspective, className);

/**
 * To make defining return codes easier, use this macro to define the function body of your Code Function().
 * This will assign a value to a string that is identical to your FunctionName e.g. CodePerspective::Instance().GetNameFromId(Success()) would give "Success"
 * REMINDER: Your Code Function()s should be in the bio::code namespace.
*/
#define BIO_CODE_FUNCTION_BODY(functionName)                                   \
BIO_ID_FUNCTION_BODY(                                                          \
    functionName,                                                              \
    bio::code::CodePerspective::Instance(),                                    \
    bio::physical::Code)

/**
 * To make defining States easier, use this macro to define the function body of your State Function().
 * This will assign a value to a string that is identical to your FunctionName e.g. StatePerspective::Instance().GetNameFromId(Value()) would give "Value".
 * REMINDER: Your State Function()s should be in the bio::state namespace.
 */
#define BIO_STATE_FUNCTION_BODY(functionName)                                  \
BIO_ID_FUNCTION_BODY(                                                          \
    functionName,                                                              \
    bio::StatePerspective::Instance(),                                         \
    bio::State)


/**
 * To make defining Properties easier, use this macro to define the function body of your Property Function().
 * This will assign a value to a string that is identical to your FunctionName e.g. PropertyPerspective::Instance().GetNameFromId(Value()) would give "Value".
 * REMINDER: Your Property Function()s should be in the bio::property namespace.
 */
#define BIO_PROPERTY_FUNCTION_BODY(functionName)                               \
BIO_ID_FUNCTION_BODY(                                                          \
    functionName,                                                              \
    bio::PropertyPerspective::Instance(),                                      \
    bio::Property)

/**
 * To make defining SymmetryTypes easier, use this macro to define the function body of your SymmetryType Function().
 * This will assign a value to a string that is identical to your FunctionName e.g. SymmetryTypePerspective::Instance().GetNameFromId(Value()) would give "Value".
 * REMINDER: Your SymmetryType Function()s should be in the bio::symmetry_type namespace.
 */
#define BIO_SYMMETRY_TYPE_FUNCTION_BODY(functionName)                          \
BIO_ID_FUNCTION_BODY(                                                          \
    functionName,                                                              \
    bio::SymmetryTypePerspective::Instance(),                                  \
    bio::SymmetryType)

/**
 * To make defining Filters easier, use this macro to define the function body of your Filter Function().
 * This will assign a value to a string that is identical to your FunctionName e.g. FilterPerspective::Instance().GetNameFromId(Value()) would give "Value".
 * REMINDER: Your Filter Function()s should be in the bio::filter namespace.
 */
#define BIO_FILTER_FUNCTION_BODY(functionName)                                 \
BIO_ID_FUNCTION_BODY(                                                          \
    functionName,                                                              \
    bio::FilterPerspective::Instance(),                                        \
    bio::Filter)
