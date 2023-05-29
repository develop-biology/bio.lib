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

#include "bio/cellular/wave/CheckInCarrierWave.h"
#include "bio/cellular/wave/SetIntervalCarrierWave.h"
#include "bio/cellular/protein/Apoptosis.h"
#include "bio/cellular/protein/Crest.h"
#include "bio/genetic/common/Class.h"
#include "bio/genetic/affinity/Affinity.h"
#include "bio/genetic/Expressor.h"
#include "bio/molecular/Vesicle.h"
#include "bio/physical/Periodic.h"

namespace bio {
namespace cellular {

/**
 * A cellular::Class extends genetic::Class <br />
 * Class in other namespaces will grow to include more complex, templated logic. <br />
 * This pattern prevents you from having to define virtual methods each of your child classes, so long as you always derive from the appropriate Class<T>. <br />
 * @tparam T a pointer to a class type.
 */
template < typename T >
class Class :
	public genetic::Class< T >,
	virtual public genetic::Expressor,
	virtual public physical::Periodic,
	virtual public molecular::Vesicle
{
public:

	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(genetic, T)

	/**
	 * Providing just the object should not Initialize anything. <br />
	 * For default constructors of virtually inherited classes. <br />
	 * @param object
	 */
	Class(
		T* object,
		Filter filter = filter::Default(),
		Milliseconds interval = GetDefaultInterval()) //unused.
		:
		genetic::Class< T >(
			object,
			filter
		)
	{

	}

	/**
	 * @param object
	 * @param name
	 * @param perspective
	 * @param filter
	 * @param interval
	 */
	Class(
		T* object,
		const Name& name,
		Filter filter = filter::Default(),
		Milliseconds interval = GetDefaultInterval())
		:
		genetic::Class< T >(
			object,
			name,
			filter
		)
	{
		physical::Periodic::Initialize(interval);
	}

	/**
	 * @param object
	 * @param id
	 * @param perspective
	 * @param filter
	 * @param interval
	 */
	Class(
		T* object,
		const Id& id,
		Filter filter = filter::Default(),
		Milliseconds interval = GetDefaultInterval())
		:
		genetic::Class< T >(
			object,
			id,
			filter
		)
	{
		physical::Periodic::Initialize(interval);
	}


	/**
	 *
	 */
	virtual ~Class()
	{

	}

	virtual Code CreateDefaultProteins()
	{
		Add< molecular::Protein* >(new Apoptosis("Apoptosis"));
		Add< molecular::Protein* >(new class Crest("Crest"));

		physical::Wave createDefaultProteinsCarrierWave;
		createDefaultProteinsCarrierWave.Modulate(BIO_EXCITATION_CLASS(genetic::Expressor, Code)(&genetic::Expressor::CreateDefaultProteins));
		return Attenuate(&createDefaultProteinsCarrierWave);
	}

	virtual Code CacheProteins()
	{
		mcApoptosis = GetByName< molecular::Protein* >("Apoptosis");
		mcCrest = GetByName< molecular::Protein* >("Crest");

		physical::Wave cacheProteinsCarrierWave;
		cacheProteinsCarrierWave.Modulate(BIO_EXCITATION_CLASS(genetic::Expressor, Code)(&genetic::Expressor::CacheProteins));
		return Attenuate(&cacheProteinsCarrierWave);
	}

	virtual Code Apoptose()
	{
		(*mcApoptosis)();

		physical::Wave apoptosisCarrierWave;
		apoptosisCarrierWave.Modulate(BIO_EXCITATION_CLASS(genetic::Expressor, Code)(&genetic::Expressor::Apoptose));
		return Attenuate(&apoptosisCarrierWave);
	}

	/**
	 * Crest()s occur at Periodic::mIntervals. <br />
	 * Define your main Periodic logic here. <br />
	 * This method must be fast: <br />
	 *	* do not read slow hardware here <br />
	 *	* do not block for a long time <br />
	 *	* do not sleep <br />
	 * If derived classes must do slow work to Crest, that slow logic MUST BE placed in a separate thread. <br />
	 * This method would then get the data stored by that thread and returns the data *quickly*. MAKE SURE that the thread never causes a long mutex wait as a side-effect in this Crest method. <br />
	 * Please call this method when you're done :) <br />
	 */
	virtual Code Crest()
	{
		return (*mcCrest)();
	}

	/**
	 * physical::Periodic method; see that class for details. <br />
	 * @return physical::Periodic::CheckIn()
	 */
	virtual bool CheckIn()
	{
		//static class for speed.
		static CheckInCarrierWave sCrestCarrierWave;
		Attenuate(&sCrestCarrierWave);

		return physical::Periodic::CheckIn();
	}

	/**
	 * Calls SetInterval on all Periodic components of *this.
	 * Override of physical::Periodic method; see that class for more information.
	 */
	virtual Code SetInterval(Milliseconds period)
	{
		//nonstatic class for simplicity, I guess...?
		SetIntervalCarrierWave setInterval(period);
		return Attenuate(&setInterval);
	}

protected:
	molecular::Protein* mcApoptosis;
	molecular::Protein* mcCrest;
};

} //cellular namespace
} //bio namespace
