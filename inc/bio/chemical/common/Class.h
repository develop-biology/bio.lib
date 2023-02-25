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

#include "bio/physical/common/Class.h"
#include "bio/physical/common/Filters.h"
#include "bio/log/Writer.h"
#include "bio/chemical/common/Cast.h"
#include "bio/chemical/relativity/Elementary.h"
#include "bio/chemical/bonding/Atom.h"
#include "SymmetryTypes.h"

namespace bio {
namespace chemical {

class Substance;

/**
 * A chemical::Class is an Identifiable Atom with logging capabilities. <br />
 * This will FormBond() with the provided T. <br />
 * Class in other namespaces will grow to include more complex, templated logic. <br />
 * This pattern prevents you from having to define virtual methods each of your child classes, so long as you always derive from the appropriate Class<T>. <br />
 * @tparam T a pointer to a class type.
 */
template < typename T >
class Class :
	public physical::Class< T >,
	protected Elementary< T >,
	virtual public physical::Identifiable< Id >,
	virtual public log::Writer,
	virtual public Atom
{
private:
	void CommonConstructor(Filter filter = filter::Default(), const Id& id=0, const Name& name=NULL)
	{
		if (filter != filter::Default())
		{
			log::Writer::Initialize(filter);
		}

		if (id > 0)
		{
			physical::Identifiable< Id >::Initialize(
				id,
				&IdPerspective::Instance());
		}
		else if (name)
		{
			physical::Identifiable< Id >::Initialize(
				name,
				&IdPerspective::Instance());
		}
		//Leave uninitialized. Maybe a child knows more and would prefer to Initialize() the Identifiable base.

		//Bond the class we're given, Virtually.
		//Cannot use mObject because it doesn't exist yet.
		AtomicNumber bondedId = Atom::GetBondId< T >();

		#if BIO_CPP_VERSION < 17
		//TODO: Atom expects a Quantum< T > but can we make one if we don't have a T yet?
		//Propose: adding a DirectAccess() method to Quantum would allow us to hack this.
		//Or: make implicit Quntum constructor from Wave*.
		this->AsAtom()->FormBondImplementation(
			this->AsWave(),
			bondedId,
			bond_type::Virtual());
		#else
		this->AsAtom()->FormBondImplementation(
			this->AsWave(),
			bondedId,
			bond_type::Virtual());
		#endif


	}

public:
	/**
	 * Ensure virtual methods point to Class implementations. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(physical, T)

	/**
	 * For when we know the Perspective but not ourselves. <br />
	 * @param object
	 * @param perspective
	 * @param filter
	 * @param symmetryType
	 */
	Class(
		T* object,
		Filter filter = filter::Default(),
		SymmetryType symmetryType = symmetry_type::Object())
		:
		physical::Class< T >(
			object,
			new physical::Symmetry(
				type::TypeName< T >(),
				symmetryType
			)),
		Elementary< T >()
	{
		CommonConstructor(filter);
	}


	/**
	 * @param object
	 * @param name
	 * @param perspective
	 * @param filter
	 */
	Class(
		T* object,
		const Name& name,
		Filter filter = filter::Default(),
		SymmetryType symmetryType = symmetry_type::Object())
		:
		physical::Class< T >(
			object,
			new physical::Symmetry(
				type::TypeName< T >(),
				symmetryType
			)),
		Elementary< T >()
	{
		CommonConstructor(filter, 0, name);
	}

	/**
	 * @param object
	 * @param id
	 * @param perspective
	 * @param filter
	 */
	Class(
		T* object,
		const Id& id,
		Filter filter = filter::Default(),
		SymmetryType symmetryType = symmetry_type::Object())
		:
		physical::Class< T >(
			object,
			new physical::Symmetry(
				type::TypeName< T >(),
				symmetryType
			)),
		Elementary< T >()
	{
		CommonConstructor(filter, id);
	}

	/**
	 *
	 */
	virtual ~Class()
	{

	}

	virtual bool RegisterProperties(const Properties& properties)
	{
		return Elementary< T >::RegisterProperties(properties);
	}

	/**
	 * Using the PeriodicTable, we can reliably implement Wave::GetProperties without having to store mProperties internally. <br />
	 * NOTE: You must still record the Properties of T elsewhere. See Elementary.h for an easy means of doing this. <br />
	 * @return the Properties of T that have been Registered with the PeriodicTable. <br />
	 */
	virtual Properties GetProperties() const
	{
		return SafelyAccess<PeriodicTable>()->GetPropertiesOf< T >();
	}

	/**
	 * From Wave. See that class for details. <br />
	 * @return this as an Atom.
	 */
	virtual Atom* AsAtom()
	{
		return this;
	}

	/**
	 * From Wave. See that class for details. <br />
	 * @return this as an Atom.
	 */
	virtual const Atom* AsAtom() const
	{
		return this;
	}

	/**
	 * Disambiguate Wave method. See that class for details. <br />
	 * @param other
	 * @return result of Wave::Attenuation.
	 */
	virtual Code Attenuate(const physical::Wave* other)
	{
		return physical::Class< T >::Attenuate(other);
	}

	/**
	 * Disambiguate Wave method. See that class for details. <br />
	 * @param other
	 * @return result of Wave::Disattenuation.
	 */
	virtual Code Disattenuate(const physical::Wave* other)
	{
		return physical::Class< T >::Disattenuate(other);
	}
};

} //chemical namespace
} //bio namespace
