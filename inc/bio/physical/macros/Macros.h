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


#include "bio/common/Macros.h"
#include "ClassMethodsLoop.h"
#include "bio/physical/Perspective.h"

/**
 * For ease of use for defining Singleton Perspectives.
 */
#define BIO_PERSPECTIVE_SINGLETON(className, dimension)                        \
BIO_SINGLETON(className, ::bio::physical::Perspective<dimension>)

/**
 * Ease of use for defining Ids.
 * For more on DIMENSIONs, Ids, etc., see Perspective.h and Identifiable.h
 */
#define BIO_ID_WITH_PLURAL(className, pluralName, dimension)                   \
typedef dimension className;                                                   \
typedef std::vector<className> pluralName;

/**
 * Define a BIO_ID with "classNames" as plural (e.g. StandardDimensions)
 */
#define BIO_ID(className, dimension)                                           \
BIO_ID_WITH_PLURAL(className, className##s, dimension)

/**
 * To make defining ids easier, use this macro to define the function body of your Id Function().
 * This will assign a value to a string that is identical to your FunctionName e.g. MyPerspective::Instance().GetNameFromId(Value()) would give "Value".
 * Necessitates that functionName be a part of any namespaces are already specified (e.g. using namespace somewhere above a call to this macro).
 */
#define BIO_ID_FUNCTION_BODY(functionName, perspective, dimension)             \
dimension g_##functionName = (perspective).GetIdFromName(#functionName);       \
dimension functionName()                                                       \
{                                                                              \
    return g_##functionName;                                                   \
}

/**
 * This is the preferred design pattern if using singletons and a custom dimension
 */
#define BIO_ID_WITH_PERSPECTIVE(className, dimension)                          \
BIO_ID(className, dimension)                                                   \
BIO_PERSPECTIVE_SINGLETON(className##Perspective, className);

/**
 * For when the plural of className isn't "classNames" (e.g. Properties or Axes)
 */
#define BIO_ID_WITH_PERSPECTIVE_WITH_PLURAL(className, pluralName, dimension)  \
BIO_ID_WITH_PLURAL(className, pluralName, dimension)                           \
BIO_PERSPECTIVE_SINGLETON(className##Perspective, className);

/**
 * To make defining return codes easier, use this macro to define the function body of your Code Function().
 * This will assign a value to a string that is identical to your FunctionName e.g. CodePerspective::Instance().GetNameFromId(Success()) would give "Success"
 * REMINDER: Your Code Function()s should be in the ::bio::code namespace.
*/
#define BIO_CODE_FUNCTION_BODY(functionName)                                   \
BIO_ID_FUNCTION_BODY(                                                          \
    functionName,                                                              \
    ::bio::CodePerspective::Instance(),                                          \
    ::bio::Code)

/**
 * To make defining States easier, use this macro to define the function body of your State Function().
 * This will assign a value to a string that is identical to your FunctionName e.g. StatePerspective::Instance().GetNameFromId(Value()) would give "Value".
 * REMINDER: Your State Function()s should be in the ::bio::state namespace.
 */
#define BIO_STATE_FUNCTION_BODY(functionName)                                  \
BIO_ID_FUNCTION_BODY(                                                          \
    functionName,                                                              \
    ::bio::StatePerspective::Instance(),                                         \
    ::bio::State)


/**
 * To make defining Properties easier, use this macro to define the function body of your Property Function().
 * This will assign a value to a string that is identical to your FunctionName e.g. PropertyPerspective::Instance().GetNameFromId(Value()) would give "Value".
 * REMINDER: Your Property Function()s should be in the ::bio::property namespace.
 */
#define BIO_PROPERTY_FUNCTION_BODY(functionName)                               \
BIO_ID_FUNCTION_BODY(                                                          \
    functionName,                                                              \
    ::bio::PropertyPerspective::Instance(),                                      \
    ::bio::Property)

/**
 * To make defining SymmetryTypes easier, use this macro to define the function body of your SymmetryType Function().
 * This will assign a value to a string that is identical to your FunctionName e.g. SymmetryTypePerspective::Instance().GetNameFromId(Value()) would give "Value".
 * REMINDER: Your SymmetryType Function()s should be in the ::bio::symmetry_type namespace.
 */
#define BIO_SYMMETRY_TYPE_FUNCTION_BODY(functionName)                          \
BIO_ID_FUNCTION_BODY(                                                          \
    functionName,                                                              \
    ::bio::SymmetryTypePerspective::Instance(),                                  \
    ::bio::SymmetryType)

/**
 * To make defining Filters easier, use this macro to define the function body of your Filter Function().
 * This will assign a value to a string that is identical to your FunctionName e.g. FilterPerspective::Instance().GetNameFromId(Value()) would give "Value".
 * REMINDER: Your Filter Function()s should be in the ::bio::filter namespace.
 */
#define BIO_FILTER_FUNCTION_BODY(functionName)                                 \
BIO_ID_FUNCTION_BODY(                                                          \
    functionName,                                                              \
    ::bio::FilterPerspective::Instance(),                                        \
    ::bio::Filter)

/**
 * Define a virtual function that forwards to a Class.
 * Currently only works for functions that have a non-void return type.
 */
#define BIO_CLASS_METHOD(                                                      \
	ns,                                                                        \
	caller,                                                                    \
	functionSignature,                                                         \
    functionCall)                                                              \
virtual functionSignature                                                      \
{                                                                              \
    return this->ns::Class< caller >::functionCall;                            \
}

#define BIO_CLASS_METHOD_WITH_MACRO(ns, caller, macro)                         \
    BIO_CLASS_METHOD(ns, BIO_EXPAND_TUPLE caller, macro)


/**
 * Calls a loop with the appropriate number of args
 * @param loopName base name of loop to call (e.g. BIO_DEFINE_CLASS_METHODS_LOOP)
 * @param iterationsMacro macro that gives the number of iterations to loop over.
 * @param ns the namespace for the given class
 * @param caller a tuple containing the class
 * @param argsMacro the arguments to be looped over (1 per iteration).
 */
#define BIO_CALL_CLASS_METHOD_LOOP_WITH_MACRO(                                 \
        loopName,                                                              \
        iterationsMacro,                                                       \
        ns,                                                                    \
        caller,                                                                \
        argsMacro                                                              \
	)                                                                          \
    BIO_CALL_LOOP(                                                             \
		loopName,                                                              \
		iterationsMacro,                                                       \
		ns,                                                                    \
		caller,                                                                \
		argsMacro)

/**
 * Calls the appropriate loop to define up to 99 class methods from BIO_GET_CLASS_METHODS_FOR_...
 * NOTE: We call BIO_GET_CLASS_METHODS_FOR_##ns() with this. ns is lowercase. This deviates from the Biology macro naming scheme but improves usability, since we don't need to provide ns in lowercase and in uppercase.
 * @param ns the namespace of the class.
 * @param caller the name of the class.
 */
#define BIO_DISAMBIGUATE_CLASS_METHODS(ns, caller)                             \
    BIO_CALL_CLASS_METHOD_LOOP_WITH_MACRO(                                     \
        BIO_DEFINE_CLASS_METHODS_LOOP,                                         \
        BIO_GET_NUM_ARGS_OF_MACRO(                                             \
			BIO_GET_CLASS_METHODS_FOR_##ns()                                   \
        ),                                                                     \
        ns,                                                                    \
        (caller),                                                              \
        BIO_CALL_NS_MACRO(                                                     \
            BIO_GET_CLASS_METHODS_FOR,                                         \
            ns                                                                 \
        )                                                                      \
    )


/**
 * Get all virtual methods defined by physical::Class.
 * @return function signatures for use in BIO_DISAMBIGUATE_CLASS_METHODS
 */
#define BIO_GET_CLASS_METHODS_FOR_physical()                                   \
    (::bio::physical::Wave* Clone() const, Clone()),                             \
    (::bio::physical::Wave* AsWave(), AsWave()),                                 \
    (const ::bio::physical::Wave* AsWave() const, AsWave()),                     \
	(operator ::bio::physical::Wave*(), operator ::bio::physical::Wave*()),        \
	(::bio::physical::Wave* Modulate(Wave* signal), Modulate(signal)),           \
	(::bio::physical::Wave* Demodulate(), Demodulate()),                         \
	(const ::bio::physical::Wave* Demodulate() const, Demodulate())

