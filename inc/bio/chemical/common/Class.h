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
#include "bio/chemical/common/Cast.h"
#include "bio/log/Writer.h"
#include "bio/chemical/Atom.h"
#include "SymmetryTypes.h"

namespace bio {
namespace chemical {

class Substance;

/**
 * A chemical::Class is an Identifiable Atom with logging capabilities.
 * This will FormBond() with the provided T.
 * Class in other namespaces will grow to include more complex, templated logic.
 * This pattern prevents you from having to define virtual methods each of your child classes, so long as you always derive from the appropriate Class<T>.
 * @tparam T
 */
template < typename T >
class Class :
	public physical::Class< T >,
	virtual public physical::Identifiable< StandardDimension >,
	virtual public log::Writer,
	virtual public Atom
{
private:
	void CtorCommon(Filter filter = filter::Default())
	{
		if (filter != filter::Default())
		{
			//Skip log::Writer::Initialize, since we don't have a log::Engine atm.
			Filterable::Initialize(filter);
		}
		//Bond the class we're given, Virtually.
		physical::Class< T >::m_object->FormBond(
			physical::Class< T >::m_object,
			bond_type::Virtual());
	}

public:
	/**
	 * Ensure virtual methods point to Class implementations.
	 */
	BIO_DISAMBIGUATE_CLASS_METHODS(physical,
		T)

	/**
	 * For when we know the Perspective but not ourselves.
	 * @param object
	 * @param perspective
	 * @param filter
	 * @param symmetryType
	 */
	Class(
		T* object,
		physical::Perspective< StandardDimension >* perspective = NULL,
		Filter filter = filter::Default(),
		SymmetryType symmetryType = symmetry_type::Object())
		:
		physical::Class< T >(
			object,
			new physical::Symmetry(
				TypeName< T >().c_str(),
				symmetryType
			)
		)
	{
		CtorCommon(filter);
	}


	/**
	 * @param object
	 * @param name
	 * @param perspective
	 * @param filter
	 */
	Class(
		T* object,
		Name name,
		physical::Perspective< StandardDimension >* perspective = NULL,
		Filter filter = filter::Default(),
		SymmetryType symmetryType = symmetry_type::Object())
		:
		physical::Class< T >(
			object,
			new physical::Symmetry(
				TypeName< T >().c_str(),
				symmetryType
			)
		)
	{
		CtorCommon(filter);

		if (perspective)
		{
			physical::Identifiable< StandardDimension >::Initialize(
				name,
				perspective
			);
		}
		else
		{
			SetName(name);
		}
	}

	/**
	 * @param object
	 * @param id
	 * @param perspective
	 * @param filter
	 */
	Class(
		T* object,
		StandardDimension id,
		physical::Perspective< StandardDimension >* perspective = NULL,
		Filter filter = filter::Default(),
		SymmetryType symmetryType = symmetry_type::Object())
		:
		physical::Class< T >(
			object,
			new physical::Symmetry(
				TypeName< T >().c_str(),
				symmetryType
			)
		)
	{
		CtorCommon(filter);

		if (perspective)
		{
			physical::Identifiable< StandardDimension >::Initialize(
				id,
				perspective
			);
		}
		else
		{
			SetId(id);
		}
	}

	/**
	 * Using the PeriodicTable, we can reliably implement Wave::GetProperties without having to store m_properties internally.
	 * NOTE: You must still record the Properties of T elsewhere. See Element.h for an easy means of doing this.
	 * @return the Properties of T that have been Registered with the PeriodicTable.
	 */
	virtual Properties GetProperties() const
	{
		return PeriodicTable::Instance().GetPropertiesOf< T >();
	}

	/**
	 *
	 */
	virtual ~Class()
	{

	}

	/**
	 * From Wave. See that class for details.
	 * @return this as an Atom.
	 */
	virtual Atom* AsAtom()
	{
		return this;
	}

	/**
	 * From Wave. See that class for details.
	 * @return this as an Atom.
	 */
	virtual const Atom* AsAtom() const
	{
		return this;
	}

	/**
	 * Disambiguate Wave method. See that class for details.
	 * @param other
	 * @return result of Wave::Attenuation.
	 */
	virtual Code Attenuate(const physical::Wave* other)
	{
		return physical::Class< T >::Attenuate(other);
	}

	/**
	 * Disambiguate Wave method. See that class for details.
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
