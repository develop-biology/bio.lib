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

#include "Types.h"
#include "Class.h"
#include "bio/common/VirtualBase.h"

namespace bio {
namespace physical {

/**
 * Periodic objects do work at regular intervals.
 */
class Periodic :
	public physical::Class< Periodic >,
	protected VirtualBase
{
public:

	/**
	 * Ensure virtual methods point to Class implementations.
	 */
	BIO_DISAMBIGUATE_CLASS_METHODS(physical,
		Periodic)

	/**
	 * Currently default is set to 200 milliseconds (200000 microseconds).
	 * @return a default value for Periodic constructors.
	 */
	static TimeUS GetDefaultInterval();

	/**
	 * All Periodic objects share the "Periodic" Property.
	 * @return {property::Periodic()}
	 */
	static Properties GetClassProperties();

	/**
	 * @param interval the period that Peak() wants to be called in microseconds.
	 */
	Periodic(TimeUS interval = GetDefaultInterval());

	/**
	 *
	 */
	virtual ~Periodic();

	/**
	 * VirtualBase required method. See that class for details (in common/)
	 * @param args
	 */
	virtual void InitializeImplementation(ByteStreams args);

	/**
	 * Peak()s occur at Periodic::m_intervals.
	 * Define your main Periodic logic here.
	 * This method must be fast:
	 *	* do not read slow hardware here
	 *	* do not block for a long time
	 *	* do not sleep
	 * If derived classes must do slow work to oscillate, that slow logic MUST BE placed in a separate thread.
	 * This method would then get the data stored by that thread and returns the data *quickly*. MAKE SURE that the thread never causes a long mutex wait as a side-effect in this Peak method.
	 */
	virtual void Peak()
	{
		//Your code goes here!
	}

	/**
	 * Checks the current time & calls Peak, if a long enough interval has passed.
	 * Call this method regularly (i.e. on a clock).
	 */
	virtual void CheckIn();

	/**
	 * Set how quickly *this should Peak()
	 * @param interval
	 */
	virtual void SetInterval(TimeUS interval);

	/**
	 * @return the time interval between Peaks of *this, in microseconds.
	 */
	TimeUS GetInterval() const;

	/**
	 * @return the last time *this Peaked.
	 */
	Timestamp GetTimeLastPeaked() const;

	/**
	 * @return the time interval between Peaks of *this, in seconds.
	 */
	float GetIntervalInSeconds() const;

	/**
	 * Sets the timestamp of the last time *this Peaked.
	 * USE WITH CAUTION!
	 * @param lastPeak
	 */
	virtual void SetLastPeakTimestamp(Timestamp lastPeak);

	/**
	 * Required method from Wave. See that class for details.
	 * @return a Symmetrical image of *this
	 */
	virtual Symmetry* Spin() const;

	/**
	 * Required method from Wave. See that class for details.
	 * Reconstruct *this from the given Symmetry.
	 * @param symmetry
	 */
	virtual Code Reify(Symmetry* symmetry);

	/**
	 * Wave method. See that class for details.
	 * @return Periodic::GetClassProperties()
	 */
	virtual Properties GetProperties() const;

protected:
	TimeUS m_interval;
	Timestamp m_lastPeakTimestamp;
};

} //physical namespace
} //bio namespace
