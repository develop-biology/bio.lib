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

#include "bio/molecular/common/Types.h"
#include "bio/molecular/common/Class.h"
#include "bio/molecular/common/Filters.h"
#include "Surface.h"
#include "bio/chemical/structure/LinearStructuralComponent.h"

namespace bio {
namespace molecular {

/**
 * As a combination of relativistic Waves and chemical Bonds, Molecules are rather complicated. They're kinda like viewing particle physics from mathematical topology.
 * I'm not sure I have the english words to describe this view of the world adequately, so looking at the code might be better.
 * Still, I'll try to describe the idea as an imagination exercise:
 *
 * Imagine a Quantum Wave (aka a built-in type, like bool or int) as a sphere. These quantum balls are the maximum resolution we can see any Molecule through (it doesn't get any more basic than built-in types).
 * Next, imagine a collection of these spheres as some kind of glob. This represents a collection of basic types and would be akin to a class or struct. However, these globs can take whatever form we want and do not have to map into any hard-coded structure. We call these globular constructs a Surface.
 * Most of the time, Surfaces will be singular globs, with a 1 to 1 Surface to primitive type correspondence. However, they can be arbitrarily knobby.
 * Now imagine 10 or so of these Surfaces all stuck together as a Molecule. So, a Molecule is just a big globby mass.
 * Surfaces do not (usually) overlap. They are, instead,  distinct, identifiable (Identifiable<StandardDimension>) regions on a Molecule.
 * Imagine identical 2 Molecules that have 1 globby collection (Surface) that differs between them. That kind of comparative difference helps us Identify the Surfaces from the otherwise indistinguishable mass of globs.
 * Next, imagine 2 Molecules bumping into each other so that 1 or more of their Surfaces touch.
 * When Surfaces interact in this manner, they can change each other. In computer science, we can just conjure change whenever, however, and wherever we want. However, in the real world, the idea that matter / energy is conserved requires that matter / energy be moved between the interacting surfaces in order to effect change.
 * This same concept has been modeled here (see Transfer..., etc., below); though we can still do whatever the **** we want cause computers.
 *
 * You can create Surfaces in 1 Molecule and then Transfer them to another. However, doing so may change (or break) the Molecules' interactions with other Molecules and systems. This would be like transferring variables between objects at runtime.
 * This Transfer system is why Molecules are Perspectives. The Id of Surfaces in 1 Molecule may not hold within another Molecule and we don't want to enforce a global Surface labeling at this time.
 * You can think of Surface::Ids as numbered variables. When an class (Molecule) is instantiated, we go through and number its member variables. Members can then be added or removed throughout the life of the object and each one will have a number that is unique to that Molecule. In other words, there is no absolutely right number for "MyVar"; instead, "MyVar" can be an int in one Molecule and a bool in another.
 *
 * The ability to Transfer Surfaces between Molecules is just one advantage that comes from this member abstraction. Another advantage is Symmetry (i.e. reflection into other languages, like json). Beyond this point, Spin() and Reify(), which are native to physical::Waves, should no longer require definition, as we will be able to use the Biology structures we've created to determine those implementations dynamically.
 *
 * Lastly, Surfaces also give us the ability to store methods via Proteins. See Protein.h for more on that.
 *
 * In order realize the behavior described above, we have to employ a non-native, Biology-specific syntax. In short, Define<>() creates a member and RotateTo gets a member.
 * For example:
 * MyType myVar = SomeFunction(); => Define<MyType>("myVar", SomeFunction());
 * MyType another = myVar; => Define<MyType>("another", RotateTo("myVar"));
 * Of course, in order to avoid slower string lookups, you can cache the Id of "myVar" within a Molecule by calling GetIdFromName("myVar"), which can then be used with RotateTo(...).
 *
 * You can think of members as globs on the surface of our Molecule and we rotate this globby mass until the side we want is facing us. This allows us to interact with our desired Surface.
 * In order to set a member, you would do something like:
 * RotateTo("myVar")->operator=(*RotateTo("another"));
 * or, more simply,
 * (*RotateTo("myVar")) = (*RotateTo("another"))
 * NOTE: if "myVar" or "another" don't exist in the Molecule, this will segfault. To avoid crashing, you may want to use BIO_SANITIZE() (from bio/common/Macros.h)
 * This can be done because Surfaces can be treated as the values they are Bonded to.
 *
 * Lastly, if you want to create a Molecule from a hard-coded data structure, you can call Use() instead of Define<>(), which will create a Surface from a (member) pointer. Surfaces created with Use will not be Transferable.
 */
class Molecule :
	public Class< Molecule >,
	public physical::Perspective< StandardDimension >,
	public chemical::LinearStructuralComponent< Surface >
{
public:

	/**
	 * Ensure virtual methods point to Class implementations.
	 */
	BIO_DISAMBIGUATE_CLASS_METHODS(molecular,
		Molecule)

	/**
	 * Standard ctors.
	 */
	 BIO_DEFAULT_IDENTIFIABLE_CONSTRUCTORS(molecular,
		Molecule,
		&MoleculePerspective::Instance(),
		filter::Molecular())

	/**
	 * Copying a Molecule will duplicate all Surfaces of toCopy.
	 * @param toCopy
	 */
	Molecule(const Molecule& toCopy);

	/**
	 *
	 */
	virtual ~Molecule();

	/**
	 * Create a Surface from an existing (member) variable.
	 * Will fail if varName already exists within *this.
	 * @tparam T
	 * @param varName
	 * @param varPtr
	 * @return the Id of the Surface created or InvalidId().
	 */
	template < typename T >
	StandardDimension Use(
		Name varName,
		T* varPtr
	)
	{
		BIO_SANITIZE(!RotateTo(varName), ,
			return InvalidId());
		Surface* toAdd = new Surface(
			varName,
			this
		);
		toAdd->Use(varPtr);
		Add< Surface* >(toAdd);
		return toAdd->GetId();
	}

	/**
	 * Create a new variable within *this, the Biology way.
	 * Will fail if varName already exists within *this.
	 * @tparam T
	 * @param varName
	 * @return the Id of the Surface created or InvalidId().
	 */
	template < typename T >
	StandardDimension Define(Name varName)
	{
		BIO_SANITIZE(!RotateTo(varName), ,
			return InvalidId());
		Surface* toAdd = new Surface(
			varName,
			this
		);
		toAdd->Manage(new T());
		Add< Surface* >(toAdd);
		return toAdd->GetId();
	}

	/**
	 * Create a new variable within *this, the Biology way.
	 * Assigns the new variable to the value provided.
	 * Will fail if varName already exists within *this.
	 * @tparam T
	 * @param varName
	 * @param assignment
	 * @return the Id of the Surface created or InvalidId().
	 */
	template < typename T >
	StandardDimension Define(
		Name varName,
		const T& assignment
	)
	{
		BIO_SANITIZE(!RotateTo(varName), ,
			return InvalidId());
		Surface* toAdd = new Surface(
			varName,
			this
		);
		toAdd->Manage(new T(assignment));
		Add< Surface* >(toAdd);
		return toAdd->GetId();
	}

	/**
	 * Get a variable from within *this.
	 * Use for getting and/or setting.
	 * REMINDER: use this->GetIdFromName("someName") to get the Id of a Surface with the given Name.
	 * @param surfaceId
	 * @return a Surface with the given Id, as defined by *this, or NULL.
	 */
	virtual Surface* RotateTo(StandardDimension surfaceId);

	/**
	 * const version of RotateTo
	 * @param surfaceId
	 * @return a const Surface with the given Id, as defined by *this, or NULL.
	 */
	virtual const Surface* RotateTo(StandardDimension surfaceId) const;

	/**
 	* Get a variable from within *this.
	 * Use for getting and/or setting.
	 * @param surfaceName
	 * @return a Surface with the given Name from *this.
	 */
	virtual Surface* RotateTo(Name surfaceName);

	/**
	 * const version of RotateTo
	 * @param surfaceName
	 * @return a const Surface with the given Name from *this.
	 */
	virtual const Surface* RotateTo(Name surfaceName) const;

	/**
	 * Copy a Surface from another Molecule into *this.
	 * @param source
	 * @param surface
	 * @return whether or not the operation succeeded.
	 */
	virtual bool DuplicateFrom(
		Molecule* source,
		Name surface
	);

	/**
	 * Move a Surface from another Molecule into *this.
	 * @param source
	 * @param surface
	 * @return whether or not the operation succeeded.
	 */
	virtual bool TransferFrom(
		Molecule* source,
		Name surface
	);

	/**
	 * Required method from Wave. See that class for details.
	 * @return a Symmetrical image of *this
	 */
	virtual physical::Symmetry* Spin() const;

	/**
	 * Required method from Wave. See that class for details.
	 * Reconstruct *this from the given Symmetry.
	 * @param symmetry
	 */
	virtual Code Reify(physical::Symmetry* symmetry);

	/**
	 * operator wrappers around RotateTo
	 * @param surfaceId
	 * @return RotateTo(...)
	 * @{
	 */
	virtual Surface* operator[](StandardDimension surfaceId);

	virtual const Surface* operator[](StandardDimension surfaceId) const;

	virtual Surface* operator[](Name name);

	virtual const Surface* operator[](Name name) const;
	/**@}*/

	/**
	 * ease-of-use operators for Rotating to a Protein and Activating it (i.e. a Biology-style function call).
	 * @param ProteinId
	 * @return RotateTo(...)
	 * @{
	 */
	virtual Code operator()(StandardDimension ProteinId);

	virtual Code operator()(Name name);
	/**@}*/

	/**
	 * Surface copy operation
	 * Copies a Surface onto *this.
	 * @param source
	 * @return this
	 */
	virtual Molecule* operator<<(Surface* source);

	/**
	 * Surface copy operation
	 * Copies *this into a Surface
	 * @param target
	 * @return target
	 */
	virtual Surface* operator>>(Surface* target);

	/**
	 * Surface move operation
	 * Adds a Surface onto *this directly, does not Clone() the source.
	 * @param source
	 * @return this
	 */
	virtual Molecule* operator<<=(Surface* source);

	/**
	 * Surface move operation
	 * Adds *this into a Surface, does not Clone() *this.
	 * @param target
	 * @return target
	 */
	virtual Surface* operator>>=(Surface* target);

	/**
	 * Molecule copy operation
	 * Duplicates all Surfaces on the source Molecule onto *this.
	 * @param source
	 * @return this
	 */
	virtual Molecule* operator<<(Molecule* source);

	/**
	 * Molecule copy operation
	 * Duplicates all Surfaces on *this onto the target Molecule.
	 * @param target
	 * @return target
	 */
	virtual Molecule* operator>>(Molecule* target);

	/**
	 * Molecule move operation
	 * Transfers all Surfaces on the source Molecule onto *this.
	 * @param source
	 * @return this
	 */
	virtual Molecule* operator<<=(Molecule* source);

	/**
	 * Molecule move operation
	 * Transfers all Surfaces on *this onto the target Molecule.
	 * @param source
	 * @return target
	 */
	virtual Molecule* operator>>=(Molecule* target);
};


} //molecular namespace
} //bio namespace
