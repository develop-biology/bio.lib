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
 *
 * In order to use a standard interface with arbitrarily complex containers, we rely on physical::Perspective::AssociateType(), chemical::Excitation, and the chemical/structure system to provide us with a means of translating a Site (AssociateType) into a class method (Excitation) that is used to query a container by Name (structure). <br />
 * See genetic/Macros.h for an easy way to define all that (its not nearly as hard to use as it is to implement). <br />
 *
 * For specifying a series of places, we use the already existing physical::Wave Modulation system in reverse order; meaning the Modulated signal is what should be evaluated BEFORE *this. Evaluation here means Localization::Seek() (past tense as Sought). <br />
 * If you can imagine a real-life wave being modulated, that is, having smaller waves that it carries, then visualizing this system should be easy: the largest wave is what we really want to do, which can only happen once the smaller waves have been resolved; so, we take a large wave of interest and look deeper and deeper into its more subtle fluctuations as we search larger and larger contexts, until we find what we are seeking. <br />
 *
 * To create a localization, first select the kind of place you want from the available LocalizationTypes (in the localization_type namespace). <br />
 * Next, note the Name of the desired place. <br />
 * And, lastly, instantiate a Localization. <br />
 * If you would like to identify a place within another place, simple repeat and Modulate the first Localization with the second. <br />
 * For example, if you want to identify where the bathroom is within a restaurant, we would start with a Localization like {localization_site::Room(), "Bathroom"}, which might cause us to ask the nearest person for the "Bathroom". Next, we would create another Localization along the lines of {localization_site::StreetAddress(), "MyFavoriteRestaurant"}. In this case, StreetAddress would tell us to use a navigation app and maybe a car or taxi service to "extract" the restaurant form the world. Then, we say bathroomLocalization % restaurantLocalization. Thus, we end up with all the information necessary to "extract" the "Bathroom" from "MyFavoriteRestaurant". <br />
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
	 * @param site
	 * @param name
	 */
	explicit Localization(
		Site site = LocalizationSitePerspective::InvalidId(),
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
	 * Get mSite. <br />
	 * @return mSite.
	 */
	virtual Site GetSite() const;

	/**
	 * Set mSite. <br />
	 * @param site
	 */
	virtual void SetSite(Site site);

	/**
	 * Get the Name to use with mSite. <br />
	 * @return mName.
	 */
	virtual Name GetNameOfSite() const;

	/**
	 * Set mName. <br />
	 * @param name
	 */
	virtual void SetNameOfSite(const Name& name);

protected:
	/**
	 * To be run at the top of Seek. <br />
	 * @param seekIn
	 * @return the result of Seeking through all Modulated Localizations.
	 */
	chemical::Substance* ResolvePrevious(chemical::Substance* seekIn) const;

	Site mSite;
	Name mName;
	Localization* mPrevious;
	chemical::ExcitationBase* mcMethod; //cached pointer to site-associated function pointer.
};

} //genetic namespace
} //bio namespace
