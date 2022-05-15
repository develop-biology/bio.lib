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
#include "bio/physical/common/Class.h"
#include "bio/common/VirtualBase.h"

namespace bio {
namespace physical {

/**
 * Periodic objects do work at regular intervals. <br />
 */
class Periodic :
	public physical::Class< Periodic >,
	protected VirtualBase
{
public:

	/**
	 * Ensure virtual methods point to Class implementations. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(physical,
		Periodic)

	/**
	 * Currently default is set to 200 milliseconds (200000 microseconds). <br />
	 * @return a default value for Periodic constructors.
	 */
	static TimeUS GetDefaultInterval();

	/**
	 * All Periodic objects share the "Periodic" Property. <br />
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
	 * VirtualBase required method. See that class for details (in common/) <br />
	 * @param args
	 */
	virtual void InitializeImplementation(ByteStreams args);

	/**
	 * Peak()s occur at Periodic::mIntervals. <br />
	 * Define your main Periodic logic here. <br />
	 * This method must be fast: <br />
	 *	* do not read slow hardware here <br />
	 *	* do not block for a long time <br />
	 *	* do not sleep <br />
	 * If derived classes must do slow work to oscillate, that slow logic MUST BE placed in a separate thread. <br />
	 * This method would then get the data stored by that thread and returns the data *quickly*. MAKE SURE that the thread never causes a long mutex wait as a side-effect in this Peak method. <br />
	 */
	virtual Code Peak()
	{

		//     YOU CODE GOES HERE!

		return code::NotImplemented();
	}

	/**
	 * Checks the current time & calls Peak, if a long enough interval has passed. <br />
	 * Call this method regularly (i.e. on a clock). <br />
	 */
	virtual void CheckIn();

	/**
	 * Set how quickly *this should Peak() <br />
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
	 * Sets the timestamp of the last time *this Peaked. <br />
	 * USE WITH CAUTION! <br />
	 * @param lastPeak
	 */
	virtual void SetLastPeakTimestamp(Timestamp lastPeak);

	/**
	 * Required method from Wave. See that class for details. <br />
	 * @return a Symmetrical image of *this
	 */
	virtual Symmetry* Spin() const;

	/**
	 * Required method from Wave. See that class for details. <br />
	 * Reconstruct *this from the given Symmetry. <br />
	 * @param symmetry
	 */
	virtual Code Reify(Symmetry* symmetry);

	/**
	 * Wave method. See that class for details. <br />
	 * @return Periodic::GetClassProperties()
	 */
	virtual Properties GetProperties() const;

protected:
	TimeUS mInterval;
	Timestamp mLastPeakTimestamp;
};

} //physical namespace
} //bio namespace
