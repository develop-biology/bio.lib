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

/**
 * This file contains all common preprocessor macros.
 * Additional macros may be specified in files they more specifically pertain to
 * All bio macros start with BIO_
*/
#pragma once

#include "Language.h"
#include "Assert.h"
#include "ByteStream.h"

/**
 * Wrapping macros is used to evaluate them within other macros, rather than being passed as a string.
 */
#define BIO_WRAP_MACRO(macro) macro

/**
 * Used to remove fhe parentheses from around an expression.
 * Should be invoked as: BIO_EXPAND_TUPLE tuple
 * where tuple is (something, like, this)
 * which expands to BIO_EXPAND_TUPLE (something, like, this) => something, like, this
 */
#define BIO_EXPAND_TUPLE(...) __VA_ARGS__

/**
 * Encapsulates an expression containing commas, allowing it to be safely passed as a single macro argument.
 * Should be invoked as:
 * 		SOME_OTHER_MACRO(
 * 			BIO_SINGLE_ARG(
 * 				something,
 * 				that,
 * 				would,
 * 				normally,
 * 				be,
 * 				many,
 * 				args
 * 			), whatever, other, arguments, are, needed)
 * While this is currently exactly the same as BIO_EXPAND_TUPLE, the use case is different, so duplicate code is maintained.
 */
#define BIO_SINGLE_ARG(...) __VA_ARGS__

/**
 * Implementation for BIO_GET_NUM_ARGS.
 * You can ignore this.
 */
#define BIO_LAST_ARG_IN_RANGE(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32, a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62, a63, a64, a65, a66, a67, a68, a69, a70, a71, a72, a73, a74, a75, a76, a77, a78, a79, a80, a81, a82, a83, a84, a85, a86, a87, a88, a89, a90, a91, a92, a93, a94, a95, a96, a97, a98, a99, a100, ...) a100

/**
 * @return the number of args given, up to 99.
 */
