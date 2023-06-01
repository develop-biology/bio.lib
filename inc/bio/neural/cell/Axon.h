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

class Axon:
	public neural::Class< Axon >,
	virtual public Neurite
{
public:

	/**
	 * Disambiguate all class methods. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(neural, Axon)

	BIO_DEFAULT_IDENTIFIABLE_CONSTRUCTORS(
		neural,
		Axon,
		filter::Neural()
	)

	/**
	 *
	 */
	virtual ~Axon();

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
	 * ProcessOutgoing takes information from the source Neuron and makes it available to the target Neuron. <br />
	 * The data accessed by *this is copied and the copy is stored in *this for access by the target Neuron. This allows the source Neuron to continue editing the data it passed, as well as for *this to manipulate data from the source without affecting other Synapses. <br />
	 * If *this is going to send data to the target, ProcessOutgoing MUST call Update(time to add the given data) <br />
	 * @return the result of sending data to the target.
	 */
	virtual Code ProcessOutgoing();

	/**
	 * Take in new information. <br />
	 * @param whenToPotentiate when the the new information should be Potentiated.
	 */
	virtual void Update(Timestamp whenToPotentiate);

	/**
	 * PROTEIN BASED <br />
	 * This method will allow the target to give some information back to *this <br />
	 * Such a call may be useful in learning, etc. <br />
	 * @param arg
	 */
	virtual void Callback(const molecular::Molecule* arg);

	/**
	 * @return the last time Update() was called.
	 */
	virtual Timestamp GetTimeLastUpdated() const;

	/**
	 * @return the time that *this should remain added for.
	 */
	virtual Milliseconds GetTimeout() const;

	/**
	 * @param lastFor the time that *this should remain added for.
	 */
	virtual void SetTimeout(Milliseconds lastFor);

	/**
	 * Sets the expiration time such that *this will expire at the given time. <br />
	 * Has no effect if the given time is later than the current time. <br />
	 * @param timeToDepotentiate the time that *this should expire at.
	 */
	virtual void ExtendTimeoutUntil(Timestamp timeToDepotentiate);

	/**
	 * PROTEIN BASED. <br />
	 * Removes data from the target <br />
	 */
	virtual void DepotentiateSignal();

	/**
	 * @return what is connected to *this, the source.
	 */
	const Neuron* GetPresynapticNeuron() const;

	/**
	 * @return the Name of what this receives data from, the source.
	 */
	Name GetPresynapticName() const;

	/**
	 * @return the Id of what this receives data from, the source.
	 */
	Id GetPresynapticId() const;

protected:
	const Neuron* mPresynapticNeuron;

	molecular::Protein* mcProcessOutgoing;
	molecular::Protein* mCallback;
	molecular::Protein* mcDepotentiateSignal;

	Timestamp mLastUpdated;

	/**
	 * How long until *this should be removed. <br />
	 */
	Milliseconds mTimeout;
};

} // neural namespace
} // bio namespace
