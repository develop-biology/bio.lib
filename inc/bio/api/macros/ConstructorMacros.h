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
 * Defines constructors for classes deriving from any Final class. <br />
 * These will allow you to construct your object with either a name or an id. <br />
 * We also define the empty constructor for when neither name nor id are supplied. <br />
 *
 * NOTE: if your class has multiple template args or otherwise uses commas in its name, you must enclose it in BIO_SINGLE_ARG() so as to not have it be considered part of the __VA_ARGS__. <br />
 * @param thisClass the name of the class to construct
 * @param finalClass the name of the Final____ class *this is deriving from.
 */
#define BIO_CONSTRUCONSTRUCTORS(thisClass, finalClass)                                \
thisClass() :                                                                  \
    finalClass()                                                               \
{}                                                                             \
explicit thisClass(::bio::Name name) :                                         \
    finalClass(name)                                                           \
{}                                                                             \
explicit thisClass(::bio::Id id) :                              \
    finalClass(id)                                                             \
{}


 /**
 * Defines constructors for classes deriving from any Final class. <br />
 * All constructors call a CommonConstructor() method with no args. <br />
 * These will allow you to construct your object with either a name or an id. <br />
 * We also define the empty constructor for when neither name nor id are supplied. <br />
 *
 * NOTE: if your class has multiple template args or otherwise uses commas in its name, you must enclose it in BIO_SINGLE_ARG() so as to not have it be considered part of the __VA_ARGS__. <br />
 * @param thisClass the name of the class to construct
 * @param finalClass the name of the Final____ class *this is deriving from.
 */
#define BIO_CONSTRUCONSTRUCTORS_WITH_COMMON_CONSTRUCTOR(thisClass, finalClass)               \
thisClass() :                                                                  \
    finalClass()                                                               \
{                                                                              \
	this->CommonConstructor();                                                        \
}                                                                              \
explicit thisClass(::bio::Name name) :                                         \
    finalClass(name)                                                           \
{                                                                              \
	this->CommonConstructor();                                                        \
}                                                                              \
explicit thisClass(::bio::Id id) :                              \
    finalClass(id)                                                             \
{                                                                              \
    this->CommonConstructor();                                                        \
}
