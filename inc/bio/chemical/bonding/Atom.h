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

#include "bio/common/Cast.h"
#include "bio/common/macro/Macros.h"
#include "bio/common/type/IsPointer.h"
#include "bio/common/type/IsReference.h"
#include "bio/common/type/IsConst.h"
#include "bio/common/type/RemovePointer.h"
#include "bio/common/type/RemoveReference.h"
#include "bio/common/type/RemoveConst.h"
#include "bio/physical/type/IsWave.h"
#include "bio/physical/wave/Quantum.h"
#include "bio/physical/common/Class.h"
#include "bio/chemical/common/BondTypes.h"
#include "bio/chemical/relativity/PeriodicTable.h"
#include "Bond.h"

namespace bio {
namespace chemical {

class Symmetry;

/**
 * Atom MUST be virtually inherited! <br />
 * Atom is an interface for other implementations. <br />
 * Atom objects form Bonds with Waves (such as Quantum Waves, like real-life electrons). Counter to how atoms are often described, here, you can Bond things that are not Atoms (iff they are a Wave). This is similar to forming ionic bonds or radicals where the "bonded" target is a sub-atomic particle (also a shared resource). <br />
 * Waves have a unique class name to Id mapping that is stored in the PeriodicTable. Atom objects take advantage of this to store and retrieve pointers to any type. <br />
 * Bonds are arbitrarily flexible, so this system can be (ab)used in many ways. <br />
 * Unlike real chemistry, the actual valence of a Atom changes to accommodate new Bonds. <br />
 * NOTE: mBonds[0] is always empty. This may change in a future release. <br />
 *
 * NOTE: iff using a C++ version below 14, only Quantum<> Bonds are supported. This is due to an inability to evaluate IsPrimitive<>() expressions at compile time and the compiler following all "possible" execution paths, which are necessarily incompatible. <br />
 */
class Atom :
	public physical::Class< Atom >
{
public:

	/**
	 * Ensure virtual methods point to Class implementations. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(physical, Atom)

	/**
	 *
	 */
	explicit Atom();

	/**
	 * We CANNOT copy Atoms contents at this time. <br />
	 * mBonds must contain uniquely bonded Waves. <br />
	 * @param other
	 */
	explicit Atom(const Atom& other);

	/**
	 *
	 */
	virtual ~Atom();

	/**
	 * Required method from Wave. See that class for details. <br />
	 * @return a Symmetrical image of *this
	 */
	virtual physical::Symmetry* Spin() const;

	/**
	 * Required method from Wave. See that class for details. <br />
	 * Reconstruct *this from the given Symmetry. <br />
	 * @param symmetry
	 */
	virtual Code Reify(physical::Symmetry* symmetry);

	/**
	 * If the given Wave Resonates (shares at least 1 Property) with any Bonded Wave in *this, the given Wave will be Demodulated and Attenuated by the Bonded Wave. <br />
	 * Please use a global Wave* if you intend to call this from a method that can be called by this. Using a global will engage the backflow prevention system and stop infinite loops from forming. <br />
	 * <br />
	 * Attenuation here operates slightly differently from the real world concept. Because we have a continuous flow of electrons providing the power to run this code, doing work is essentially free (or at least abstracted and we don't HAVE to worry about it), Attenuation is more like amplification, where flux, in terms of work, is generated, rather than dispersed. However, if we treat some desired state as flux and any deviation from that state as offering resistance, "information flux" is lost as the desired state is approached, making Attenuation technically correct. <br />
	 * @param other
	 */
	virtual Code Attenuate(const physical::Wave* other);

	/**
	 * If the given Wave Resonates (shares at least 1 Property) with any Bonded Wave in *this, the given Wave will be Demodulated and Disattenuated by the Bonded Wave. <br />
	 * This is the opposite of Attenuation (above). <br />
	 * Please use a global Wave* if you intend to call this from a method that can be called by this. Using a global will engage the backflow prevention system and stop infinite loops from forming. <br />
	 * @param other
	 */
	virtual Code Disattenuate(const physical::Wave* other);

	/**
	 * Simply get a bond. <br />
	 * @param position
	 * @return the Bonded Wave*
	 */
	physical::Wave* GetBonded(Valence position);

	/**
	 * Simply get a bond. <br />
	 * @param position
	 * @return the Bonded Wave*
	 */
	const physical::Wave* GetBonded(Valence position) const;


	/**
	 * Gets the bond to an bonded of type T from *this, then casts the Bond()ed Wave to T. <br />
	 * NOTE: const here is a lie (sort of). Changing the returned value indirectly changes *this. <br />
	 * @tparam T a non-reference, non-pointer type which is Bonded to *this; const is okay.
	 * @return a T that is Bond()ed with *this; else NULL.
	 */
	template < typename T >
	T* AsBonded() const
	{
		BIO_STATIC_ASSERT(!type::IsReference< T >())
		BIO_STATIC_ASSERT(!type::IsPointer< T >())

		Valence position = GetBondPosition< T >();
		BIO_SANITIZE(position,
			,
			return NULL
		)

		//What should be actually Bonded is a pointer to a physical::Class.
		//This is performed by the chemical::Class constructor.
		physical::Class< T >* bonded = ForceCast< physical::Class< T >* >(mBonds.OptimizedAccess(position)->GetBonded());
		return bonded->GetWaveObject();
	}

	/**
	 * If T IsPrimitive(), you can use *this to get the Bonded Quantum Wave for that primitive. <br />
	 * NOTE: const here is a lie (sort of). Changing the returned value indirectly changes *this. <br /
	 * @tparam T a non-reference, non-pointer type which is Bonded to *this; const is okay.
	 * @return *this as a T, from a Bonded Quantum Wave, or NULL.
	 */
	template < typename T >
	T* AsBondedQuantum() const
	{
		BIO_STATIC_ASSERT(!type::IsReference< T >())
		BIO_STATIC_ASSERT(!type::IsPointer< T >())

		Valence position = GetBondPosition< T >();
		BIO_SANITIZE(position, , return 0)
		physical::Quantum< T >* bonded = ForceCast< physical::Quantum< T >* >(mBonds.OptimizedAccess(position)->GetBonded());
		return bonded->GetQuantumObject();
	}

	/**
	 * This method is here so that we can add different As____ methods without changing the downstream interfaces. <br />
	 * NOTE: As CANNOT be used in constructors. The returned pointer may not be valid!!! <br />
	 * NOTE: const here is a lie (sort of). Changing the returned value indirectly changes *this. <br />
	 * @tparam T a pointer to a Bonded type and nothing else, no double pointers, references, etc; const is okay.
	 * @return *this as a T or NULL.
	 */
	template < typename T >
	T As() const
	{
		//We store a Wave* or a T*. Nothing else.
		BIO_STATIC_ASSERT(type::IsPointer< T >())
		BIO_STATIC_ASSERT(!type::IsPointer< typename type::RemovePointer< T >::Type >())

		#if BIO_CPP_VERSION >= 17
		if BIO_CONSTEXPR(!type::IsWave< T >())
		#endif
		{
			return AsBondedQuantum< typename type::RemovePointer< T >::Type >();
		}
		#if BIO_CPP_VERSION >= 17
		else
		{
			return AsBonded< typename type::RemovePointer< T >::Type >();
		}
		#endif
	}


	/**
	 * Atoms can be converted to anything they are bonded to. <br />
	 * NOTE: this CANNOT be used in constructors. The returned pointer may not be valid!!! <br />
	 * @tparam T a pointer to a Bonded type and nothing else, no double pointers, references, etc; const is okay.
	 * @return *this as a T or NULL.
	 */
	template < typename T >
	operator T() const
	{
		return As< T >();
	}

	/**
	 * This method strips all decorations from T in order to guarantee consistent behavior across varied usages. <br />
	 * For example GetBondId< const MyClass* > will give the same result as GetBondId< MyClass& >. <br />
	 * Because of this behavior, Atoms are incapable of bonding both a MyClass* as a Quantum and a MyClass object as a native Wave. <br />
	 * This is intentional. <br />
	 * @tparam T
	 * @return the Id to use when bonding the given type.
	 */
	template < typename T >
	static AtomicNumber GetBondId()
	{
		if BIO_CONSTEXPR(type::IsPointer< T >())
		{
			return GetBondId< typename type::RemovePointer< T >::Type >();
		}
		else if BIO_CONSTEXPR(type::IsReference< T >())
		{
			return GetBondId< typename type::RemoveReference< T >::Type >();
		}
		else if BIO_CONSTEXPR(type::IsConst< T >())
		{
			return GetBondId< typename type::RemoveConst< T >::Type >();
		}
		else
		{
			BIO_STATIC_ASSERT(!type::IsConst< T >());
			BIO_STATIC_ASSERT(!type::IsReference< T >());
			BIO_STATIC_ASSERT(!type::IsPointer< T >());
		}

		#if BIO_CPP_VERSION < 17
		return SafelyAccess<PeriodicTable>()->GetIdFromType< physical::Quantum< T >* >();
		#else
		if constexpr(!type::IsWave< T >())
		{
			return SafelyAccess<PeriodicTable>()->GetIdFromType< physical::Quantum< T >* >();
		}
		else
		{
			return SafelyAccess<PeriodicTable>()->GetIdFromType< T* >();
		}
		#endif
	}

	/**
	 * Adds a new Bond to *this or updates an Empty Bond for T. <br />
	 * Updating a Bond requires both Breaking and Forming steps to be done manually. <br />
	 * You CANNOT bond the same T twice (without Breaking the initial Bond). <br />
	 * See Molecule.h if you would like to Bond multiple Ts. <br />
	 * Like GetBondId, this method strips all decorations from T in order to guarantee consistent behavior across varied usages. <br />
	 * For example FormBond< const MyClass* > will give the same result as FormBond< MyClass& >. <br />
	 * Because of this behavior, Atoms are incapable of bonding both a MyClass* as a Quantum and a MyClass object as a native Wave. <br />
	 * This is intentional. <br />
	 * @tparam T A pointer to an instance of a class deriving from Wave and which is not already Bonded.
	 * @param toBond what to Bond
	 * @return The position of the created Bond in *this.
	 */
	template < typename T >
	Valence FormBond(
		T toBond,
		BondType type = bond_type::Unknown())
	{
		#if BIO_CPP_VERSION >= 17
		if constexpr(!type::IsWave< T >())
		#endif
		{ //extra scope doesn't hurt.
			if BIO_CONSTEXPR(type::IsPointer< T >())
			{
				#if BIO_CPP_VERSION < 17
				bool you_cannot_bond_pointers_until_cpp17 = false;
				BIO_ASSERT(you_cannot_bond_pointers_until_cpp17);
				#else
				BIO_SANITIZE(toBond,,return NULL)
				return FormBond< typename type::RemovePointer< T >::Type >(*toBond, type); //necessitates copy by value...
				#endif
			}
			else if BIO_CONSTEXPR(type::IsReference< T >())
			{
				return FormBond< typename type::RemoveReference< T >::Type >(toBond, type);
			}

			AtomicNumber bondedId = GetBondId< T >();
			return FormBondImplementation((new physical::Quantum< T >(toBond))->AsWave(),
				bondedId,
				type
			);
		}
		#if BIO_CPP_VERSION >= 17
		else
		{
			AtomicNumber bondedId = GetBondId< T >();
			physical::Wave* waveToBond;
			if constexpr(type::IsPointer< T >())
			{
				waveToBond = toBond->AsWave();
			}
			else
			{
				waveToBond = toBond.AsWave();
			}
			return FormBondImplementation(
				waveToBond,
				bondedId,
				type
			);
		}
		#endif
	}

	/**
	 * Breaking a Bond Break()s the associated position. <br />
	 * Removal of the Bond object is done upon destruction. <br />
	 * Updating a Bond requires both Breaking and Forming steps to be done manually. <br />
	 * NOTE: toDisassociate is not currently used for anything beyond automatic template type detection. <br />
	 * Like GetBondId, this method strips all decorations from T in order to guarantee consistent behavior across varied usages. <br />
	 * For example BreakBond< const MyClass* > will give the same result as BreakBond< MyClass& >. <br />
	 * Because of this behavior, Atoms are incapable of bonding both a MyClass* as a Quantum and a MyClass object as a native Wave. <br />
	 * This is intentional. <br />
	 * @tparam T
	 * @param toDisassociate
	 * @param type
	 * @return true if the Bond was Broken; false otherwise.
	 */
	template < typename T >
	bool BreakBond(
		T toDisassociate,
		BondType type = bond_type::Unknown())
	{
		return BreakBondImplementation(
			GetBondId< T >(),
			type
		);
	}


	/**
	 * Gives the array index of a Bond()ed Wave. <br />
	 * @param bondedId
	 * @return The position of the given Wave (Id) within *this; else 0.
	 */
	Valence GetBondPosition(AtomicNumber bondedId) const;

	/**
	 * Converts the given typeName to and Id <br />
	 * Gives the array index of a Bond()ed Wave. <br />
	 * @param typeName
	 * @return The position of the given Wave (Id) within *this; else 0.
	 */
	Valence GetBondPosition(const Name& typeName) const;


	/**
	 * GetBondPosition via a type, rather than an Wave Id. <br />
	 * @tparam T
	 * @return GetBondPosition() for the given type; else 0.
	 */
	template < typename T >
	Valence GetBondPosition() const
	{
		return GetBondPosition(GetBondId< T >());
	}

	/**
	 * Get the type of a Bond. <br />
	 * @param position
	 * @return the BondType of the Bond at the given position or BondTypePerspective::InvalidId().
	 */
	BondType GetBondType(Valence position) const;

	/**
	 * Get the type of a Bond. <br />
	 * @tparam T
	 * @return the BondType of the Bond for T or BondTypePerspective::InvalidId().
	 */
	template < typename T >
	BondType GetBondType() const
	{
		return GetBondType(GetBondPosition< T >());
	}

	/**
	 * DANGEROUS! <br />
	 * @return a pointer to the Bonds in *this.
	 */
	Bonds* GetAllBonds();

	/**
	 * DANGEROUS! (but slightly less so). <br />
	 * @return a pointer to the Bonds in *this.
	 */
	const Bonds* GetAllBonds() const;

	/**
	 * Create a Bond. <br />
	 * This is public for use in constructors. <br />
	 * Please use FormBond<> unless you are forced to call this impl method. <br />
	 * @param toBond
	 * @param id
	 * @param type
	 * @return the position of the new Bond in *this.
	 */
	virtual Valence FormBondImplementation(
		physical::Wave* toBond,
		AtomicNumber id,
		BondType type
	);

	/**
	 * Remove a Bond. <br />
	 * This is public for use in destructors. <br />
	 * Please use BreakBond<> unless you are forced to call this impl method. <br />
	 * @param id
	 * @param type
	 * @return
	 */
	virtual bool BreakBondImplementation(
		AtomicNumber id,
		BondType type
	);

protected:
	Bonds mBonds;

	//Prevent (Dis)Attenuation from being called multiple times in the same call stack.
	Arrangement< physical::Wave* > mBackflowPreventer;
};

} //chemical namespace
} //bio namespace
