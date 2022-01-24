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

namespace bio {
namespace molecular {

/**
 * An EnvironmentDependent object is one that changes how it behaves based on where it exists.
 * This class assumes there is only ever 1 greater environment directly above the inheriting class.
 * For example, a Protein is dependent on the Cell in which it resides and a Cell is dependent on the Tissue in which it resides. However, no where is a Protein dependent on multiple Cells directly nor is any Cell dependent on more than 1 Tissue at a time. Instead, these EnvironmentDependent classes rely on their greater context to move between themselves and further contexts, whatever resources they need or produce. In other words, the immediate m_environment is a pivot point into the larger system & the larger system is often tiered, having an even larger system "above" it.
 * @tparam T what *this is dependent on (will store a T* as m_environment).
 */
template <Typename T>
class EnvironmentDependent
{
public:
	/**
	 * @param environment
	 */
	EnvironmentDependent(T* environment = NULL) :
		m_environment(environment)
	{

	}

	/**
	 *
	 */
	virtual ~Environment()
	{

	}

	/**
	 * @return The m_environment.
	 */
	virtual T* GetEnvironment()
	{
		return m_environment;
	}

	/**
	 * @return The m_environment as const.
	 */
	virtual const T* GetEnvironment() const
	{
		return m_environment;
	}

	/**
	 * Sets m_environment.
	 * Don't let the environment go out of scope or be deleted before *this!
	 * @param environment
	 */
	virtual void SetEnvironment(T* environment)
	{
		m_environment = environment;
	}

protected:
	T* m_environment;
};

} //molecular namespace
} //bio namespace