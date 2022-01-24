/*
 * This file is a part of the Biology project by eons LLC.
 * Biology (aka Develop Biology) is a framework for approaching software
 * development from a natural sciences perspective.
 *
 * Copyright (C) 2021 Séon O'Shannon & eons LLC
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

#include "bio/physical/Class.h"
#include "bio/physical/Filters.h"
#include "bio/log/Writer.h"
#include "Atom.h"

namespace bio {
namespace chemical {

/**
 * A chemical::Class is an Atom in addition to being a physical::Wave with logging capabilities.
 * This will FormBond(m_object).
 * Class in other namespaces will grow to include more complex, templated logic.
 * This pattern prevents you from having to define virtual methods each of your child classes, so long as you always derive from the appropriate Class<T>.
 * @tparam T
 */
template <typename T>
class Class :
	public physical::Class<T>,
	virtual public log::Writer,
	virtual public Atom
{
public:
	/**
	 * @param object
	 * @param symmetry
	 */
	Class(T* object, Filter filter = filter::Default())
		:
		physical::Class<T>(
			object,
			new Symmetry(
				TypeName<T>(),
				symmetry_Type::Object()))
	{
		if (filter != filter::Default)
		{
			//Skip log::Writer::Initialize, since we don't have a log::Engine atm.
			Filterable::Initialize(filter);
		}
		//Bond the class we're given, Virtually.
		m_object->FormBond(
			m_object,
			bond_type::Virtual());
	}

	/**
	 * Using the PeriodicTable, we can reliably implement Wave::GetProperties without having to store m_properties internally.
	 * NOTE: You must still record the Properties of T elsewhere. See Element.h for an easy means of doing this.
	 * @return the Properties of T that have been Registered with the PeriodicTable.
	 */
	virtual Properties GetProperties() const {
		return PeriodicTable::Instance().GetPropertiesOf<T>();
	}

	/**
	 *
	 */
	virtual ~Class()
	{

	}
};

} //chemical namespace
} //bio namespace