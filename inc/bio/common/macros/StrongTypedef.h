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

/**
 * Another problem with C++: the "typedef" keyword does not create a distinct type, only an alias. Thus 2 identical typedefs of different names become merged into the same symbol at compile time. <br />
 *
 * Here, we work around this bug by creating a wrapper class that does nothing but contain another value. <br />
 * Unfortunately, operator type() alone is not sufficient to treat this new class as the type it holds and we must instead forward all operations to the contained type. <br />
 * ugh. <br />
 * If you know of a better solution to this problem, please make a pull request. <br />
 *
 * This wrapper is currently not virtual and cannot be inherited from. This may change in a future release. <br />
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
    type operator*(const type& t) const {return m_t * t;}                      \ <br />
    type operator/(const type& t) const {return m_t / t;}                      \
    type operator*(const name& other) const {return m_t * other.m_t;}          \ <br />
    type operator/(const name& other) const {return m_t / other.m_t;}          \
    friend std::ostream& operator <<(std::ostream& out, const name& t)         \
    {                                                                          \
        out << t.m_t;                                                          \
        return out;                                                            \
    }                                                                          \
    /*that's all we're doing for now. Please add to this list as necessary*/   \
                                                                               \
/*public because we need to treat this as type when we don't know the type.*/  \
public:                                                                        \
    type m_t;                                                                  \
};
