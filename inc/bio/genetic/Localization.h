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
 * Each place is a different Localization, which has more places (Localizations) within it.
 * This system allows us to traverse arbitrarily complex spaces with ease.
 *
 * In order to use a standard interface with arbitrarily complex containers, we rely on physical::Perspective::AssociateType(), chemical::Excitation, and the chemical/structure system to provide us with a means of translating a LocalizationType (AssociateType) into a class method (Excitation) that is used to query a container by Name (structure).
 * See genetic/Macros.h for an easy way to define all that (its not nearly as hard to use as it is to implement).
 *
 * To create a localization, first select the kind of place you want from the available LocalizationTypes (in the localization_type namespace).
 * Next, note the Name of the desired place.
 * And, lastly, instantiate a Localization.
 * If you would like to identify a place within that place, simple repeat and set the further Localization as the m_next of the first.
 * For example, if you want to identify where the bathroom is within a restaurant, we would start with a Localization along the lines of {localization_type::StreetAddress(), "MyFavoriteRestaurant"}. In this case, StreetAddress would tell us to use a navigation app and maybe a car or taxi service to "extract" the restaurant form the world. Then, we would set the m_next of this Localization to something like {localization_type::Room(), "Bathroom"}, which might cause us to ask the nearest person for "Bathroom", thereby "extracting" the "Bathroom" from "MyFavoriteRestaurant". What you do in the Bathroom is up to you.
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
	 * Will recurse only if done is provided.
	 * Seek recursion stops when m_next is NULL or locale.m_site is invalid (i.e. the default value when not set).
	 * @param locale
	 * @param seekIn
	 * @param done indicates when to stop recursion.
	 * @return a place specified by locale or NULL.
	 */
	static chemical::Substance* Seek(
		const Localization& locale,
		chemical::Substance* seekIn,
		bool* donePtr = NULL
	);

	LocalizationSite m_site;
	Name m_name;
	Localization* m_next;
};

} //genetic namespace
} //bio namespace
