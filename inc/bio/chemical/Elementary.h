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

#include "PeriodicTable.h"

namespace bio {
namespace chemical {

/**
 * Elements are used to make entries in the PeriodicTable. <br />
 * When constructing an Elementary<>, you must provide its Properties. <br />
 *
 * When to use: <br />
 * You have a class that implements an interface and/or an interface which will be non-virtually inherited by downstream classes and you want those downstream classes to be able to call a method for each of your inherited classes without having to force the child classes to disambiguate each parent instance in a virtual override of your class method. <br />
 *
 * How to use: <br />
 * 1. Define some abstract base class with the methods you wish to call (e.g. AbstractMotif, with ImportImplementation). <br />
 * 2. Add a static Properties GetClassProperties() method to the abstract base which returns some unique Properties for the feature you're building (NOTE: this should also be the return value of the virtual physical::Wave::GetProperties() method). <br />
 * 3. Define however many implementations of that abstract base as you would like (e.g. templated children, like StructuralComponentImplementation). <br />
 * 4. Define an interface class that will call the appropriate interface methods (e.g. Structure). <br />
 * 5. Define high-level wrappers around the interface(s) and interface with virtual inheritance for the interface (e.g. UnorderedMotif). <br />
 * 6. Make your wrapper classes derive from Elementary with CRTP (provide the wrapper class as T for Elementary<T>) and provide your base class GetClassProperties() to the Elementary constructor. <br />
 *
 * Once all that is done and you utilize Atom Bonding methods (or derive from chemical::Class) you will have essentially inverted the inheritance tree, allowing base classes to call what would be derived class methods without the derived classes having to implement those methods. <br />
 * To elaborate, if you have a class that derives from multiple high-level wrappers, under normal inheritance, you would have to create a MyImplementation() override method of the abstract base class method which calls all the different parent class methods (e.g. operator== comparing all parent equivalencies). If you do not do this, the method becomes ambiguous because each wrapper provides an equally valid interface. However, by recording which wrappers have the properties of the abstract base, you can call each wrapper's interface of a base class method by casting the wrapper to the base and calling the virtual function (the base is NOT virtually inherited). <br />
 * The reason Elementary must be used on the wrapper and not on your multi-wrapper child is that doing so would create multiple eligible Bonds within an Atom, some of which would have ambiguous definitions, as mentioned. <br />
 *
 * @tparam T
 */
template < typename T >
class Elementary
{
public:

	/**
	 * Ensure we only Record the Properties of T once. They should always be the same. <br />
	 */
	static bool hasRegistered;

	/**
	 * Records the given Properties as those for the type T within the PeriodicTable. <br />
	 * @param properties
	 */
	void RegisterProperties(const Properties properties)
	{
		if (!hasRegistered)
		{
			PeriodicTable::Instance().RecordPropertiesOf< T >(properties);

		}
	}

	/**
	 * @param properties
	 */
	Elementary(const Properties properties)
	{
		RegisterProperties(properties);
	}

	/**
	 * If you don't know the class Properties at time of construction, you can use this. <br />
	 * Though RegisterProperties should be as close to, if not directly, a constexpr and should be known even before *this is constructed. <br />
	 */
	Elementary()
	{

	}


	/**
	 *
	 */
	virtual ~Elementary()
	{

	}

};

} //chemical namespace
} //bio namespace
