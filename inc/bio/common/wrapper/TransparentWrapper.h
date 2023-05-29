/*
 * This file is a part of the Biology project by eons LLC.
 * Biology (aka Develop Biology) is a framework for approaching software
 * development from a natural sciences perspective.
 *
 * Copyright (C) 2023 Séon O'Shannon & eons LLC
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

#include <ostream>
#include "bio/common/type/IsPointer.h"

namespace bio {

//@formatter:off

/**
 * TransparentWrappers should appear to be the type they wrap in all respects.
 * However, this is not currently possible for member access. <br />
 * Unfortunately, operator type() alone is not sufficient to treat this new class as the type it holds and we must instead forward all operations to the contained type. <br />
 * ugh. <br />
 * If you know of a better solution to this problem, please make a pull request. <br />
 *
 * For example 5 + TransparentWrapper<int>(2) = 7; however, TransparentWrapper<MyClass>(myObject).mSomeMember is invalid. <br />
 * To use TransparentWrappers for more than operations on built-in types, you can use the pattern: <br />
 * 		MyClass* myObject; <br />
 * 		*myObject = TransparentWrapper< MyClass >(myOtherObject); <br />
 * Using this pattern invokes the operator MyClass(), casting *this TransparentWrapper to an instance of MyClass. <br />
 *
 * This wrapper is currently semi virtual and can technically be inherited from. <br />
 * No functions or operators are virtual but the destructor is. This is done so that other wrapper types may use *this. <br />
 * This may change in a future release. <br />
 *
 * NOTE: TransparentWrappers will not be Primitive unless explicitly implemented as such. See macros/StrongTypedef.h for more info. <br />
 *
 * @tparam T
 */
template < typename T >
class TransparentWrapper
{
public:
	typedef T Type;

	TransparentWrapper(T t) : mT(t) {}
	TransparentWrapper(const TransparentWrapper<T>& other) : mT(other.mT) {}
    virtual ~TransparentWrapper()
	{
		#if BIO_CPP_VERSION >= 17
		if BIO_CONSTEXPR(type::IsPointer<T>())
		{
			delete mT;
		}
		#endif
		//FIXME: memory leaks when wrapping pointers on cpp < 17
	}
    operator T() {return mT;}
	explicit operator const T() const {return mT;} // Breaks everything....

	//START: Support for basic types
	//TODO: Add pointer support for basic methods
	TransparentWrapper<T>& operator=(const T& t) {mT = t; return *this;}
	TransparentWrapper<T>& operator=(const TransparentWrapper<T>& other) {mT = other.mT; return *this;}
    bool operator==(const T& t) const  {return mT == t;}
    bool operator!=(const T& t) const  {return mT != t;}
    bool operator<=(const T& t) const  {return mT <= t;}
    bool operator>=(const T& t) const  {return mT >= t;}
    bool operator<(const T& t) const {return mT < t;}
    bool operator>(const T& t) const {return mT > t;}
    bool operator==(const TransparentWrapper& other) const {return mT == other.mT;}
    bool operator!=(const TransparentWrapper& other) const {return mT != other.mT;}
    bool operator<=(const TransparentWrapper& other) const {return mT <= other.mT;}
    bool operator>=(const TransparentWrapper& other) const {return mT >= other.mT;}
    bool operator<(const TransparentWrapper& other) const {return mT < other.mT;}
    bool operator>(const TransparentWrapper& other) const {return mT > other.mT;}
    T& operator++() {return ++mT;}
    T operator++(int) {return mT++;}
    T& operator--() {return --mT;}
    T operator--(int) {return mT--;}
    T operator+=(const T& t) {return mT += t;}
    T operator-=(const T& t) {return mT -= t;}
    T operator+=(const TransparentWrapper& other) {return mT += other.mT;}
    T operator-=(const TransparentWrapper& other) {return mT -= other.mT;}
    T operator+(const T& t) const {return mT + t;}
    T operator-(const T& t) const {return mT - t;}
    T operator+(const TransparentWrapper& other) const {return mT + other.mT;}
    T operator-(const TransparentWrapper& other) const {return mT - other.mT;}
    T operator*=(const T& t) {return mT *= t;}
    T operator/=(const T& t) {return mT /= t;}
    T operator*=(const TransparentWrapper& other) {return mT *= other.mT;}
    T operator/=(const TransparentWrapper& other) {return mT /= other.mT;}
    T operator*(const T& t) const {return mT * t;}
    T operator/(const T& t) const {return mT / t;}
    T operator*(const TransparentWrapper& other) const {return mT * other.mT;}
    T operator/(const TransparentWrapper& other) const {return mT / other.mT;}
    friend ::std::ostream& operator <<(std::ostream& out, const TransparentWrapper& t)
    {
        out << t.mT;
        return out;
    }
	//END: Support for basic types

	//START: Support for pointers
	T* operator->() {
		if BIO_CONSTEXPR(type::IsPointer<T>())
		{
			return mT;
		}
		return &mT;
	}
	const T* operator->() const
	{
		if BIO_CONSTEXPR(type::IsPointer<T>())
		{
			return mT;
		}
		return &mT;
	}
	//END: Support for pointers

    //that's all we're doing for now. Please add to this list as necessary

	//public because we need to treat this as T when we don't know the T.
public:
    T mT;
};
//@formatter:on

} //bio namespace
