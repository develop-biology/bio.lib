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

/**
 * This is not for you.
 */
#define BIO_SITE_FUNCTION(perspective, site, type, function, typeTuple, argTuple)\
	bool g_##site##Registered =                                                \
		::bio::perspective::Instance().AssociateType(                          \
			g_##site,                                                          \
			(                                                                  \
				new BIO_EXCITATION_CLASS(                                      \
					::bio::chemical::LinearMotif< type >,                      \
					type,                                                      \
					BIO_EXPAND_TUPLE typeTuple                                 \
				)(                                                             \
					&::bio::chemical::LinearMotif< type >::function,           \
					BIO_EXPAND_TUPLE argTuple                                  \
				)                                                              \
			)->AsWave()                                                        \
		);

/**
 * To make defining Sites easier, use this macro to define the function body of your Site Function().
 * This will assign a value to a string that is identical to your FunctionName e.g. LocalizationSitePerspective::Instance().GetNameFromId(Value()) would give "Value".
 * This will also help you define the required extraction method (chemical::Excitation*) required for accessing your Site.
 * REMINDER: Your Site Function()s should be in the ::bio::localization_site namespace.
 */
#define BIO_LOCALIZATION_SITE_FUNCTION_BODY(functionName, toExtract)           \
	BIO_ID_FUNCTION_BODY(                                                      \
		functionName##LocalizationSite,                                        \
		::bio::LocalizationSitePerspective::Instance(),                        \
		::bio::Site)                                                           \
	BIO_SITE_FUNCTION(LocalizationSitePerspective, functionName##LocalizationSite, toExtract, GetByNameImplementation, (Name), (NULL))

/**
 * To make defining Sites easier, use this macro to define the function body of your Site Function().
 * This will assign a value to a string that is identical to your FunctionName e.g. InsertionSitePerspective::Instance().GetNameFromId(Value()) would give "Value".
 * This will also help you define the required insertion method (chemical::Excitation*) required for using your Site.
 * REMINDER: Your Site Function()s should be in the ::bio::insertion_site namespace.
 */
#define BIO_INSERTION_SITE_FUNCTION_BODY(functionName, toInsert)               \
	BIO_ID_FUNCTION_BODY(                                                      \
		functionName##InsertionSite,                                           \
		::bio::InsertionSitePerspective::Instance(),                           \
		::bio::Site)                                                           \
	BIO_SITE_FUNCTION(InsertionSitePerspective, functionName##InsertionSite, toInsert, AddImplementation, (toInsert), (NULL))

/**
 * Ease of use method for declaring all kinds of sites at once.
 */
#define BIO_SITE(functionName)                                                 \
	namespace site {                                                           \
		Site functionName();                                                   \
	}

/**
 * Ease of use method of defining all kinds of sites at once.
 */
#define BIO_SITE_FUNCTION_BODY(functionName, type)                             \
	namespace site {                                                           \
		BIO_LOCALIZATION_SITE_FUNCTION_BODY(functionName, type)                \
		BIO_INSERTION_SITE_FUNCTION_BODY(functionName, type)                   \
	}
