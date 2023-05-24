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

#include "bio/molecular/common/Types.h"
#include "bio/molecular/common/Class.h"
#include "bio/molecular/common/Filters.h"
#include "Surface.h"
#include "bio/chemical/structure/motif/DependentMotif.h"
#include "bio/chemical/solution/Solute.h"

namespace bio {
namespace molecular {

/**
 * As a combination of relativistic Waves and chemical Bonds, Molecules are rather complicated. They're kinda like viewing particle physics from mathematical topology. <br />
 * I'm not sure I have the english words to describe this view of the world adequately, so looking at the code might be better. <br />
 * Still, I'll try to describe the idea as an imagination exercise: <br />
 *
 * Imagine a Quantum Wave (aka a built-in type, like bool or int) as a sphere. These quantum balls are Surfaces and represent the maximum resolution we can see any Molecule through (it doesn't get any more basic than built-in types). <br />
 * Next, imagine a collection of these spheres as some kind of glob. This represents a collection of basic types and would be akin to a class or struct. However, these globs can take whatever form we want and do not have to map into any hard-coded structure. We call these globular constructs a Molecule; and, just as structs can contain other structs, a Surface may itself be a Molecule, and on and on.<br />
 * Most of the time, Surfaces will be singular globs, with a 1 to 1 Surface to primitive type correspondence. However, they can be arbitrarily knobby. <br />
 * Now imagine 10 or so of these Surfaces all stuck together as a Molecule. So, a Molecule is just a big globby mass. <br />
 * Surfaces do not (usually) overlap. They are, instead,  distinct, identifiable (Identifiable<Id>) regions on a Molecule. <br />
 * Imagine 2 identical Molecules that have 1 globby collection (Surface) that differs between them. That kind of comparative difference helps us Identify the Surfaces from the otherwise indistinguishable mass of globs. <br />
 * Next, imagine 2 Molecules bumping into each other so that 1 or more of their Surfaces touch. <br />
 * When Surfaces interact in this manner, they can change each other. In computer science, we can just conjure change whenever, however, and wherever we want. However, in the real world, the idea that matter and energy are conserved requires that matter or energy be moved between the interacting surfaces in order to effect change. <br />
 * This same concept has been modeled here (see Transfer..., etc., below); though we can still do whatever the **** we want cause computers. 
 *
 * You can create Surfaces in 1 Molecule and then Transfer them to another. However, doing so may change (or break) the Molecules' interactions with other Molecules and systems. This would be like transferring variables between objects at runtime. <br />
 * We've chosen to rely on global Id <-> Name mappings for all Identifiable< Id > classes including Molecule & Surface. Ids are provided by the IdPerspective for object Names and the PeriodicTable for type names. Relying on globally consistent Ids allows us to inherit from other objects in the same dimension while not losing track of the children's names (e.g. if all Vesicles were to be tracked by a different Perspective than Molecules & we upcast a Vesicle* to a Molecule*, we wouldn't be able to get the Molecule*'s name from the MoleculePerspective because the name would only be stored in the VesivlePerspective). <br />
 * This is nice but comes with a major downside: there is no guarantee that 2 Surfaces of the same Name share have the same structure nor type. <br />
 * You can think of Surface::Ids as numbered variables. When a class (Molecule) is instantiated, we go through and number its member variables. Members can then be added or removed throughout the life of the object, and each one will have a number that is unique to its Name, but not unique to the Molecule it belongs to. In other words, there is no absolutely right type for "MyVar"; instead, "MyVar" can be an int in one Molecule and a bool in another. <br />
 * If you would like to Transfer a Surface to another Molecule, please check that the destination does not already have a Surface with the same Id (or Name). <br />
 * If you would like to combine 2 Surfaces on 2 different Molucules, you can Mix them. See Solute & Solution for examples. <br />
 *
 * The ability to Transfer Surfaces between Molecules is just one advantage that comes from this member abstraction. Another advantage is Symmetry (i.e. reflection into other languages, like json). Beyond this point, Spin() and Reify(), which are native to physical::Waves, should no longer require definition, as we will be able to use the Biology structures we've created to determine those implementations dynamically. <br />
 *
 * Lastly, Surfaces also give us the ability to store methods via Proteins. See Protein.h for more on that. <br />
 *
 * In order realize the behavior described above, we have to employ a non-native, Biology-specific syntax. In short, Define<>() creates a member and RotateTo gets a member. <br />
 * For example: <br />
 * MyType myVar = SomeFunction(); => Define<MyType>("myVar", SomeFunction()); <br />
 * MyType another = myVar; => Define<MyType>("another", RotateTo("myVar")); <br />
 * Of course, in order to avoid slower string lookups, you can cache the Id of "myVar" within a Molecule by calling GetIdFromName("myVar"), which can then be used with RotateTo(...). <br />
 *
 * You can think of members as globs on the surface of our Molecule and we rotate this globby mass until the side we want is facing us. This allows us to interact with our desired Surface. <br />
 * In order to set a member, you would do something like: <br />
 * RotateTo("myVar")->operator=(*RotateTo("another")); <br />
 * or, more simply, <br />
 * (*RotateTo("myVar")) = (*RotateTo("another")) <br />
 * NOTE: if "myVar" or "another" don't exist in the Molecule, this will segfault. To avoid crashing, you may want to use BIO_SANITIZE() (from bio/common/Macros.h) <br />
 * This can be done because Surfaces can be treated as the values they are Bonded to. <br />
 *
 * Lastly, if you want to create a Molecule from a hard-coded data structure, you can call Use() instead of Define<>(), which will create a Surface from a (member) pointer. Surfaces created with Use will not be Transferable. <br />
 */
class Molecule :
	public Class< Molecule >,
	public Covalent< chemical::DependentMotif< Surface*, Molecule* > >,
	virtual public physical::Perspective< Id >
{
private:
	/**
	 *
	 */
	void CommonConstructor();

public:

	/**
	 * Ensure virtual methods point to Class implementations. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(molecular, Molecule)

	/**
	 * Standard constructors. <br />
	 */
	BIO_DEFAULT_IDENTIFIABLE_CONSTRUCTORS_WITH_COMMON_CONSTRUCTOR(
		molecular,
		Molecule,
		filter::Molecular()
	)

	/**
	 * Copying a Molecule will duplicate all Surfaces of toCopy. <br />
	 * @param toCopy
	 */
	Molecule(const Molecule& toCopy);

	/**
	 *
	 */
	virtual ~Molecule();

	/**
	 * Create a Surface from an existing (member) variable. <br />
	 * Will fail if varName already exists within *this. <br />
	 * @tparam T
	 * @param varName
	 * @param varPtr
	 * @return the Id of the Surface created or InvalidId().
	 */
	template < typename T >
	Id Use(
		const Name& varName,
		T* varPtr
	)
	{
		BIO_SANITIZE(!RotateTo(varName), , return InvalidId())
		Surface* toAdd = new Surface(
			varName,
			this
		);
		toAdd->Use(varPtr);
		Add< Surface* >(toAdd);
		return toAdd->GetId();
	}

	/**
	 * Create a new variable within *this, the Biology way. <br />
	 * The created variable will have no type set upon creation. <br />
	 * Will fail if varName already exists within *this. <br />
	 * @param varName
	 * @return the Id of the Surface created or InvalidId().
	 */
	Id Define(const Name& varName)
	{
		BIO_SANITIZE(!RotateTo(varName),
			,
			return InvalidId()
		)
		Surface* toAdd = new Surface(
			varName,
			this
		);
		Add< Surface* >(toAdd);
		return toAdd->GetId();
	}

	/**
	 * Create a new variable within *this, the Biology way. <br />
	 * Will fail if varName already exists within *this. <br />
	 * @tparam T
	 * @param varName
	 * @return the Id of the Surface created or InvalidId().
	 */
	template < typename T >
	Id Define(const Name& varName)
	{
		BIO_SANITIZE(!RotateTo(varName),
			,
			return InvalidId()
		)
		Surface* toAdd = new Surface(
			varName,
			this
		);
		toAdd->Manage(new T());
		Add< Surface* >(toAdd);
		return toAdd->GetId();
	}

	/**
	 * Create a new variable within *this, the Biology way. <br />
	 * Assigns the new variable to the value provided. <br />
	 * Will fail if varName already exists within *this. <br />
	 * @tparam T
	 * @param varName
	 * @param assignment
	 * @return the Id of the Surface created or InvalidId().
	 */
	template < typename T >
	Id Define(
		const Name& varName,
		const T& assignment
	)
	{
		Surface* preExisting = RotateTo(varName);
		BIO_SANITIZE(!preExisting,
			,
			return InvalidId()
		)
		Surface* toAdd = new Surface(
			varName,
			this
		);
		toAdd->Manage(new T(assignment));
		Add< Surface* >(toAdd);
		return toAdd->GetId();
	}

	/**
	 * Get a variable from within *this. <br />
	 * Use for getting and/or setting. <br />
	 * REMINDER: use this->GetIdFromName("someName") to get the Id of a Surface with the given Name. <br />
	 * @param surfaceId
	 * @return a Surface with the given Id, as defined by *this, or NULL.
	 */
	virtual Surface* RotateTo(const Id& surfaceId);

	/**
	 * const version of RotateTo <br />
	 * @param surfaceId
	 * @return a const Surface with the given Id, as defined by *this, or NULL.
	 */
	virtual const Surface* RotateTo(const Id& surfaceId) const;

	/**
 	* Get a variable from within *this. <br />
	 * Use for getting and/or setting. <br />
	 * @param surfaceName
	 * @return a Surface with the given Name from *this.
	 */
	virtual Surface* RotateTo(const Name& surfaceName);

	/**
	 * const version of RotateTo <br />
	 * @param surfaceName
	 * @return a const Surface with the given Name from *this.
	 */
	virtual const Surface* RotateTo(const Name& surfaceName) const;

	/**
	 * Copy a Surface from another Molecule into *this. <br />
	 * @param source
	 * @param surface
	 * @return whether or not the operation succeeded.
	 */
	virtual bool DuplicateFrom(
		Molecule* source,
		const Name& surface
	);

	/**
	 * Move a Surface from another Molecule into *this. <br />
	 * @param source
	 * @param surface
	 * @return whether or not the operation succeeded.
	 */
	virtual bool TransferFrom(
		Molecule* source,
		const Name& surface
	);

	/**
	 * Required method from Wave. See that class for details. <br />
	 * @return a Symmetrical image of *this
	 */
	virtual const physical::Symmetry* Spin() const;

	/**
	 * Required method from Wave. See that class for details. <br />
	 * Reconstruct *this from the given Symmetry. <br />
	 * @param symmetry
	 */
	virtual Code Reify(physical::Symmetry* symmetry);

	/**
	 * operator wrappers around RotateTo <br />
	 * @param surfaceId
	 * @return RotateTo(...)
	 */
	virtual Surface* operator()(const Id& surfaceId);

	/**
	 * operator wrappers around RotateTo <br />
	 * @param surfaceId
	 * @return RotateTo(...)
	 */
	virtual const Surface* operator()(const Id& surfaceId) const;

	/**
	 * operator wrappers around RotateTo <br />
	 * @param surfaceId
	 * @return RotateTo(...)
	 */
	template < typename T >
	Surface* operator()(const Id& surfaceId)
	{
		return ChemicalCast< T >(RotateTo(surfaceId));
	}

	/**
	 * operator wrappers around RotateTo <br />
	 * @param surfaceName
	 * @return RotateTo(...)
	 */
	virtual Surface* operator()(const Name& name);

	/**
	 * operator wrappers around RotateTo <br />
	 * @param surfaceName
	 * @return RotateTo(...)
	 */
	virtual const Surface* operator()(const Name& name) const;

	/**
	 * operator wrappers around RotateTo <br />
	 * @param surfaceName
	 * @return RotateTo(...)
	 */
	template < typename T >
	Surface* operator()(const Name& surfaceName)
	{
		return ChemicalCast< T >(RotateTo(surfaceName));
	}

	/**
	 * Surface move operation. <br />
	 * Moves a Surface onto *this. <br />
	 * @param source
	 * @return this
	 */
	virtual Molecule* operator<<(Surface* source);

	/**
	 * Surface move operation. <br />
	 * Places *this in a Surface. <br />
	 * NOTE: There is no check that *this is not in multiple Surfaces. <br />
	 * @param target
	 * @return target
	 *
	 */
	virtual Surface* operator>>(Surface* target);

	/**
	 * Molecule copy operation. <br />
	 * Copies all Surfaces on the source Molecule onto *this. <br />
	 * @param source
	 * @return this
	 */
	virtual Molecule* operator<<(Molecule* source);

	/**
	 * Molecule move operation. <br />
	 * Moves all Surfaces on *this onto the target Molecule. <br />
	 * This REMOVES all Surfaces from *this. <br />
	 * @param target
	 * @return target
	 */
	virtual Molecule* operator>>(Molecule* target);
};

} //molecular namespace
} //bio namespace
