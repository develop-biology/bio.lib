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

#include "bio/physical/common/Class.h"
#include "bio/physical/symmetry/Symmetry.h"
#include "bio/physical/common/SymmetryTypes.h"
#include "bio/physical/common/Superpositions.h"
#include "bio/common/macro/Macros.h"
#include "bio/common/type/TypeName.h"
#include "Collapse.h"
#include "Interference.h"

namespace bio {
namespace physical {

/**
 * Quanta are simple Waves intended for built-in types. <br />
 * They allow anything to be treated as a Biological Wave. <br />
 * Iff you cannot derive from Wave, use Quantum<> instead. <br />
 * Each Quantum<T> will store a pointer to a T which it will new on creation and delete on destruction. <br />
 * @tparam T
 */
template < typename T >
class Quantum :
	public physical::Class< Quantum< T > >
{
public:

	/**
	 * Ensure virtual methods point to Class implementations. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(physical, Quantum< T >)

	/**
	 *
	 */
	Quantum()
		:
		physical::Class< Quantum< T > >(
			this,
			new Symmetry(
				type::TypeName< T >(),
				symmetry_type::Value())),
		mQuantized(new T()),
		mControlling(true)
	{

	}

	/**
	 * @param assignment
	 */
	Quantum(const T& assignment)
		:
		physical::Class< Quantum< T > >(
			this,
			new Symmetry(
				type::TypeName< T >(),
				symmetry_type::Value())),
		mQuantized(new T(assignment)),
		mControlling(true)
	{
	}

	Quantum(T* directControl)
		:
		physical::Class< Quantum< T > >(
			this,
			new Symmetry(
				type::TypeName< T >(),
				symmetry_type::Value())),
		mQuantized(directControl),
		mControlling(false)
	{
	}

	/**
	 * @param other
	 */
	Quantum(const Quantum< T >& other)
		:
		physical::Class< Quantum< T > >(
			this,
			new Symmetry(
				type::TypeName< T >(),
				symmetry_type::Value())),
		mQuantized(new T(other)),
		mControlling(true)
	{

	}

	/**
	 *
	 */
	virtual ~Quantum()
	{
		if (this->mControlling && this->mQuantized)
		{
			delete this->mQuantized;
			this->mQuantized = NULL;
		}
	}

	/**
	 * Simple getter.
	 * @return the object wrapped by *this.
	 */
	virtual T* GetQuantumObject()
	{
		BIO_SANITIZE(this->mQuantized,,return NULL)
		return this->mQuantized;
	}

	/**
	 * Simple getter.
	 * @return the object wrapped by *this.
	 */
	virtual const T* GetQuantumObject() const
	{
		BIO_SANITIZE(this->mQuantized,,return NULL)
		return this->mQuantized;
	}

	/**
	 * Transparent access to the value of *this. <br />
	 * For ease of use. <br />
	 * @return *this as a T.
	 */
	operator T&()
	{
		BIO_SANITIZE(mQuantized,
			return *this->GetQuantumObject(),
			return T());
	}

	/**
	 * Transparent access to the value of *this. <br />
	 * For ease of use. <br />
	 * @return *this as a T.
	 */
	operator const T&() const
	{
		BIO_SANITIZE(mQuantized,
			return *this->GetQuantumObject(),
			return T());
	}

	/**
	 * Required method from Wave. See that class for details. <br />
	 * @return a Symmetrical image of *this
	 */
	virtual const Symmetry* Spin() const
	{
		this->mSymmetry->AccessValue()->Set(*this->mQuantized);
		return this->Wave::Spin();
	}

	/**
	 * Required method from Wave. See that class for details. <br />
	 * Reconstruct *this from the given Symmetry. <br />
	 * @param symmetry
	 */
	virtual Code Refiy(const Symmetry* symmetry)
	{
		BIO_SANITIZE(symmetry, ,
			return code::BadArgument1());
		//Wave::Reify(symmetry); //this does nothing useful.
		*this->mQuantized = symmetry->GetValue().As< T >();
		return code::Success();
	}

	/**
	 * Override of Wave method. <br />
	 * Here, we enable Superposing all Quantum Variables. <br />
	 * NOTE: The value of our mInterference does not matter; the mInterference of the interfere shall prevail. <br />
	 * @param displacement
	 * @param pattern
	 * @return whether or not there is more work to do; see Wave.h for more info.
	 */
	virtual bool Superpose(const ConstWaves& displacement, const Interference* pattern)
	{
		//First, make sure our parent can't take care of this for us.
		BIO_SANITIZE_AT_SAFETY_LEVEL_1(this->Superpose(displacement, pattern), return true,)

		//Assume all Waves, including *this, are Spinning appropriately.

		const Superposition superposition = pattern->GetSuperpositionFor(this->mSymmetry->GetId());

		if (superposition == superposition::Complex())
		{
			return true;
		}

		ByteStream collapse = Collapse::Measure(superposition, displacement);
		BIO_SANITIZE(collapse.Is< T >(), , return false)
		(*this->mQuantized) = collapse.As< T >();
		return false;
	}

protected:
	T* mQuantized;
	bool mControlling;
};

} //physical namespace
} //bio namespace
