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

#include "bio/physical/common/Class.h"
#include "bio/physical/macro/Macros.h"
#include "Attraction.h"

namespace bio {

/**
 * Affinity is simply a base class for further Affinity specification. <br />
 * They allow you to build a selector that can be compared against Waves later. <br />
 * Unlike Wave::GetResonanceBetween(), Affinity can be either positive or negative and can change what is being compared. <br />
 * <br />
 * NOTE: Affinity is in the main bio namespace for ease of use across domains. <br />
 */
class Affinity :
	public physical::Class< Affinity >
{
public:

	/**
	 * Sync the template-specific Attraction Force with our non-specific Affinity Strength. <br />
	 * bool here is simply a dummy value. <br />
	 */
	typedef physical::Attraction< bool >::Force Strength;

	/**
	 * Ensure virtual methods point to Class implementations. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(physical, Affinity)

	/**
	 *
	 */
	Affinity();

	/**
	 *
	 */
	virtual ~Affinity();

	/**
	 * To get the Affinity::Strength of an interaction, we add all Attraction::Forces together. <br />
	 * Here, negative Attractions (i.e. repulsions) will cause the Affinity to be weaker (i.e. smaller) or negative. <br />
	 * "Abstract"; will always return 0 unless overridden. <br />
	 * @param wave
	 * @return the sum of all Attractions between *this and the wave.
	 */
	virtual Strength GetStrengthOfAttractionTo(const Wave* wave) const;

	/**
	 * Abstract; will always return false unless GetStrengthOfAttractionTo is overridden. <br />
	 * @param wave
	 * @param threshold how high the Affinity::Strength must be to return true.
	 * @return  whether or not the Affinity::Strength of interacting with the wave is greater than the threshold.
	 */
	virtual bool AttractionExists(const Wave* wave, Strength threshold=0) const;

	/**
	 * Ease of use wrapper for AttractionExists(). <br />
	 * @param wave
	 * @return whether or not AttractionExists between *this and wave.
	 */
	virtual bool operator==( const Wave* wave ) const;

};

} //bio namespace
