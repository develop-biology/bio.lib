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

/**
 * Because we support c++98 but also want to take advantage of later c++ features, we have to make our own wrappers around some keywords. <br />
 * Here, we define the "constexpr" keyword for only c++11 and beyond. <br />
 */
//@formatter:off
#if BIO_CPP_VERSION < 14
	#define BIO_CONSTEXPR
#else
	#define BIO_CONSTEXPR constexpr
#endif
//@formatter:on

//@formatter:off
#if BIO_CPP_VERSION < 11
	#define BIO_FINAL
#else
	#define BIO_FINAL final
#endif
//@formatter:on
