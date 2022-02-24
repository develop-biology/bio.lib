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

namespace bio {

namespace chemical {
class Substance;
}

namespace genetic {

class Localization;

/**
 * Localizations are a chain of named places.
 * Each place is a different Localization, which has more places (Localizations) around it.
 * This system allows us to traverse arbitrarily complex spaces with ease.
 *
 * In order to use a standard interface with arbitrarily complex containers, we rely on physical::Perspective::AssociateType(), chemical::Excitation, and the chemical/structure system to provide us with a means of translating a LocalizationSite (AssociateType) into a class method (Excitation) that is used to query a container by Name (structure).
 * See genetic/Macros.h for an easy way to define all that (its not nearly as hard to use as it is to implement).
 *
 * To create a localization, first select the kind of place you want from the available LocalizationTypes (in the localization_type namespace).
 * Next, note the Name of the desired place.
 * And, lastly, instantiate a Localization.
 * If you would like to identify a place within another place, simple repeat and set the containing Localization as the m_previous of the first.
 * For example, if you want to identify where the bathroom is within a restaurant, we would start with a Localization like {localization_site::Room(), "Bathroom"}, which might cause us to ask the nearest person for the "Bathroom". Next, we would create another Localization along the lines of {localization_site::StreetAddress(), "MyFavoriteRestaurant"}. In this case, StreetAddress would tell us to use a navigation app and maybe a car or taxi service to "extract" the restaurant form the world. Thus, we end up with all the information necessary to "extract" the "Bathroom" from "MyFavoriteRestaurant".
 */
class Localization
{
public:
	/**
	 * @param site
	 * @param name
	 */
	explicit Localization(
		LocalizationSite site = LocalizationSitePerspective::InvalidId(),
		Name name = NULL
	);

	/**
	 *
	 */
	virtual ~Localization();

	/**
	 * Find some place by following a chain of Localizations.
	 * Will recurse upward, following m_previous for as long as possible.
	 * @param seekIn
	 * @return a Substance somewhere within the Substance provided or NULL.
	 */
	virtual chemical::Substance* Seek(chemical::Substance* seekIn);

	LocalizationSite m_site;
	Name m_name;
	Localization* m_previous;
};

} //genetic namespace
} //bio namespace
