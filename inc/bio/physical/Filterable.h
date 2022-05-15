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

//Filterable is a base class for all classes that need to use logs.

#include "bio/physical/common/Types.h"
#include "bio/physical/common/Class.h"
#include "bio/common/VirtualBase.h"

namespace bio {
namespace physical {

/**
 * Filterable objects have a Filter. <br />
 * NOTE: Filterable objects only use a single Filter at a time. <br />
 * See log::Writer for example usage. <br />
 */
class Filterable :
	public physical::Class< Filterable >,
	protected VirtualBase
{
public:

	/**
	 * Ensure virtual methods point to Class implementations. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(physical,
		Filterable)


	/**
	 *
	 */
	Filterable();

	/**
	 * @param filter
	 */
	Filterable(Filter filter);

	/**
	 *
	 */
	virtual ~Filterable();

	/**
	 * VirtualBase required method. See that class for details (in common/) <br />
	 * @param args
	 */
	virtual void InitializeImplementation(ByteStreams args);

	/**
	 * Set the filter for *this. <br />
	 * Overload this to propagate Filter changes to own classes, etc. <br />
	 * @param filter
	 */
	virtual void SetFilter(Filter filter);

	/**
	 * @return the Filter used by *this.
	 */
	Filter GetFilter() const;

	/**
	 * Required method from Wave. See that class for details. <br />
	 * @return a Symmetrical image of *this
	 */
	virtual Symmetry* Spin() const;

	/**
	 * Required method from Wave. See that class for details. <br />
	 * Reconstruct *this from the given Symmetry. <br />
	 * @param symmetry
	 */
	virtual Code Reify(Symmetry* symmetry);

protected:
	Filter mFilter;
};
} //physical namespace
} //bio namespace
