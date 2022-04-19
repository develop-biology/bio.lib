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

//since we use these here, we might as well include them.
#include <ostream>
//#include "bio/common/IsPrimitive.h" //Cannot be included due to circular dependency. YOU MUST INCLUDE THIS WHERE USING BIO_STRONG_TYPEDEF

/**
 * Apparently, we cannot use fully qualified names to define structs yet. <br />
 * See: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=66892 <br />
 * Until this is patched, we must use a separate macro to define any sibling namespace symbols. <br />
 * THIS MUST BE CALLED IN THE bio NAMESPACE! <br />
 * name and type must both also be FULLY QUALIFIED past the ::bio namespace! <br />
 * NOTE: If using c++11 or below, this is hard-coded to as Primitive. If you desire behavior otherwise, either duplicate this code or selectively undef and redefine it. <br />
 */
#if BIO_CPP_VERSION < 14
#define BIO_STRONG_TYPEDEF_BIO_NAMESPACE_DEFINITIONS(type, name, defaultValue) \
/*Forward primitive name to type.*/                                            \
namespace utility {                                                            \
template <>                                                                    \
struct IsPrimitiveImplementation< name >                                       \
{                                                                              \
    static const bool m_value = true;                                          \
};                                                                             \
} /*utility namespace*/
#else
#define BIO_STRONG_TYPEDEF_BIO_NAMESPACE_DEFINITIONS(type, name, defaultValue) \
/*Forward primitive name to type.*/                                            \
namespace utility {                                                            \
template <>                                                                    \
struct IsPrimitiveImplementation< name >                                       \
{                                                                              \
    static const bool m_value = IsPrimitive< type >();                         \
};                                                                             \
} /*utility namespace*/
#endif

/**
 * Another problem with C++: the "typedef" keyword does not create a distinct type, only an alias. Thus 2 identical typedefs of different names become merged into the same symbol at compile time. <br />
 *
 * Here, we work around this bug by creating a wrapper class that does nothing but contain another value. <br />
 * Unfortunately, operator type() alone is not sufficient to treat this new class as the type it holds and we must instead forward all operations to the contained type. <br />
 * ugh. <br />
 * If you know of a better solution to this problem, please make a pull request. <br />
 *
 * This wrapper is currently not virtual and cannot be inherited from. This may change in a future release. <br />
 *
 * NOTE: THIS MUST BE CALLED FROM THE ::bio NAMESPACE! <br />
 * Until the bug in gcc that necessitates BIO_STRONG_TYPEDEF_BIO_NAMESPACE_DEFINITIONS is fixed, we cannot support namespaced types. <br />
 * :( <br />
 */
#define BIO_STRONG_TYPEDEF(type, name, defaultValue)                           \
class name                                                                     \
{                                                                              \
public:                                                                        \
    name(type t = defaultValue) :                                              \
        m_t(t)                                                                 \
    {}                                                                         \
    ~name() {}                                                                 \
    operator type() {return m_t;}                                              \
    bool operator==(const type& t) const  {return m_t == t;}                   \
    bool operator!=(const type& t) const  {return m_t != t;}                   \
    bool operator<=(const type& t) const  {return m_t <= t;}                   \
    bool operator>=(const type& t) const  {return m_t >= t;}                   \
    bool operator<(const type& t) const {return m_t < t;}                      \
    bool operator>(const type& t) const {return m_t > t;}                      \
    bool operator==(const name& other) const {return m_t == other.m_t;}        \
    bool operator!=(const name& other) const {return m_t != other.m_t;}        \
    bool operator<=(const name& other) const {return m_t <= other.m_t;}        \
    bool operator>=(const name& other) const {return m_t >= other.m_t;}        \
    bool operator<(const name& other) const {return m_t < other.m_t;}          \
    bool operator>(const name& other) const {return m_t > other.m_t;}          \
    type& operator++() {return ++m_t;}                                         \
    type operator++(int) {return m_t++;}                                       \
    type& operator--() {return --m_t;}                                         \
    type operator--(int) {return m_t--;}                                       \
    type operator+=(const type& t) {return m_t += t;}                          \
    type operator-=(const type& t) {return m_t -= t;}                          \
    type operator+=(const name& other) {return m_t += other.m_t;}              \
    type operator-=(const name& other) {return m_t -= other.m_t;}              \
    type operator+(const type& t) const {return m_t + t;}                      \
    type operator-(const type& t) const {return m_t - t;}                      \
    type operator+(const name& other) const {return m_t + other.m_t;}          \
    type operator-(const name& other) const {return m_t - other.m_t;}          \
    type operator*=(const type& t) {return m_t *= t;}                          \
    type operator/=(const type& t) {return m_t /= t;}                          \
    type operator*=(const name& other) {return m_t *= other.m_t;}              \
    type operator/=(const name& other) {return m_t /= other.m_t;}              \
    type operator*(const type& t) const {return m_t * t;}                      \
    type operator/(const type& t) const {return m_t / t;}                      \
    type operator*(const name& other) const {return m_t * other.m_t;}          \
    type operator/(const name& other) const {return m_t / other.m_t;}          \
    friend ::std::ostream& operator <<(std::ostream& out, const name& t)         \
    {                                                                          \
        out << t.m_t;                                                          \
        return out;                                                            \
    }                                                                          \
    /*that's all we're doing for now. Please add to this list as necessary*/   \
                                                                               \
/*public because we need to treat this as type when we don't know the type.*/  \
public:                                                                        \
    type m_t;                                                                  \
};                                                                             \
BIO_STRONG_TYPEDEF_BIO_NAMESPACE_DEFINITIONS(                                  \
	BIO_SINGLE_ARG(type),                                                      \
	name,                                                                      \
	BIO_SINGLE_ARG(defaultValue))
