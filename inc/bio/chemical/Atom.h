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
#include "bio/common/macros/Macros.h"
#include "bio/physical/utilities/IsWave.h"
#include "bio/common/macros/OSMacros.h"
#include "bio/physical/Quantum.h"
#include "bio/physical/common/Class.h"
#include "bio/chemical/common/BondTypes.h"
#include "PeriodicTable.h"
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
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(physical,
		Atom)

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
	 * If the given Wave Resonates with any Bonded Wave in *this, the given Wave will be Demodulated and Attenuated by the Bonded Wave. <br />
	 * Attenuation here operates slightly differently from the real world concept. Because we have a continuous flow of electrons providing the power to run this code, doing work is essentially free (or at least abstracted and we don't HAVE to worry about it), Attenuation is more like amplification, where flux, in terms of work, is generated, rather than dispersed. However, if we treat some desired state as flux and any deviation from that state as offering resistance, "information flux" is lost as the desired state is approached, making Attenuation technically correct. <br />
	 * @param other
	 */
	virtual Code Attenuate(const Wave* other);

	/**
	 * If the given Wave Resonates with any Bonded Wave in *this, the given Wave will be Demodulated and Disattenuated by the Bonded Wave. <br />
	 * This is the opposite of Attenuation (above). <br />
	 * @param other
	 */
	virtual Code Disattenuate(const Wave* other);

	/**
	 * Simply get a bond. <br />
	 * @param position
	 * @return the Bonded Wave*
	 */
	Wave* GetBonded(Valence position);

	/**
	 * Simply get a bond. <br />
	 * @param position
	 * @return the Bonded Wave*
	 */
	const Wave* GetBonded(Valence position) const;

	/**
	 * Gets the bond to an bonded of type T from *this, then casts the Bond()ed Wave to T*. <br />
	 * @tparam T
	 * @return a T* that is Bond()ed with *this; else NULL.
	 */
	template < typename T >
	T AsBonded()
	{
		Valence position = GetBondPosition< T >();
		BIO_SANITIZE(position, ,
			return NULL);
		return ForceCast< T >(mBonds.OptimizedAccess(position)->GetBonded());
	}

	/**
	 * Gets the bond to an bonded of type T from *this, then casts the Bond()ed Wave to T*. <br />
	 * @tparam T
	 * @return  a const T* that is Bond()ed with *this; else NULL.
	 */
	template < typename T >
	const T AsBonded() const
	{
		Valence position = GetBondPosition< T >();
		BIO_SANITIZE(position, ,
			return 0);
		return ForceCast< const T >(mBonds.OptimizedAccess(position)->GetBonded());
	}

	/**
	 * If T IsPrimitive(), you can use *this to get the Bonded Quantum Wave for that primitive. <br />
	 * @tparam T
	 * @return *this as a T*, from a Bonded Quantum Wave, or NULL.
	 */
	template < typename T >
	T AsBondedQuantum()
	{
		physical::Quantum< T >* bonded = AsBonded< physical::Quantum< T >* >();
		BIO_SANITIZE(bonded, ,
			return 0)
		return bonded->operator T();
	}

	/**
	 * Const version of AsBondedQuantum(). <br />
	 * @tparam T
	 * @return *this as a const T*, from a Bonded Quantum Wave.
	 */
	template < typename T >
	const T AsBondedQuantum() const
	{
		physical::Quantum< T >* bonded = AsBonded< physical::Quantum< T >* >();
		BIO_SANITIZE(bonded, ,
			return 0)
		return bonded->operator T();
	}

	/**
	 * This method is here so that we can add different As____ methods without changing the downstream interfaces. <br />
	 * @tparam T
	 * @return *this as a T* or NULL.
	 */
	template < typename T >
	T As()
	{
		#if BIO_CPP_VERSION < 17
		return AsBondedQuantum< T >();
		#else
		if constexpr(!utility::IsWave< T >())
		{
			return AsBondedQuantum< T >();
		}
		else
		{
			return AsBonded< T >();
		}
		#endif
	}

	/**
	 * Const version of As(). <br />
	 * This method is here so that we can add different As____ methods without changing the downstream interfaces. <br />
	 * @tparam T
	 * @return *this as a const T* or NULL.
	 */
	template < typename T >
	const T As() const
	{
		#if BIO_CPP_VERSION < 17
		return AsBondedQuantum< T >();
		#else
		if constexpr(!utility::IsWave< T >())
		{
			return AsBondedQuantum< T >();
		}
		else
		{
			return AsBonded< T >();
		}
		#endif
	}


	/**
	 * Atoms can be converted to anything they are bonded to. <br />
	 * @tparam T
	 * @return *this as a T* or NULL.
	 */
	template < typename T >
	operator T()
	{
		return As< T >();
	}

	/**
	 * @tparam T
	 * @return the Id to use when bonding the given type.
	 */
	template < typename T >
	static AtomicNumber GetBondId()
	{
		#if BIO_CPP_VERSION < 17
		return SafelyAccess<PeriodicTable>()->GetIdFromType< physical::Quantum< T >* >();
		#else
		if constexpr(!utility::IsWave< T >())
		{
			return SafelyAccess<PeriodicTable>()->GetIdFromType< physical::Quantum< T >* >();
		}
		else
		{
			return SafelyAccess<PeriodicTable>()->GetIdFromType< T >();
		}
		#endif
	}

	/**
	 * Adds a new Bond to *this or updates an Empty Bond for T. <br />
	 * Updating a Bond requires both Breaking and Forming steps to be done manually. <br />
	 * You CANNOT bond the same T twice (without Breaking the initial Bond). <br />
	 * See Molecule.h if you would like to Bond multiple Ts. <br />
	 * @tparam T A pointer to an instance of a class deriving from Wave and which is not already Bonded.
	 * @param toBond what to Bond
	 * @return true, if the Bond was created; false otherwise.
	 */
	template < typename T >
	bool FormBond(
		T toBond,
		BondType type = bond_type::Unknown())
	{
		AtomicNumber bondedId = GetBondId< T >();
		#if BIO_CPP_VERSION < 17
		return FormBondImplementation(
			(new physical::Quantum< T >(toBond))->AsWave(),
			bondedId,
			type
		);
		#else
		if constexpr(!utility::IsWave< T >())
		{
			return FormBondImplementation((new physical::Quantum< T >(toBond))->AsWave(),
				bondedId,
				type
			);
		}
		else
		{
			return FormBondImplementation(
				toBond->AsWave(),
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
		#if BIO_CPP_VERSION < 17
		return BreakBond< physical::Quantum< T >* >( 
			NULL,
			type
		);
		#else
		if constexpr(!utility::IsWave< T >())
		{
			return BreakBond< physical::Quantum< T >* >(
				NULL,
				type
			); //T matters, toDisassociate does not.
		}

		AtomicNumber bondedId = SafelyAccess<PeriodicTable>()->GetIdFromType< T >();
		return BreakBondImplementation(
			toDisassociate,
			bondedId,
			type
		);
		#endif
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
	Valence GetBondPosition(Name typeName) const;


	/**
	 * GetBondPosition via a type, rather than an Wave Id. <br />
	 * @tparam T
	 * @return GetBondPosition() for the given type; else 0.
	 */
	template < typename T >
	Valence GetBondPosition() const
	{
		#if BIO_CPP_VERSION < 17
		return GetBondPosition(SafelyAccess<PeriodicTable>()->GetIdFromType< physical::Quantum< T >* >());
		#else
		if constexpr(!utility::IsWave< T >())
		{
			return GetBondPosition(SafelyAccess<PeriodicTable>()->GetIdFromType< physical::Quantum< T >* >());
		}
		return GetBondPosition(SafelyAccess<PeriodicTable>()->GetIdFromType< T >());
		#endif
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
		#if BIO_CPP_VERSION < 17
		return GetBondType(GetBondPosition< physical::Quantum< T > >());
		#else
		if constexpr(!utility::IsWave< T >())
		{
			return GetBondType(GetBondPosition< physical::Quantum< T > >());
		}
		return GetBondType(GetBondPosition< T >());
		#endif
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
	 * This is public for use in ctors. Please use FormBond<> unless you are forced to call this impl method. <br />
	 * @param toBond
	 * @param id
	 * @param type
	 */
	virtual bool FormBondImplementation(
		Wave* toBond,
		AtomicNumber id,
		BondType type
	);

	/**
	 * Remove a Bond. <br />
	 * This is public for use in dtors. Please use BreakBond<> unless you are forced to call this impl method. <br />
	 * @param toDisassociate
	 * @param id
	 * @param type
	 * @return
	 */
	virtual bool BreakBondImplementation(
		Wave* toDisassociate,
		AtomicNumber id,
		BondType type
	);

protected:
	Bonds mBonds;
};

} //chemical namespace
} //bio namespace
