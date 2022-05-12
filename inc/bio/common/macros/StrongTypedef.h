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

//#include <ostream> //YOU MUST INCLUDE THIS WHERE USING BIO_STRONG_TYPEDEF
//#include "bio/common/TransparentWrapper.h" //YOU MUST INCLUDE THIS WHERE USING BIO_STRONG_TYPEDEF
//#include "bio/common/IsPrimitive.h" //Cannot be included due to circular dependency. YOU MUST INCLUDE THIS WHERE USING BIO_STRONG_TYPEDEF
//#include "bio/common/macros/KeywordMacros.h"
//#include "bio/common/macros/LanguageMacros.h"

/**
 * In C++, the "typedef" keyword does not create a distinct type, only an alias. Thus 2 identical typedefs of different names become merged into the same symbol at compile time. <br />
 * Here, we work around this bug by creating a wrapper class that does nothing but contain another value. <br />
 * When declaring a StrongTypedef, we also want to make sure the class is appropriately declared as Primitive (i.e. IsPrimitive< SomeStrongTypedef >() returns the appropriate value). <br />
 * NOTE: If using a C++ version below 14, this will be treated as Primitive (c++14 and above treats this as if it were the given type). If you desire behavior otherwise, either duplicate this code or selectively undef and redefine it. <br />
 */
#define BIO_STRONG_TYPEDEF(type, name, defaultValue)                           \
class name : public TransparentWrapper< type >                                 \
{                                                                              \
public:                                                                        \
    name(type t = defaultValue) : TransparentWrapper< type >(t) {}             \
    ~name() {}                                                                 \
    /*All other operators in TransparentWrapper*/                              \
    friend ::std::ostream& operator <<(std::ostream& out, const name& t)       \
    {                                                                          \
        out << t.m_t;                                                          \
        return out;                                                            \
    }                                                                          \
};                                                                             \
BIO_FORWARD_PRIMITIVE(name, type)
