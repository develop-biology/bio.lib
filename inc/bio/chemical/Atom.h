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

#include "bio/common/Cast.h"
#include "bio/common/Macros.h"
#include "bio/common/IsPrimitive.h"
#include "bio/common/OS.h"
#include "bio/physical/Quantum.h"
#include "bio/physical/Class.h"
#include "PeriodicTable.h"
#include "BondTypes.h"
#include "Bond.h"

namespace bio {
namespace chemical {

class Symmetry;

class Bond;

/**
 * Atom MUST be virtually inherited!
 * Atom is an interface for other implementations.
 * Atom objects form Bonds with Waves (such as Quantum Waves, like real-life electrons). Counter to how atoms are often described, here, you can Bond things that are not Atoms (iff they are a Wave). This is similar to forming ionic bonds or radicals where the "bonded" target is a sub-atomic particle (also a shared resource).
 * Waves have a unique class name to Id mapping that is stored in the PeriodicTable. Atom objects take advantage of this to store and retrieve pointers to any type.
 * Bonds are arbitrarily flexible, so this system can be (ab)used in many ways.
 * Unlike real chemistry, the actual valence of a Atom changes to accommodate new Bonds.
 * NOTE: m_bonds[0] is always empty. This may change in a future release.
 */
class Atom :
	public physical::Class< Atom >
{
public:

	/**
	 * Ensure virtual methods point to Class implementations.
	 */
	BIO_DISAMBIGUATE_CLASS_METHODS(physical,
		Atom)

	/**
	 *
	 */
	explicit Atom();

	/**
	 * We CANNOT copy Atoms contents at this time.
	 * m_bonds must contain uniquely bonded Waves.
	 * @param other
	 */
	explicit Atom(const Atom& other);

	/**
	 *
	 */
	virtual ~Atom();

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
	 * If the given Wave Resonates with any Bonded Wave in *this, the given Wave will be Demodulated and Attenuated by the Bonded Wave.
	 * Attenuation here operates slightly differently from the real world concept. Because we have a continuous flow of electrons providing the power to run this code, doing work is essentially free (or at least abstracted and we don't HAVE to worry about it), Attenuation is more like amplification, where flux, in terms of work, is generated, rather than dispersed. However, if we treat some desired state as flux and any deviation from that state as offering resistance, "information flux" is lost as the desired state is approached, making Attenuation technically correct.
	 * @param other
	 */
	virtual Code Attenuate(const Wave* other);

	/**
	 * If the given Wave Resonates with any Bonded Wave in *this, the given Wave will be Demodulated and Disattenuated by the Bonded Wave.
	 * This is the opposite of Attenuation (above).
	 * @param other
	 */
	virtual Code Disattenuate(const Wave* other);

	/**
	 * Simply get a bond.
	 * @param position
	 * @return the Bonded Wave*
	 */
	Wave* GetBonded(Valence position);

	/**
	 * Simply get a bond.
	 * @param position
	 * @return the Bonded Wave*
	 */
	const Wave* GetBonded(Valence position) const;

	/**
	 * Gets the bond to an bonded of type T from *this, then casts the Bond()ed Wave to T*.
	 * @tparam T
	 * @return a T* that is Bond()ed with *this; else NULL.
	 */
	template < typename T >
	T* AsBonded()
	{
		Valence position = GetBondPosition< T >();
		BIO_SANITIZE(position, ,
			return NULL);
		return Cast< T* >(m_bonds[position].GetBonded());
	}

	/**
	 * Gets the bond to an bonded of type T from *this, then casts the Bond()ed Wave to T*.
	 * @tparam T
	 * @return  a const T* that is Bond()ed with *this; else NULL.
	 */
	template < typename T >
	const T* AsBonded() const
	{
		Valence position = GetBondPosition< T >();
		BIO_SANITIZE(position, ,
			return NULL);
		return Cast< const T* >(m_bonds[position].GetBonded());
	}

	/**
	 * If T IsPrimitive(), you can use *this to get the Bonded Quantum Wave for that primitive.
	 * @tparam T
	 * @return *this as a T*, from a Bonded Quantum Wave, or NULL.
	 */
	template < typename T >
	T* AsBondedQuantum()
	{
		return Cast< T* >(AsBonded< physical::Quantum< T > >());
	}

	/**
	 * Const version of AsBondedQuantum().
	 * @tparam T
	 * @return *this as a const T*, from a Bonded Quantum Wave.
	 */
	template < typename T >
	const T* AsBondedQuantum() const
	{
		return AsBonded< physical::Quantum< T > >();
	}

	/**
	 * Checks if T is a primitive type, which would require a Quantum<T> Bond or if T should be a Wave and can be Bonded regularly.
	 * @tparam T
	 * @return *this as a T* or NULL.
	 */
	template < typename T >
	T* As()
	{
		if (IsPrimitive< T >())
		{
			return AsBondedQuantum< T >();
		}
		else
		{
			return AsBonded< T >();
		}
	}

	/**
	 * Const version of As().
	 * @tparam T
	 * @return *this as a const T* or NULL.
	 */
	template < typename T >
	const T* As() const
	{
		if (IsPrimitive< T >())
		{
			return AsBondedQuantum< T >();
		}
		else
		{
			return AsBonded< T >();
		}
	}


	/**
	 * Atoms can be converted to anything they are bonded to.
	 * @tparam T
	 * @return *this as a T* or NULL.
	 */
	template < typename T >
	operator T*()
	{
		return As< T >();
	}

	/**
	 * Adds a new Bond to *this or updates an Empty Bond for T.
	 * Updating a Bond requires both Breaking and Forming steps to be done manually.
	 * You CANNOT bond the same T twice (without Breaking the initial Bond).
	 * See Molecule.h if you would like to Bond multiple Ts.
	 * @tparam T A class deriving from Wave which is not already Bonded.
	 * @param toBond what to Bond
	 * @return true, if the Bond was created; false otherwise.
	 */
	template < typename T >
	bool FormBond(
		T* toBond,
		BondType type = bond_type::Unknown())
	{
		BIO_SANITIZE(IsPrimitive< T >(), ,
			return false);
		AtomicNumber bondedId = PeriodicTable::Instance().GetIdFromType< T >();
		return FormBondImplementation(
			toBond->AsWave(),
			bondedId,
			type
		);
	}

	/**
	 * Breaking a Bond Break()s the associated position.
	 * Removal of the Bond object is done upon destruction.
	 * Updating a Bond requires both Breaking and Forming steps to be done manually.
	 * @tparam T
	 * @param toDisassociate
	 * @param type
	 * @return true if the Bond was Broken; false otherwise.
	 */
	template < typename T >
	bool BreakBond(
		T* toDisassociate,
		BondType type = bond_type::Unknown())
	{
		BIO_SANITIZE(IsPrimitive< T >(), ,
			return false);
		BIO_SANITIZE(Cast< Wave* >(toDisassociate), ,
			return false);
		AtomicNumber bondedId = PeriodicTable::Instance().GetIdFromType< T >();
		return BreakBondImplementation(
			toDisassociate,
			bondedId,
			type
		);
	}


	/**
	 * Gives the array index of a Bond()ed Wave.
	 * @param bondedId
	 * @return The position of the given Wave (Id) within *this; else 0.
	 */
	Valence GetBondPosition(AtomicNumber bondedId) const;

	/**
	 * Converts the given typeName to and Id
	 * Gives the array index of a Bond()ed Wave.
	 * @param typeName
	 * @return The position of the given Wave (Id) within *this; else 0.
	 */
	Valence GetBondPosition(Name typeName) const;


	/**
	 * GetBondPosition via a type, rather than an Wave Id.
	 * @tparam T
	 * @return GetBondPosition() for the given type; else 0.
	 */
	template < typename T >
	Valence GetBondPosition() const
	{
		return GetBondPosition(PeriodicTable::Instance().GetIdFromType< T >());
	}

	/**
	 * Get the type of a Bond.
	 * @param position
	 * @return the BondType of the Bond at the given position or BondTypePerspective::InvalidId().
	 */
	BondType GetBondType(Valence position) const;

	/**
	 * Get the type of a Bond.
	 * @tparam T
	 * @return the BondType of the Bond for T or BondTypePerspective::InvalidId().
	 */
	template < typename T >
	BondType GetBondType() const
	{
		return GetBondType(GetBondPosition< T >());
	}

protected:
	Bond* m_bonds;
	Valence m_valence;

	/**
	 * Create a Bond.
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
	 * Remove a Bond.
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
};

} //chemical namespace
} //bio namespace
