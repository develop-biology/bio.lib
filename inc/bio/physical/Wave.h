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

#include "bio/physical/common/Types.h"

//@formatter:off
#if BIO_CPP_VERSION < 11
	#include <stdint.h>
#else
	#include <cstdint>
#endif
//@formatter:on

namespace bio {

//the easy way out...
//see AsAtom()
namespace chemical {
class Atom;
}

namespace physical {

class Symmetry;

typedef ::bio::Arrangement< Symmetry* > Symmetries;

class Wave;

typedef ::bio::Arrangement< Wave* > Waves;
typedef ::bio::Arrangement< const Wave* > ConstWaves;

/**
 * A Wave is a base class for all Biology objects. <br />
 * Waves can be Periodic but do not have to be. By default, we assume Waves have no discernible frequency, amplitude, or any other Property. This is similar to a chaotic pattern or a wave modulated with enough signals that it becomes indiscernible from white noise / randomness. <br />
 * We do not / cannot make any assumptions about the ordering / pattern of a Wave at this level. Such Properties will only manifest in derived classes. <br />
 *
 * You should be able to upcast to Wave from any other class. <br />
 * Cloning a Wave should produce a functional fully derived class. This means that all diamond inheritance should be resolved by all children when calling Clone <br />
 * For downcasting to a derived class, you will need to use the chemical::Atom::Bond(...) methodology. See chemical/Atom.h for more info. <br />
 *
 * Waves may be Modulated to store additional signals and subsequently Demodulated in order to retrieve that original signal. In this way, any Wave can carry any other. <br />
 */
class Wave
{
public:

	/**
	 *
	 */
	Wave(Symmetry* symmetry = NULL);

	/**
	 *
	 */
	virtual ~Wave();

	/**
	 * @return a copy of the most derived object of *this.
	 */
	virtual Wave* Clone() const;

	/**
	 * Waves, depending on their behavior, can have different Properties. <br />
	 * GetProperties can be used for determining how to downcast *this. <br />
	 * The Properties returned dictate what a Wave can do and / or what can be done with one. <br />
	 * If we treat Properties as fourier components of a waveform, we could restate GetProperties as GetPeriodicComponents. In this context, "what a wave can do" and "what can be done with a wave" can be expressed as "which systems resonate with the wave in question" or "which systems have comparable periodic components", which is true here as well: when 2 Waves have the same Properties (i.e. Resonate with each other) they can be treated the same in some regard (perhaps they are "numeric" and can be "added") and when 2 waves have comparable properties, they can interact with each other. <br />
	 * It is up to you and other users of this framework to determine which Properties to use where. This is your space, so make use of it when you feel it's appropriate. <br />
	 * NOTE: Waves do not actually have mProperties. This method MUST be implemented by children in order to work. If using any chemical::Class or beyond, this method will be implemented for you. See chemical/Class.h for more info. <br />
	 * @return the Properties of *this (empty vector unless overridden).
	 */
	virtual Properties GetProperties() const;

	/**
	 * Check how (and if) 2 Waves Resonate with each other. <br />
	 * Resonance is defined as a commonality between 2 or more Waves. This is a little bit more generic than real life resonance, which is strictly a measure of increased amplitude when 2 or more waves interact. Here, Waves interacting could mean an increase in aperiodic behavior, where no frequency has any single discernible change to it, or any number of other complex transformations. <br />
	 * @param waves
	 * @return the number of overlapping Properties between both waves given.
	 */
	static Properties GetResonanceBetween(ConstWaves waves);

	/**
	 * Ease of use method for getting the Resonance between just 2 waves, rather than n. <br />
	 * @param wave1
	 * @param wave2
	 * @return the number of overlapping Properties between both waves given.
	 */
	static Properties GetResonanceBetween(
		const Wave* wave1,
		const Wave* wave2
	);

	/**
	 * Ease of use method for getting the Resonance between a Wave and a set of Properties <br />
	 * @param wave
	 * @param properties
	 * @return the number of overlapping Properties between the Wave and Properties given.
	 */
	static Properties GetResonanceBetween(
		const Wave* wave,
		const Properties& properties
	);

	/**
	 * Spinning a Wave produces a Symmetry. Waves can be Rotated about any number of Axes. <br />
	 * Spinning a Wave along one dimension (one Axis) would be the equivalent of reflecting that Wave or, possibly, refracting, dispersing, or otherwise altering the wave (e.g. if the Axis acted like a prism, rather than a mirror). When a Wave is Spun around multiple dimensions, the resulting Symmetry and effected transformations may not fall under any single property characteristic of real waves. Thus we treat Waves more like particles with a discrete spin. <br />
	 *
	 * You can visualize this by imagining a triangle that is then folded over to produce a polygon. Reflecting a triangle about 1 axis generates a rectangle, 2 a square, 3 a hexagon, 4 an octagon, and so on (sides = 3 + fibonacci sequence, starting at 0). Though, in this system, Axes do not overlap, so 1/2 of the polygon is discarded, once n >= 2. In this way, Spin() generates a triangle and you decide the shape by specifying how many Axes to Rotate the Symmetry about. <br />
	 *
	 * You should override Spin to update mSymmetry, then return the parent method. <br />
	 * See Quantum.h for an example interface. <br />
	 * @return mSymmetry.
	 */
	virtual Symmetry* Spin() const;

