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
 * Elementary<> classes are used to create Elements in the PeriodicTable. <br />
 * When constructing an Elementary<>, you must provide its Properties. <br />
 * Properties act as feature flags and can hint at what a Wave* might really be; check out physical/common/Types.h for a more thorough explanation of Properties. <br />
 * <br />
 * In traditional inheritance, a child knows its parents but parents do not know their children. By storing the properties of a type in a virtual method which utilizes the furthest-derived child class, a parent can probe for what other classes its vtable is likely to include. <br />
 * This system is intentionally left imperfect. We could, for example, simply store the names of all derived classes as properties and parents would thus know all of their children. However, what we want to create is a system that separates interfaces from implementation. <br />
 * To illustrate this, imagine an abstract base class which has some Property: "X". We call this abstract base our "interface" as it defines the methods we want to call. We then create an implementation class which is not abstract and which defines the methods in the abstract base. The implementation class should also have the Property "X". Now, imagine this implementation is Bonded to an Atom. The Atom can call GetProperties() on the Bonded Wave* and see that it has the Property "X". Knowing this, the Atom can cast the Wave* as our abstract base and call some desired method. It does not matter how that method is implemented; it only matters what methods are available. Hence, we do not need to store the names of implementers in our Properties. <br />
 * <br />
 * We call the system created by Properties "inverted inheritance", since we can safely downcast to our interfaces without knowing what else the Wave* is. <br />
 * <br />
 * NOTE: when using multiple Elementary classes which implement the same interface in different ways, it is best to Covalent<>ly inherit them, which will cause the GetProperties method of each to resolve to the respective interface and will not cause conflicts in implementation. <br />
 * <br />
 * When to use: <br />
 * You have an upstream class (e.g. AbstractMotif) that which defines some (possibly abstract) methods and which will be non-virtually inherited by downstream classes (e.g. LinearMotif). You want to be able to call the downstream implementation of the upstream methods from a naive parent of the upstream class (e.g. Wave*). You want to do this in a way which does not force each downstream class to disambiguate (override) each upstream method (e.g. not putting AbstractMotif methods in Wave). <br />
 * <br />
 * How to use: <br />
 * Proper usage of Elementary depends on certainly the PeriodicTable and often Atom as well. <br />
 * If you derive from chemical::Class<>, you are already deriving from Elementary; otherwise, simply derive from *this publicly or protectedly. <br />
 * On instantiation (or later), call RegisterProperties() with the Properties of your class. KEEP IN MIND: RegisterProperties can only be called once per class type. <br />
 * Once Properties have been Registered, they will appear in the PeriodicTable and will be returned by GetProperties(). This will make your class Resonate with other classes which share the same Properties and will allow safe downcasting from Wave*. <br />
 * After all that, Atom::Attenuate() and other such methods can be used to propagate Excitations, etc. to your class based on their Resonance. <br />
 *
 * @tparam T
 */
template < typename T >
class Elementary
{
public:
	/**
	 * Records the given Properties as those for the type T within the PeriodicTable. <br />
	 * Also ensures we only Record the Properties of T once. They should always be the same. <br />
	 * @param properties
	 */
	virtual bool RegisterProperties(const Properties& properties)
	{
		BIO_SANITIZE_AT_SAFETY_LEVEL_1(properties.Size(),,return false)
		static bool canRegister = true;
		BIO_SANITIZE_AT_SAFETY_LEVEL_1(canRegister,,return false)
		canRegister = false;
		return SafelyAccess<PeriodicTable>()->RecordPropertiesOf< T >(properties);
	}

	/**
	 * Make sure *this is Associated in the PeriodicTable's registry. <br />
	 * This should almost always be a nop and might be more wasteful than useful, so if there's a better way, we should explore that. <br />
	 * Where this really comes in handy is in enabling LinearMotif::CreateImplementation(), which requires that the CONTENT_TYPE have a valid type Associated in the PeriodicTable. <br />
	 * For example, if we want to GetOrCreateByName< Cell >("My Cell"), we have to have a valid Cell stored in the PeriodicTable. <br />
	 * So, we either do this and Associate a new T with the PeriodicTable here or manage the types in the PeriodicTable through some external system. <br />
	 * Because the minimum requirements for LinearMotif's CONTENT_TYPE are only being a ChemicalClass<>, and anything else that deals with the PeriodicTable will likely be dealing with Substances or beyond, we've chosen to put this here. <br />
	 * Ultimately, the cost of running if(!(!NULL)) on every class instantiation is worth the ease of use provided by automatic type registration. <br />
	 * Records *this as the archetypal Wave for the id of T. <br />
	 * @param force
	 */
	void RegisterType()
	{
		static T* archetype = NULL;
		if (!archetype)
		{
			archetype = new T(); //will be deleted by PeriodicTable.
			SafelyAccess< PeriodicTable >()->template AssociateType< T >(archetype->AsWave());
		}
	}

	/**
	 * @param properties
	 */
	Elementary(const Properties properties)
	{
		RegisterProperties(properties);
		RegisterType(force);
	}

	/**
	 * If you don't know the class Properties at time of construction, you can use this. <br />
	 * Though RegisterProperties should be as close to, if not directly, a constexpr and should be known even before *this is constructed. <br />
	 */
	Elementary()
	{
		RegisterType();
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
