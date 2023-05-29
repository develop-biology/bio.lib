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

#include "StemCell.h"

namespace bio {
namespace neural {

class Neurite:
	public neural::Class< Neurite >,
	public StemCell
{
public:

	/**
	 * Disambiguate all class methods. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(neural, Neurite)

	BIO_DEFAULT_IDENTIFIABLE_CONSTRUCTORS(
		neural,
		Neurite,
		filter::Neural()
	)

	/**
	 *
	 */
	virtual ~Neurite();

	/**
	 * Use this method to populate any member variable Protein*s. <br />
	 * You'll want to do this to speed up your code by bypassing the dynamic execution provided by genetic::Expressor. <br />
	 */
	virtual Code CacheProteins();

	/**
	 * If you use CacheProteins, you'll likely want to create your default Proteins here. <br />
	 * This will prevent dereferencing null or garbage pointers when using your cached Proteins. <br />
	 */
	virtual Code CreateDefaultProteins();
	
	/**
	 * PROTEIN BASED. <br />
	 * Override this method to RETURN whether or not your Neurite's value is equivalent to zero (e.g. zero vector, empty set, etc.) <br />
	 * If using an unprocessed -> processed scheme, this refers to the unprocessed value. <br />
	 * This is used by ShouldBeProcessed <br />
	 * @return true if the value of *this is zero, false otherwise.
	 */
	virtual bool IsZero() const;

	/**
	 * PROTEIN BASED. <br />
	 * Used for Depotentiate signals, where it is useful to forget what the processed data was. <br />
	 */
	virtual void MakeZero();

protected:
	molecular::Protein* mcIsZero;
	molecular::Protein* mcMakeZero;
};

} // neural namespace
} // bio namespace
