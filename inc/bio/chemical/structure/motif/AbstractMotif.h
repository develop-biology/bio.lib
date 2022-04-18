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

#include "bio/chemical/common/Types.h"
#include "bio/common/container/Container.h"

namespace bio {
namespace chemical {

class AbstractMotif
{
public:

	/**
	 * See physical::Wave for more info on Resonance and how to use Properties. <br />
	 * @return {property::Structural()}
	 */
	static Properties GetClassProperties();

	/**
	 *
	 */
	AbstractMotif();

	/**
	 *
	 */
	virtual ~AbstractMotif();

	/**
	 * Implementation for accessing all Contents. <br />
	 * @return all Contents in *this.
	 */
	virtual Container* GetAllImplementation(); 

	/**
	 * Const interface for accessing all Contents. <br />
	 * @return all Contents in *this.
	 */
	virtual const Container* GetAllImplementation() const; 

	/**
	 * Clears the contents of *this. <br />
	 */
	virtual void ClearImplementation()
	{
		//nop
	}

	/**
	 * Implementation for counting Contents. <br />
	 * @return the number of Contents in *this.
	 */
	virtual unsigned long GetCountImplementation() const
	{
		return 0;
	}

	/**
	 * Copy the contents of another container into *this. <br />
	 * void* used to avoid ambiguous implicit casting. 
	 * @param other
	 */
	virtual void ImportImplementation(const void* other) 
	{
		//nop
	}

	/**
	 * Get the Contents of *this as a string. <br />
	 * @param separator e.g. ", ", the default, or just " ".
	 * @return the Contents of *this as a string.
	 */
	virtual std::string GetStringFromImplementation(std::string separator = ", ")
	{
		return "";
	}

protected:
	mutable Container* m_contents; 
};

} //chemical namespace
} //bio namespace
