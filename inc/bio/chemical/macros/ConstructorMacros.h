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
 * Defines constructors for classes deriving from chemical::Class and beyond. <br />
 * These will allow you to construct your object with either a name or an id. <br />
 * We also define the empty constructor for when neither name nor id are supplied. <br />
 *
 * IMPORTANT: despite using __VA_ARGS__, this will fail if nothing more than the namespace and class are provided. <br />
 * At a minimum, you MUST ALSO INCLUDE THE Perspective AFTER class! <br />
 * e.g. BIO_DEFAULT_IDENTIFIABLE_CONSTRUCONSTRUCTORS(my_namespace, MyClass, &MyClassPerspective::Instance()) <br />
 *
 * NOTE: if your class has multiple template args or otherwise uses commas in its name, you must enclose it in BIO_SINGLE_ARG() so as to not have it be considered part of the __VA_ARGS__. <br />
 * @param ns the namespace of the class
 * @param class the name of the class
 * @param ... all arguments to the ns::Class<class>(...) constructor.
 */
#define BIO_DEFAULT_IDENTIFIABLE_CONSTRUCONSTRUCTORS(ns, class, ...)                  \
class() :                                                                      \
    ns::Class< class >(this, __VA_ARGS__)                                      \
{}                                                                             \
explicit class(::bio::Name name) :                                             \
    ns::Class< class >(this, name, __VA_ARGS__)                                \
{}                                                                             \
explicit class(::bio::Id id) :                                  \
    ns::Class< class >(this, id, __VA_ARGS__)                                  \
{}

/**
 * Defines constructors for classes deriving from chemical::Class and beyond. <br />
 * All constructors call a CommonConstructor() method with no args. <br />
 * These will allow you to construct your object with either a name or an id. <br />
 * We also define the empty constructor for when neither name nor id are supplied. <br />
 *
 * IMPORTANT: despite using __VA_ARGS__, this will fail if nothing more than the namespace and class are provided. <br />
 * At a minimum, you MUST ALSO INCLUDE THE Perspective AFTER class! <br />
 * e.g. BIO_DEFAULT_IDENTIFIABLE_CONSTRUCONSTRUCTORS(my_namespace, MyClass, &MyClassPerspective::Instance()) <br />
 *
 * NOTE: if your class has multiple template args or otherwise uses commas in its name, you must enclose it in BIO_SINGLE_ARG() so as to not have it be considered part of the __VA_ARGS__. <br />
 *
 * @param ns the namespace of the class
 * @param class the name of the class
 * @param ... all arguments to the ns::Class<class>(...) constructor.
 */
#define BIO_DEFAULT_IDENTIFIABLE_CONSTRUCONSTRUCTORS_WITH_COMMON_CONSTRUCTOR(ns, class, ...) \
class() :                                                                      \
    ns::Class< class >(this, __VA_ARGS__)                                      \
{                                                                              \
    this->CommonConstructor();                                                        \
}                                                                              \
explicit class(::bio::Name name) :                                             \
    ns::Class< class >(this, name, __VA_ARGS__)                                \
{                                                                              \
    this->CommonConstructor();                                                        \
}                                                                              \
explicit class(::bio::Id id) :                                  \
    ns::Class< class >(this, id, __VA_ARGS__)                                  \
{                                                                              \
    this->CommonConstructor();                                                        \
}