#define BIO_GET_NUM_ARGS(...)                                                  \
	BIO_LAST_ARG_IN_RANGE(dummy, ## __VA_ARGS__, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

/**
 * Calls BIO_GET_NUM_ARGS after evaluating the provided macro.
 * @param macro a macro that returns some number of values
 * @return the number of values returned by macro, up to 99.
 */
#define BIO_GET_NUM_ARGS_OF_MACRO(macro)                                       \
    BIO_LAST_ARG_IN_RANGE(dummy, macro, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

/**
 * Calls a namespaced macro.
 * NOTE: These deviate from the traditional naming scheme because namespaces are lowercase and, for ease of use, we will not require the namespace be written in both capitals and lower case.
 * Will expand MY_MACRO, cellular to MY_MACRO_cellular(...)
 * @param macroName the name of the macro to call
 * @param ns the namespace in which to call the macro.
 */
#define BIO_CALL_NS_MACRO(macroName, ns, ...)                                  \
	macroName##_##ns(__VA_ARGS__)

/**
 * Call a macro-based loop. These are hard-coded with a certain iteration count.
 * Expands loopName from something like MY_LOOP with iterations of, say, 5 to MY_LOOP_5(...). Requires that MY_LOOP_5 be implemented.
 * @param loopName the base name of the macro to call
 * @param iterations the number of times to call the given macro
 * @param ... all arguments to give to the loop, including iterations.
 */
#define BIO_CALL_LOOP(loopName, iterations, ...)                               \
    loopName##_##iterations(__VA_ARGS__)

/**
 * Singleton implementation makes the constructor private so that there is only one instance ever created, which is by Instance()
 * we also override default copy constructor and assignment operator so that nobody can make a copy of the singleton  (otherwise it wouldn't be a singleton). We don't define them, so these methods will give a link error if used.
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
 * Create a consistent map and pair.
 */
#define BIO_DEFINE_MAP(mapName, keyType, valueType)                            \
typedef std::map<keyType, valueType> mapName;                                  \
typedef std::pair<keyType, valueType> mapName##Pair;


#if BIO_CPP_VERSION < 11
	#define BIO_CACHE(expression)                                              \
    bio::ByteStream RESULT;                                                    \
    RESULT.Set(expression);
#else
	#define BIO_CACHE(expression)                                              \
	auto RESULT = (expression);
#endif

/**
 * BIO_SANITIZE evaluates the current BIO_SAFETY_LEVEL and performs the appropriate checks.
 * A lower BIO_SAFETY_LEVEL will mean faster run speed but a higher chance of crashing.
 * NOTE: "auto" in the examples below is only available after c++11 and should be substituted with the appropriate type.
 *
 * This is basically (test) ? (success) : (failure)
 *
 * IMPORTANT: Do not declare variables within any statement (test, success, & failure).
 * Any variables declared will go out of scope and won't be accessible to you.
 *
 * BIO_SANITIZE should be usable in the following ways (and the variations thereof):
 * BIO_SANITIZE(some_test(), some_test()->do_something(), );
 * BIO_SANITIZE(myVar, myVar->do_something(), do_something_else());
 * auto myVar; BIO_SANITIZE(some_test(), myVar = some_test(), myVar = 0);
 * auto myOtherVar; BIO_SANITIZE(myVar, myOtherVar = myVar->do_something(), myOtherVar = 0);
 */
#define BIO_SANITIZE_AT_SAFETY_LEVEL_0(test, success, failure)                 \
{                                                                              \
    success;                                                                   \
}

#define BIO_SANITIZE_AT_SAFETY_LEVEL_1(test, success, failure)                 \
{                                                                              \
    BIO_ASSERT(test);                                                          \
    success;                                                                   \
}

#define BIO_SANITIZE_AT_SAFETY_LEVEL_2(test, success, failure)                 \
if (test)                                                                      \
{                                                                              \
    success;                                                                   \
}                                                                              \
else                                                                           \
{                                                                              \
    failure;                                                                   \
}

//@formatter:off
#if defined(BIO_SAFETY_LEVEL) && BIO_SAFETY_LEVEL == 0
	#define BIO_SANITIZE(test, success, failure)    BIO_SANITIZE_AT_SAFETY_LEVEL_0(test, BIO_SINGLE_ARG(success), BIO_SINGLE_ARG(failure))
#elif defined(BIO_SAFETY_LEVEL) && BIO_SAFETY_LEVEL == 1
	#define BIO_SANITIZE(test, success, failure)    BIO_SANITIZE_AT_SAFETY_LEVEL_1(test, BIO_SINGLE_ARG(success), BIO_SINGLE_ARG(failure))
#else
	#define BIO_SANITIZE(test, success, failure)    BIO_SANITIZE_AT_SAFETY_LEVEL_2(test, BIO_SINGLE_ARG(success), BIO_SINGLE_ARG(failure))
#endif
//@formatter:on

/**
 * For more intensive tests, the result can be cached in a variable called RESULT of the given type.
 *
 * You can access the cached result of TEST in the variable RESULT.
 *               THIS CAN GO HORRIBLY WRONG!!!!!
 * C++11 is safe and should throw compiler errors if you mess up but C++98 requires the use of a ByteStream hack around the auto keyword. This should be safe when using built in types and pointers but THE BEHAVIOR OF ANYTHING ELSE IS UNDEFINED.

 * To make sure this is safe, do the following:
 *     1. Make sure your success and/or failure expressions treat RESULT as the return type of your test expression.
 *     2. Do not use a test expression that returns a non-basic type if you are using C++98. Pointers are okay.
 *
 * The above examples can be rewritten as:
 * BIO_SANITIZE_WITH_CACHE(some_test(), RESULT->do_something(), );
 * BIO_SANITIZE_WITH_CACHE(myVar, myVar->do_something(), do_something_else());
 * auto myVar; BIO_SANITIZE_WITH_CACHE(some_test(), myVar = RESULT, myVar = 0);
 * auto myOtherVar; BIO_SANITIZE_WITH_CACHE(myVar, myOtherVar = myVar->do_something(), myOtherVar = 0);
 *
 * Just as with BIO_SANITIZE, "auto" may only be used with c++11 or greater and should be replaced with the appropriate type.
 * NOTE: when using myVar->methods(), we don't use RESULT so that any mutating (i.e. non-const) operations are applied to myVar and not lost when RESULT goes out of scope.
 */
#define BIO_SANITIZE_WITH_CACHE(test, success, failure)                        \
{                                                                              \
    BIO_CACHE(test)                                                            \
    BIO_SANITIZE(                                                              \
        RESULT,                                                                \
        BIO_SINGLE_ARG(success),                                                  \
        BIO_SINGLE_ARG(failure))                                                  \
}

#define BIO_SANITIZE_WITH_CACHE_AT_SAFETY_LEVEL_0(test, success, failure)      \
{                                                                              \
    BIO_CACHE(test)                                                            \
    BIO_SANITIZE_AT_SAFETY_LEVEL_0(                                            \
        RESULT,                                                                \
        BIO_SINGLE_ARG(success),                                                  \
        BIO_SINGLE_ARG(failure))                                                  \
}

#define BIO_SANITIZE_WITH_CACHE_AT_SAFETY_LEVEL_1(test, success, failure)      \
{                                                                              \
    BIO_CACHE(test)                                                            \
    BIO_SANITIZE_AT_SAFETY_LEVEL_1(                                            \
        RESULT,                                                                \
        BIO_SINGLE_ARG(success),                                                  \
        BIO_SINGLE_ARG(failure))                                                  \
}

#define BIO_SANITIZE_WITH_CACHE_AT_SAFETY_LEVEL_2(test, success, failure)      \
{                                                                              \
    BIO_CACHE(test)                                                            \
    BIO_SANITIZE_AT_SAFETY_LEVEL_2(                                            \
        RESULT,                                                                \
        BIO_SINGLE_ARG(success),                                                  \
        BIO_SINGLE_ARG(failure))                                                  \
}