	/**
	 * Reifying a Wave takes a Symmetry and realizes it by copying the values supplied into *this. <br />
	 * Will update mSymmetry to the Symmetry provided but do nothing else. <br />
	 * You should override Reify to update the contents of *this from the given Symmetry. Calling the parent method after is optional. <br />
	 * See Quantum.h for an example interface. <br />
	 * @param symmetry
	 */
	virtual Code Reify(Symmetry* symmetry);

	/**
	 * This will overwrite any signal currently carried by *this. <br />
	 * @return the signal Modulated.
	 */
	virtual Wave* Modulate(Wave* signal);

	/**
	 * Treats *this as a carrier wave. <br />
	 * @return the signal carried by *this.
	 */
	virtual Wave* Demodulate();

	/**
	 * Treats *this as a carrier wave. <br />
	 * @return the signal carried by *this.
	 */
	virtual const Wave* Demodulate() const;

	/**
	 * Moves other through *this, taking something from it. <br />
	 * Used for += operator <br />
	 * This is a nop unless implemented by children. <br />
	 * @param other
	 */
	virtual Code Attenuate(const Wave* other);

	/**
	 * Pulls other out of *this, maybe giving something back? <br />
	 * The opposite of Attenuation. <br />
	 * Used for -= operator. <br />
	 * This is a nop unless implemented by children. <br />
	 * @param other
	 */
	virtual Code Disattenuate(const Wave* other);

	/**
	 * For Upcasting. <br />
	 * Used for resolving ambiguous inheritance without the need to explicitly derive from Wave. <br />
	 * NOTE: operator Wave*() and various perturbations fail to resolve ambiguous implicit upcasting. <br />
	 * @return this
	 */
	Wave* AsWave()
	{
		return this;
	}

	/**
	 * For Upcasting. <br />
	 * Used for resolving ambiguous inheritance without the need to explicitly derive from Wave. <br />
	 * NOTE: operator Wave*() and various perturbations fail to resolve ambiguous implicit upcasting. <br />
	 * @return this
	 */
	const Wave* AsWave() const
	{
		return this;
	}

	/**
	 * For Downcasting. <br />
	 * Because Wave cannot be virtually inherited and will likely become an ambiguous base, we commit a sin and provide a cheat. <br />
	 * This method can be used to resolve any downcasting. <br />
	 * For example: this->AsAtom()->As<Whatever>(). <br />
	 * @return this as an Atom, if possible.
	 */
	virtual chemical::Atom* AsAtom()
	{
		return NULL;
	}

	/**
	 * For Downcasting. <br />
	 * Because Wave cannot be virtually inherited and will likely become an ambiguous base, we commit a sin and provide a cheat. <br />
	 * This method can be used to resolve any downcasting. <br />
	 * For example: this->AsAtom()->As<Whatever>(). <br />
	 * @return this as an Atom, if possible.
	 */
	virtual const chemical::Atom* AsAtom() const
	{
		return NULL;
	}

	//HERE THERE BE OPERATORS

	/**
	 * Reifies *this. <br />
	 * (*myParticle) | jsonAxis("..."); <br />
	 * @param symmetry
	 */
	virtual void operator|(Symmetry* symmetry);

	/**
	 * Modulate operator (i.e. not "multiply") <br />
	 * @param signal
	 * @return Modulate(signal)
	 */
	virtual Wave* operator*(Wave* signal);

	/**
	 * Demodulate operator (i.e. not "dereference") <br />
	 * @return Demodulate()
	 */
	virtual Wave* operator*();

	/**
	 * Demodulate operator (i.e. not "dereference") <br />
	 * @return Demodulate()
	 */
	virtual const Wave* operator*() const;

	/**
	 * Makes other interfere with *this. <br />
	 * Attenuates other. <br />
	 * @param other
	 */
	virtual void operator+(const Wave* other);

	/**
	 * Removes the interference of other from *this. <br />
	 * Disattenuates other. <br />
	 * @param other
	 */
	virtual void operator-(const Wave* other);

protected:
	/**
	 * We cache our Symmetry here to avoid excessive new & deletes when Spinning & Reifying *this. <br />
	 */
	Symmetry* mSymmetry;

	/**
	 * for Modulation. <br />
	 */
	Wave* mSignal;
};

} //physical namespace
} //bio namespace
