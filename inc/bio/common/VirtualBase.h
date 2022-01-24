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

#include "Types.h"

namespace bio {

/**
 * NOTE: inheriting from VirtualBase is only necessary if your class will be virtually inherited and implements non-default constructors.
 *
 * C++ has a problem: virtually inherited classes are instantiated by the most derived class, bypassing all other initializers. Because we want our most derived classes to be simpler, not more complex, we either have to rely on a chaotic mesh of last-to-write setter calls in intermediate class constructors or do what we've done here.
 * VirtualBase does 1 thing and that is to fix the problem above. Here's how we do this:
 * First, the child class of VirtualBase should PROTECTEDLY inherit VirtualBase (i.e class MyClass : protected VirtualBase).
 * Next, that child should implement InitializeImplementation(), taking each arg as if it were provided to the class constructor.
 * Lastly, children of the child class, which virtually inherit this child of VirtualBase, should call ChildClass::Initialize(args), where ChildClass is the name of the child class and args are all of the arguments necessary to initialize the child.
 * NOTE: unlike normal c++ constructors, no args can be left out of the Initialize args list and both order and type must be perfectly aligned with what the child class expects.
 * Unfortunately, because all this happens at run time, there are no compile-time type checks.
 *
 * When multiple child classes all call Initialize on a virtually inherited parent, only the FIRST call will take effect. All others will be ignored (but you can fall back on chaotic setter calls to overcome this). By inverting the way virtually inherited construction works, we can rely on passing arguments to a base class in order to virtual base class, restoring normal derived->base initialization behavior.
 *
 * To illustrate how this works:
 * [furthest derived class] ----> [virtual base default ctor] (implicit)
 * [furthest derived class] ----> [parent class 1 ctor] ----> [grandparent 1 ctor]
 * [furthest derived class] ----> [parent class 2 ctor] ----> [(virtual base)::Initialize()]
 * [furthest derived class] ----> [parent class 3 ctor] ----> [(virtual base)::Initialize()]
 * In this example, only "parent class 2" gets to Initialize the virtual base.
 * "Parent class 3" does not get to Initialize the virtual base.
 * "Furthest derived class" elects "parent class 2" as the Initializer based on it's inheritance order ("parent class 2" comes first).
 * If we change up the order and have "grandparent 1" call Initialize, we'll have to make sure "parent class 1" passes the appropriate arguments to the "grandparent 1" ctor.
 * Passing arguments to the Initializing parent is thus the key to making this system work without needing to have the "furthest derived class" include the virtual base in its initializer list.
 */
class VirtualBase
{
protected:
	/**
	 *
	 */
	VirtualBase();

	/**
	 *
	 */
	virtual ~VirtualBase();

	/**
	 * Calls InitializeImplementation once and then becomes a nop.
	 * @param args
	 */
	void Initialize(ByteStreams args);

	/**
	 * Ease-of-use calls for wrapping c++98 vector initialization.
	 * Calls Initialize(ByteStreams).
	 * @param arg1
	 */
	void Initialize(ByteStream arg1);

	/**
	 * Ease-of-use calls for wrapping c++98 vector initialization.
	 * Calls Initialize(ByteStreams).
	 * @param arg1
	 * @param arg2
	 */
	void Initialize(ByteStream arg1, ByteStream arg2);

	/**
	 * Ease-of-use calls for wrapping c++98 vector initialization.
	 * Calls Initialize(ByteStreams).
	 * @param arg1
	 * @param arg2
	 * @param arg3
	 */
	void Initialize(ByteStream arg1, ByteStream arg2, ByteStream arg3);

	/**
	 * Your initialization logic goes here.
	 * @param args
	 */
	virtual void InitializeImplementation(ByteStreams args) = 0;

	/**
	 * false until Initialize is called, then true forever more.
	 * This could be private but is left accessible to children in case hacks become necessary.
	 */
	bool m_hasBeenInitialized;
};

} //bio namespace