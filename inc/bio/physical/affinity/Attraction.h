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

#include "bio/physical/common/Types.h"
#include "bio/physical/common/Class.h"
#include "bio/physical/macro/Macros.h"

namespace bio {
namespace physical {

/**
 * physical::Attraction is essentially a key value pair between 2 objects in the same DIMENSION. <br />
 * (See relativity/Perspective.h for an explanation of DIMENSION). <br />
 * The Key to Attraction is the DIMENSION; the value is the strength of the Attraction. <br />
 * <br />
 * To create repulsion, simply set a negative Strength. <br />
 */
template < typename DIMENSION >
class Attraction:
	public physical::Class< Attraction< DIMENSION > >
{
public:
	
	typedef double Force;
	
	Attraction(
		const DIMENSION& dimension = 0, //Invalid Id across DIMENSIONS
		const Force strength = 0.0
	):
		physical::Class< Attraction< DIMENSION>>(
			this,
			NULL
		),
		m_dimension(dimension),
		m_strength(strength)
	{}
	
	/**
	 * Set the Force of *this. <br />
	 * @param strength 
	 */
	void SetForce(const Force strength)
	{
		m_strength = strength;
	}
	
	/**
	 * @return the Force of *this. <br /> 
	 */
	Force GetForce() const
	{
		return m_strength;
	}
	
	/**
	 * Set the DIMENSION of *this. <br />
	 * @param dimension 
	 */
	void SetDimension(const DIMENSION& dimension)
	{
		m_dimension = dimension;
	}
	
	/**
	 * @return the DIMENSION of *this. <br /> 
	 */
	DIMENSION GetDimension() const
	{
		return m_dimension;
	}

	/**
	 * Compare only the DIMENSION, not Strength. <br />
	 * @param other
	 * @return whether the DIMENSION of *this is equal to the DIMENSION of other.
	 */
	bool operator==(const Attraction& other) const
	{
		return m_dimension == other.m_dimension;
	}
	
protected:
	DIMENSION m_dimension;
	Force m_strength;
};

} //physical namespace
} //bio namespace
