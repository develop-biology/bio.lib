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

/**
 * This file contains all common preprocessor macros. <br />
 * Additional macros may be specified in files they more specifically pertain to <br />
 * All bio macros start with BIO_ <br />
*/
#pragma once

#include "AssertMacros.h"
#include "LanguageMacros.h"
#include "NumArgsMacros.h"
#include "OptimizeMacros.h"
#include "OSMacros.h"
#include "SanitizeMacros.h"
#include "StrongTypedef.h"
#include "bio/common/ByteStream.h"

/**
 * Wrapping macros is used to evaluate them within other macros, rather than being passed as a string. <br />
 */
#define BIO_WRAP_MACRO(macro) macro

/**
 * Used to remove fhe parentheses from around an expression. <br />
 * Should be invoked as: BIO_EXPAND_TUPLE tuple <br />
 * where tuple is (something, like, this) <br />
 * which expands to BIO_EXPAND_TUPLE (something, like, this) => something, like, this <br />
 */
#define BIO_EXPAND_TUPLE(...) __VA_ARGS__

/**
 * Encapsulates an expression containing commas, allowing it to be safely passed as a single macro argument. <br />
 * Should be invoked as: <br />
 * 		SOME_OTHER_MACRO( <br />
 * 			BIO_SINGLE_ARG( <br />
 * 				something, <br />
 * 				that, <br />
 * 				would, <br />
 * 				normally, <br />
 * 				be, <br />
 * 				many, <br />
 * 				args <br />
 * 			), whatever, other, arguments, are, needed) <br />
 * While this is currently exactly the same as BIO_EXPAND_TUPLE, the use case is different, so duplicate code is maintained. <br />
 */
#define BIO_SINGLE_ARG(...) __VA_ARGS__

/**
 * Calls a namespaced macro. <br />
 * NOTE: These deviate from the traditional naming scheme because namespaces are lowercase and, for ease of use, we will not require the namespace be written in both capitals and lower case. <br />
 * Will expand MY_MACRO, cellular to MY_MACRO_cellular(...) <br />
 * @param macroName the name of the macro to call
 * @param ns the namespace in which to call the macro.
 */
#define BIO_CALL_NS_MACRO(macroName, ns, ...)                                  \
    macroName##_##ns(__VA_ARGS__)

/**
 * Call a macro-based loop. These are hard-coded with a certain iteration count. <br />
 * Expands loopName from something like MY_LOOP with iterations of, say, 5 to MY_LOOP_5(...). Requires that MY_LOOP_5 be implemented. <br />
 * @param loopName the base name of the macro to call
 * @param iterations the number of times to call the given macro
 * @param ... all arguments to give to the loop, including iterations.
 */
#define BIO_CALL_LOOP(loopName, iterations, ...)                               \
    loopName##_##iterations(__VA_ARGS__)

/**
 * Singleton interface makes the constructor private so that there is only one instance ever created, which is by Instance() <br />
 * we also override default copy constructor and assignment operator so that nobody can make a copy of the singleton  (otherwise it wouldn't be a singleton). We don't define them, so these methods will give a link error if used. <br />
 * @param className the name of the new singleton
 * @param baseClass the name of the class the new singleton will inherit from (i.e. what to make into a singleton).
*/
#define BIO_SINGLETON(className, baseClass)                                    \
class className :                                                              \
    public baseClass                                                           \
{                                                                              \
public:                                                                        \
    static className& Instance()                                               \
    {                                                                          \
        static className instance;                                             \
        return instance;                                                       \
    }                                                                          \
private:                                                                       \
    className()                                                                \
    {                                                                          \
    }                                                                          \
    className(className const &);                                              \
    void operator=(className const &);                                         \
};

/**
 * Create a consistent map and pair. <br />
 */
#define BIO_DEFINE_MAP(mapName, keyType, valueType)                            \
typedef std::map<keyType, valueType> mapName;                                  \
typedef std::pair<keyType, valueType> mapName##Pair;

