/*
 * This file is a part of the Biology project by eons LLC.
 * Biology (aka Develop Biology) is a framework for approaching software
 * development from a natural sciences perspective.
 *
 * Copyright (C) 2023 Séon O'Shannon & eons LLC
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

#include "Neurite.h"

namespace bio {
namespace neural {

class Dendrite:
	public neural::Class< Dendrite >,
	virtual public Neurite
{
public:

	/**
	 * Disambiguate all class methods. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(neural, Dendrite)

	BIO_DEFAULT_IDENTIFIABLE_CONSTRUCTORS(
		neural,
		Dendrite,
		filter::Neural()
	)

	/**
	 *
	 */
	virtual ~Dendrite();

	/**
	 * Use this method to populate any member variable Protein*s. <br />
	 * You'll want to do this to speed up your code by bypassing the dynamic execution provided by genetic::Expressor. <br />
	 */
	virtual Code CacheProteins();

	/**
	 * If you use CacheProteins, you'll likely want to create your default Proteins here. <br />
	 * This will prevent dereferencing null or garbage pointers when using your cached Proteins. <br />
	 */
	virtual Code CreateDefaultProteins();

	/**
	 * PROTEIN BASED. <br />
	 * Called after ProcessOutgoing calls Update(time) <br />
	 * For example, if the source says this should be added at the current time, and *this has some sort of delay (perhaps from learning), this method will return the current time plus the indicated delay. <br />
	 * @param whenToPotentiate the time at which the source wants *this to be added. <br />
	 * @return the timestamp that Update will use to set the time at which *this should be added.
	 */
	virtual Timestamp PrepareForPotentiation(Timestamp whenToPotentiate);

	/**
	 * PROTEIN BASED. <br />
	 * Called directly after *this is added, PrepareForDepotentiation does whatever is necessary for ProcessDepotentiation to work. <br />
	 * For example, if *this uses a New and Old value, this method would set Old to New and zero out New. <br />
	 */
	virtual void PrepareForDepotentiation();

	/**
	 * PROTEIN BASED. <br />
	 * Modifies the receiver according to the purpose of *this (e.g. incrementing/decrementing the receiver's value) <br />
	 * Removal is always the opposite of Addition such that: p = Removal(Addition(p)) && p = Addition(Removal(p)) <br />
	 * EXTREME care must be taken when the above is not possible. <br />
	 * @return the result of adding *this to the target.
	 */
	virtual Code ProcessPotentiation();

	/**
	 * PROTEIN BASED. <br />
	 * Modifies the receiver according to the purpose of *this (e.g. incrementing/decrementing the receiver's value) <br />
	 * Removal is always the opposite of Addition such that: p = Removal(Addition(p)) && p = Addition(Removal(p)) <br />
	 * EXTREME care must be taken when the above is not possible. <br />
	 * @return the result of removing *this from the target.
	 */
	virtual Code ProcessDepotentiation();

	/**
	 * PROTEIN BASED. <br />
	 * If you would like to do something after *this has been fully added to the target, put that code in the appropriate Post____() method. <br />
	 * @return the result of PostPotentiation()
	 */
	virtual Code PostPotentiation();

	/**
	 * PROTEIN BASED. <br />
	 * If you would like to do something after *this has been fully removed from the target, put that code in the appropriate Post____() method. <br />
	 * @return the result of PostDepotentiation()
	 */
	virtual Code PostDepotentiation();

	/**
	 * This method does not need to be overwritten. See the IsZero, above. <br />
	 * @return true if *this should be added at the current time, and the value of the data are >0 || AreZerosAllowed is true. false otherwise
	 */
	virtual bool ShouldBePotentiated() const;

	/**
	 * @return true if *this has expired.
	 */
	virtual bool ShouldBeDepotentiated() const;

	/**
	 * PROTEIN BASED. <br />
	 * For rare circumstances, where *this might need to be re-added <br />
	 * TODO: is there a better solution? <br />
	 */
	virtual void ReReady();

	/**
	 * @return the timestamp at which *this should be added to the target
	 */
	virtual Timestamp GetTimeToAdd() const;

	/**
	 * @return what *this sends data to, the target.
	 */
	Neuron* GetPostsynapticNeuron();

	/**
	 * @return what *this sends data to, the target.
	 */
	const Neuron* GetPostsynapticNeuron() const;

	/**
	 * @return the Name of what *this sends data to, the target.
	 */
	Name GetPostsynapticName() const;

	/**
	 * @return the Id of what *this sends data to, the target.
	 */
	Id GetPostsynapticId() const;

protected:
	Neuron* mPostsynapticNeuron;

	molecular::Protein* mcPrepareForPotentiation;
	molecular::Protein* mcProcessPotentiation;
	molecular::Protein* mcPostPotentiation;

	molecular::Protein* mcPrepareForDepotentiation;
	molecular::Protein* mcProcessDepotentiation;
	molecular::Protein* mcPostDepotentiation;

	molecular::Protein* mcReReady;

	Timestamp mPotentiateAt;
	Timestamp mLastPotentiated;

	/**
	 * Used when the target object is unknown or has not been created yet. <br />
	 */
	Id mPostsynapticNeuronId;


	/**
	 * What to do when *this is no longer ready to be potentiated. <br />
	 */
	virtual void NoLongerReady();

	/**
	 * Hook for when *this is Potentiated. <br />
	 */
	virtual void Potentiated();

	/**
	 * Hook for when *this is Depotentiated. <br />
	 */
	virtual void Depotentiated();

};

} // neural namespace
} // bio namespace
