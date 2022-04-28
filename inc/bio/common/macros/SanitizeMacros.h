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

#include "CacheMacros.h"


/**
 * BIO_SANITIZE evaluates the current BIO_SAFETY_LEVEL and performs the appropriate checks. <br />
 * A lower BIO_SAFETY_LEVEL will mean faster run speed but a higher chance of crashing. <br />
 * NOTE: "auto" in the examples below is only available after c++11 and should be substituted with the appropriate type. <br />
 *
 * This is basically (test) ? (success) : (failure) <br />
 *
 * IMPORTANT: Do not declare variables within any statement (test, success, & failure). <br />
 * Any variables declared will go out of scope and won't be accessible to you. <br />
 *
 * BIO_SANITIZE should be usable in the following ways (and the variations thereof): <br />
 * BIO_SANITIZE(some_test(), some_test()->do_something(), ) <br />
 * BIO_SANITIZE(myVar, myVar->do_something(), do_something_else()) <br />
 * auto myVar; BIO_SANITIZE(some_test(), myVar = some_test(), myVar = 0) <br />
 * auto myOtherVar; BIO_SANITIZE(myVar, myOtherVar = myVar->do_something(), myOtherVar = 0) <br />
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
 * For more intensive tests, the result can be cached in a variable called RESULT of the given type. <br />
 *
 * You can access the cached result of TEST in the variable RESULT. <br />
 *               THIS CAN GO HORRIBLY WRONG!!!!! <br />
 * C++11 is safe and should throw compiler errors if you mess up but C++98 requires the use of a ByteStream hack around the auto keyword. This should be safe when using built in types and pointers but THE BEHAVIOR OF ANYTHING ELSE IS UNDEFINED. <br />

 * To make sure this is safe, do the following: <br />
 *     1. Make sure your success and/or failure expressions treat RESULT as the return type of your test expression. <br />
 *     2. Do not use a test expression that returns a non-basic type if you are using C++98. Pointers are okay. <br />
 *
 * The above examples can be rewritten as: <br />
 * BIO_SANITIZE_WITH_CACHE(some_test(), RESULT->do_something(), ); <br />
 * BIO_SANITIZE_WITH_CACHE(myVar, myVar->do_something(), do_something_else()); <br />
 * auto myVar; BIO_SANITIZE_WITH_CACHE(some_test(), myVar = RESULT, myVar = 0); <br />
 * auto myOtherVar; BIO_SANITIZE_WITH_CACHE(myVar, myOtherVar = myVar->do_something(), myOtherVar = 0); <br />
 *
 * Just as with BIO_SANITIZE, "auto" may only be used with c++11 or greater and should be replaced with the appropriate type. <br />
 * NOTE: when using myVar->methods(), we don't use RESULT so that any mutating (i.e. non-const) operations are applied to myVar and not lost when RESULT goes out of scope. <br />
 */
#define BIO_SANITIZE_WITH_CACHE(test, success, failure)                        \
{                                                                              \
    BIO_BYTESTREAM_CACHE(test)                                                            \
    BIO_SANITIZE(                                                              \
        RESULT,                                                                \
        BIO_SINGLE_ARG(success),                                               \
        BIO_SINGLE_ARG(failure))                                               \
}

#define BIO_SANITIZE_WITH_CACHE_AT_SAFETY_LEVEL_0(test, success, failure)      \
{                                                                              \
    BIO_BYTESTREAM_CACHE(test)                                                            \
    BIO_SANITIZE_AT_SAFETY_LEVEL_0(                                            \
        RESULT,                                                                \
        BIO_SINGLE_ARG(success),                                               \
        BIO_SINGLE_ARG(failure))                                               \
}

#define BIO_SANITIZE_WITH_CACHE_AT_SAFETY_LEVEL_1(test, success, failure)      \
{                                                                              \
    BIO_BYTESTREAM_CACHE(test)                                                            \
    BIO_SANITIZE_AT_SAFETY_LEVEL_1(                                            \
        RESULT,                                                                \
        BIO_SINGLE_ARG(success),                                               \
        BIO_SINGLE_ARG(failure))                                               \
}

#define BIO_SANITIZE_WITH_CACHE_AT_SAFETY_LEVEL_2(test, success, failure)      \
{                                                                              \
    BIO_BYTESTREAM_CACHE(test)                                                            \
    BIO_SANITIZE_AT_SAFETY_LEVEL_2(                                            \
        RESULT,                                                                \
        BIO_SINGLE_ARG(success),                                               \
        BIO_SINGLE_ARG(failure))                                               \
}
