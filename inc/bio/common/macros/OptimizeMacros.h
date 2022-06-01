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
 * BIO_SAFETY_LEVEL dictates how fast vs safe *this should be. <br />
 * A lower level means faster runtime and a higher chance of crashing. <br />
 * A higher level means slower runtime and a smaller chance of crashing. <br />
 * Values generally range from 0 to 3 (unless you make your own code dependent on the BIO_SAFETY_LEVEL). <br />
 * For BIO_SANITIZE, this means: <br />
 * 0. just run success, don't even check the condition; i.e. assume failure is not possible. <br />
 * 1. check the condition and run the provided failure if necessary; i.e. assume failure is not bad. <br />
 * 2. check the condition and throw an exception when failing; i.e. assume failure is fatal but recoverable if caught. <br />
 * 3. assert the condition is true and halt execution when failing; i.e. assume failure is fatal and cannot be recovered. <br />
 * NOTE: there are no catch statements in *this framework. <br />
 *
 * The default BIO_SAFETY_LEVEL is 3.
 */
#ifndef BIO_SAFETY_LEVEL
	#define BIO_SAFETY_LEVEL 3
#endif

/**
 * Thread locking & unlocking is unnecessary on single-threaded builds. <br />
 * If you do not intend on using threads at all, <br />
 * #define BIO_THREAD_ENFORCEMENT_LEVEL 0 <br />
 * Doing so will make all thread related operations into nops and save you some cpu cycles. <br />
 * At higher thread enforcement levels, locking becomes increasingly strict. It is recommended that you compile with as high of an enforcement level as you can. <br />
 * The current max is 2. <br />
 */
#ifndef BIO_THREAD_ENFORCEMENT_LEVEL
	#define BIO_THREAD_ENFORCEMENT_LEVEL 2
#endif

/**
 * Certain places in the bio framework afford easy toggling between storing fewer variables and calculating the values only when needed or caching the values and only calculating them once (or as necessary). <br />
 * BIO_MEMORY_OPTIMIZE_LEVEL controls this tradeoff. <br />
 * At a lower value, BIO_MEMORY_OPTIMIZE_LEVEL will cause more memory to be cached, saving cpu. <br />
 * At a higher value, BIO_MEMORY_OPTIMIZE_LEVEL will cause less memory to be cached, costing cpu. <br />
 * Values generally range from 0 to 1 (on or off, for the time being). <br />
 */
#ifndef BIO_MEMORY_OPTIMIZE_LEVEL
	#define BIO_MEMORY_OPTIMIZE_LEVEL 0
#endif

/**
 * The symmetry system is somewhat costly, especially on memory. <br />
 * If you do not need any of the features offered by Symmetry, <br />
 * #define BIO_ENABLE_REFLECTION 0. <br />
 * Please do not #undef this. Just set it to 0. <br />
 * See the following for more info (all in bio/physical): <br />
 * Symmetry.h <br />
 * Axis.h <br />
 * Wave.h <br />
 * and all the classes in bio/molecular. <br />
 */
#ifndef BIO_ENABLE_REFLECTION
	#define BIO_ENABLE_REFLECTION 1
#endif
