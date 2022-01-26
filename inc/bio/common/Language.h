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

#define BIO_CPP_VERSION 0
#define BIO_C_VERSION 0


#ifdef __cplusplus
	#if __cplusplus == 199711L
		#undef BIO_CPP_VERSION
		#define BIO_CPP_VERSION 1
	#elif __cplusplus == 201103L
		#undef BIO_CPP_VERSION
		#define BIO_CPP_VERSION 11
	#elif __cplusplus == 201402L
		#undef BIO_CPP_VERSION
		#define BIO_CPP_VERSION 14
	#elif __cplusplus == 201703L
		#undef BIO_CPP_VERSION
		#define BIO_CPP_VERSION 17
	#elif __cplusplus == 202002L
		#undef BIO_CPP_VERSION
		#define BIO_CPP_VERSION 20
	#else
		#undef BIO_CPP_VERSION
		#define BIO_CPP_VERSION 20
	#endif
#endif

#ifdef __STDC_VERSION__
	#if __STDC_VERSION__ == 199409L
		#undef BIO_C_VERSION
		#define BIO_C_VERSION 1
	#elif __STDC_VERSION__ == 199901L
		#undef BIO_C_VERSION
		#define BIO_C_VERSION 2
	#elif __STDC_VERSION__ == 201112L
		#undef BIO_C_VERSION
		#define BIO_C_VERSION 11
	#elif __STDC_VERSION__ == 201710L
		#undef BIO_C_VERSION
		#define BIO_C_VERSION 18
	#else
		#undef BIO_C_VERSION
		#define BIO_C_VERSION 18
	#endif
#endif
