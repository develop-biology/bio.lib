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

#include "ClassMethodsLoopMacros.h"

/**
 * Define a virtual function that forwards to a Class.
 * Currently only works for functions that have a non-void return type.
 */
#define BIO_CLASS_METHOD(\
    ns, \
    caller, \
    functionSignature, \
    functionCall)                                                              \
functionSignature                                                              \
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
#define BIO_CALL_CLASS_METHOD_LOOP_WITH_MACRO(\
        loopName, \
        iterationsMacro, \
        ns, \
        caller, \
        argsMacro                                                              \
)                                                                          \
    BIO_CALL_LOOP(                                                             \
        loopName,                                                              \
        iterationsMacro,                                                       \
        ns,                                                                    \
        caller,                                                                \
        argsMacro)

/**
 * Calls the appropriate loop to define up to 99 class methods from BIO_GET_REQUIRED_CLASS_METHODS_FOR_...
 * NOTE: We call BIO_GET_REQUIRED_CLASS_METHODS_FOR_##ns() with this. ns is lowercase. This deviates from the Biology macro naming scheme but improves usability, since we don't need to provide ns in lowercase and in uppercase.
 * @param ns the namespace of the class.
 * @param caller the name of the class.
 */
#define BIO_DISAMBIGUATE_REQUIRED_CLASS_METHODS(ns, caller)                    \
    BIO_CALL_CLASS_METHOD_LOOP_WITH_MACRO(                                     \
        BIO_DEFINE_CLASS_METHODS_LOOP,                                         \
        BIO_GET_NUM_ARGS_OF_MACRO(                                             \
            BIO_GET_REQUIRED_CLASS_METHODS_FOR_##ns()                          \
        ),                                                                     \
        ns,                                                                    \
        (caller),                                                              \
        BIO_CALL_NS_MACRO(                                                     \
            BIO_GET_REQUIRED_CLASS_METHODS_FOR,                                \
            ns                                                                 \
        )                                                                      \
    )

/**
 * Calls the appropriate loop to define up to 99 class methods from BIO_GET_CLASS_OPTIONAL_METHODS_FOR_...
 * NOTE: We call BIO_GET_OPTIONAL_CLASS_METHODS_FOR_##ns() with this. ns is lowercase. This deviates from the Biology macro naming scheme but improves usability, since we don't need to provide ns in lowercase and in uppercase.
 * @param ns the namespace of the class.
 * @param caller the name of the class.
 */
#define BIO_DISAMBIGUATE_OPTIONAL_CLASS_METHODS(ns, caller)                    \
    BIO_CALL_CLASS_METHOD_LOOP_WITH_MACRO(                                     \
        BIO_DEFINE_CLASS_METHODS_LOOP,                                         \
        BIO_GET_NUM_ARGS_OF_MACRO(                                             \
            BIO_GET_OPTIONAL_CLASS_METHODS_FOR_##ns()                          \
        ),                                                                     \
        ns,                                                                    \
        (caller),                                                              \
        BIO_CALL_NS_MACRO(                                                     \
            BIO_GET_OPTIONAL_CLASS_METHODS_FOR,                                \
            ns                                                                 \
        )                                                                      \
    )

/**
 * Ease of use wrapper for defining REQUIRED and OPTIONAL CLASS_METHODS.
 * @param ns the namespace of the class.
 * @param caller the name of the class.
 */
#define BIO_DISAMBIGUATE_ALL_CLASS_METHODS(ns, caller)                         \
    BIO_DISAMBIGUATE_REQUIRED_CLASS_METHODS(ns, BIO_SINGLE_ARG(caller))        \
    BIO_DISAMBIGUATE_OPTIONAL_CLASS_METHODS(ns, BIO_SINGLE_ARG(caller))
