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

#include "bio/molecular/macros/Macros.h"
#include "bio/genetic/common/Types.h"


/**
 * Get all virtual methods defined by genetic::Class.
 * @return function signatures for use in BIO_DISAMBIGUATE_CLASS_METHODS
 */
#define BIO_GET_CLASS_METHODS_FOR_genetic()                                    \
    BIO_GET_CLASS_METHODS_FOR_molecular()

/**
Macro for defining TranscriptionFactors.
*/
#define BIO_TRANSCRIPTION_FACTOR_FUNCTION_BODY(functionName)                   \
BIO_ID_FUNCTION_BODY(                                                          \
    functionName,                                                              \
    ::bio::genetic::TranscriptionFactorPerspective::Instance(),                \
    ::bio::genetic::TranscriptionFactor)


/**
 * To make defining LocalizationSites easier, use this macro to define the function body of your LocalizationSite Function().
 * This will assign a value to a string that is identical to your FunctionName e.g. LocalizationSitePerspective::Instance().GetNameFromId(Value()) would give "Value".
 * This will also help you define the required extraction method (chemical::Excitation*) required for accessing your LocalizationSite.
 * REMINDER: Your LocalizationSite Function()s should be in the ::bio::localization_site namespace.
 */
#define BIO_LOCALIZATION_SITE_FUNCTION_BODY(functionName, toExtract)           \
	BIO_ID_FUNCTION_BODY(                                                      \
		functionName,                                                          \
		::bio::LocalizationSitePerspective::Instance(),                        \
		::bio::LocalizationSite)                                               \
        bool g_##functionName##Registered = ::bio::LocalizationSitePerspective::Instance().AssociateType(g_##functionName, (new BIO_EXCITATION_CLASS(::bio::chemical::LinearStructuralComponent< toExtract >, toExtract, Name, bool)(&::bio::chemical::LinearStructuralComponent< toExtract >::GetByNameImplementation, NULL, false))->AsWave());
