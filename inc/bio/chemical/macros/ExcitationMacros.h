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

#define BIO_EXCITATION_CLASS_0(wave, ret, ...)                                 \
	::bio::chemical::ExcitationWithoutArgument< wave, ret >

#define BIO_EXCITATION_CLASS_1(wave, ret, ...)                                 \
	::bio::chemical::ExcitationWithArgument< wave, ret, __VA_ARGS__ >

#define BIO_EXCITATION_CLASS_2(wave, ret, ...)                                 \
	::bio::chemical::ExcitationWithTwoArguments< wave, ret, __VA_ARGS__ >

#define BIO_CREATE_EXCITATION_CLASS(wave, ret, num, ...)                       \
	BIO_EXCITATION_CLASS_##num(wave, ret, __VA_ARGS__)

#define BIO_CREATE_EXCITATION_CLASS_WITH_NUM(wave, ret, num, ...)              \
	BIO_CREATE_EXCITATION_CLASS(wave, ret, num, __VA_ARGS__)

/**
 * Work around for C++ < 17, where variadic macros are supported but variadic template parameters are not.
 * This will work with all versions of C++ (see undef below).
 * NOTE: only 1 argument is currently supported with C++ < 17. For 2 or more arguments, you must either extend this macro by creating your own Excitation and defining BIO_EXCITATION_2, etc. or use at least C++17.
 * @param wave a class deriving from physical::Wave* (a class, not an object)
 * @param ret the return type of the function to call
 * @param ... any arguments to the function
 */
#define BIO_EXCITATION_CLASS(wave, ret, ...)                                   \
	BIO_CREATE_EXCITATION_CLASS_WITH_NUM(                                      \
		wave,                                                                  \
		ret,                                                                   \
		BIO_GET_NUM_ARGS(__VA_ARGS__),                                         \
		__VA_ARGS__                                                            \
	)

//@formatter:off
#if BIO_CPP_VERSION >= 17
	#undef BIO_EXCITATION_CLASS
	#define BIO_EXCITATION_CLASS(wave, ret ...)                                \
		::bio::chemical::Excitation< wave, ret, ... >
#endif
//@formatter:on

//This only works for Excitations of 0 arguments. We cannot pass both the arg type and value at this time.
//#define BIO_EXCITATION(wave, ret, func, ...)                                   \
//	new BIO_EXCITATION_CLASS(wave, ret, __VA_ARGS__)(&wave::func)
