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

#include "bio/genetic/common/Types.h"
#include "bio/physical/common/Class.h"
#include "bio/physical/macro/Macros.h"
#include "bio/chemical/reaction/Excitation.h"

namespace bio {

namespace chemical {
class Substance;
}

namespace genetic {

class Localization;

/**
 * Localizations are a chain of named places. <br />
 * Each place is a different Localization, which has more places (Localizations) around it. <br />
 * This system allows us to traverse arbitrarily complex spaces with ease. <br />
 * <br />
 * Localizations use the "Move" Affinity. You don't need to know anything about this, its all taken care of for you. If you want to use a different Affinity, create a child of *this and customize it as you'd like; see Insertion for an example. <br />
 * <br />
 * In order to use a standard interface with arbitrarily complex containers, we rely on the Translocator's Location + Affinity + peptidase system to provide us with a means of translating a Location into a class method (peptidase / Excitation) that is used to query a container by Name. <br />
 * See genetic/Macros.h for an easy way to define all that (its not nearly as hard to use as it is to implement). <br />
 * <br />
 * For specifying a series of places, we use the already existing physical::Wave Modulation system in reverse order; meaning the Modulated signal is what should be evaluated BEFORE *this. Evaluation here means Localization::Seek() (past tense as Sought). <br />
 * If you can imagine a real-life wave being modulated, that is, having smaller waves that it carries, then visualizing this system should be easy: the largest wave is what we really want to do, which can only happen once the smaller waves have been resolved; so, we take a large wave of interest and look deeper and deeper into its more subtle fluctuations as we search larger and larger contexts, until we find what we are seeking. <br />
 * <br />
 * To create a localization, first select the kind of place you want from the available Locations (in the location namespace). <br />
 * Next, note the Name of the desired place. <br />
 * And, lastly, instantiate a Localization. <br />
 * If you would like to identify a place within another place, simple repeat and Modulate the first Localization with the second. <br />
 * For example, if you want to identify where the bathroom is within a restaurant, we would start with a Localization like {location::Room(), "Bathroom"}, which might cause us to ask the nearest person for the "Bathroom". Next, we would create another Localization along the lines of {location::StreetAddress(), "MyFavoriteRestaurant"}. In this case, StreetAddress would tell us to use a navigation app and maybe a car or taxi service to "extract" the restaurant form the world. Then, we say bathroomLocalization % restaurantLocalization. Thus, we end up with all the information necessary to "extract" the "Bathroom" from "MyFavoriteRestaurant". <br />
 */
class Localization :
	physical::Class< Localization >
{
public:

	/**
	 * Ensure virtual methods point to Class implementations. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(physical, Localization)


	/**
	 * @param location
	 * @param name
	 */
	explicit Localization(
		Location location = LocalizationLocationPerspective::InvalidId(),
		const Name& name = NULL
	);

	/**
	 *
	 */
	virtual ~Localization();

	/**
	 * Find some place by following a chain of Localizations. <br />
	 * Will recurse upward, following mPrevious for as long as possible. <br />
	 * @param seekIn
	 * @return a Substance somewhere within the Substance provided or NULL.
	 */
	virtual chemical::Substance* Seek(chemical::Substance* seekIn) const;

	/**
	 * Get mLocation. <br />
	 * @return mLocation.
	 */
	virtual Location GetLocation() const;

	/**
	 * Set mLocation. <br />
	 * @param location
	 */
	virtual void SetLocation(Location location);

	/**
	 * Get the Name to use with mLocation. <br />
	 * @return mName.
	 */
	virtual Name GetNameOfLocation() const;

	/**
	 * Set mName. <br />
	 * @param name
	 */
	virtual void SetNameOfLocation(const Name& name);

protected:
	/**
	 * To be run at the top of Seek. <br />
	 * @param seekIn
	 * @return the result of Seeking through all Modulated Localizations.
	 */
	chemical::Substance* ResolvePrevious(chemical::Substance* seekIn) const;

	Location mLocation;
	Name mName;
	Localization* mPrevious;
	chemical::ExcitationBase* mcMethod; //cached pointer to location-associated function pointer.
};

} //genetic namespace
} //bio namespace
