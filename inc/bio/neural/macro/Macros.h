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

#include "bio/genetic/macro/Macros.h"

/**
 * Get all virtual methods defined by neural::Class. <br />
 * @return function signatures for use in BIO_DISAMBIGUATE_REQUIRED_CLASS_METHODS
 */
#define BIO_GET_REQUIRED_CLASS_METHODS_FOR_neural()                            \
    BIO_GET_REQUIRED_CLASS_METHODS_FOR_cellular()

/**
 * Get all virtual methods defined by neural::Class. <br />
 * @return function signatures for use in BIO_DISAMBIGUATE_OPTIONAL_CLASS_METHODS
 */
#define BIO_GET_OPTIONAL_CLASS_METHODS_FOR_neural()                            \
    BIO_GET_OPTIONAL_CLASS_METHODS_FOR_cellular()

/**
 * To make defining Neuron::FiringConditions easier, use this macro to define the function body of your Neuron::FiringCondition Function(). <br />
 * This will assign a value to a string that is identical to your FunctionName e.g. SafelyAccess<Neuron::FiringConditionPerspective>()->GetNameFromId(Value()) would give "Value". <br />
 * REMINDER: Your Neuron::FiringCondition Function()s should be in the ::bio::neural::Neuron namespace. <br />
 */
#define BIO_FIRING_CONDITION_FUNCTION_BODY(functionName)                       \
BIO_ID_FUNCTION_BODY(                                                          \
    functionName,                                                              \
    ::bio::neural::FiringConditionPerspective::Instance(),                     \
    ::bio::neural::FiringCondition)

/**
 * To make defining Potentials easier, use this macro to define the function body of your Potential Function().
 * This will assign a value to a string that is identical to your FunctionName e.g. PotentialPerspective::Instance().NameFromId(Value()) would give "Value"
 * Necessitates that functionName be a part of the "potential" namespace and that any other namespaces are already specified (e.g. using namespace)
 */
#define BIO_POTENTIAL_FUNCTION_BODY(functionName)                              \
BIO_ID_FUNCTION_BODY(                                                          \
    functionName,                                                              \
    bio::PotentialPerspective::Instance(),                                     \
    bio::Potential)

/**
 * To make defining Features easier, use this macro to define the function body of your Feature Function().
 * This will assign a value to a string that is identical to your FunctionName e.g. FeaturePerspective::Instance().NameFromId(Value()) would give "Value"
 * Necessitates that functionName be a part of the "potential" namespace and that any other namespaces are already specified (e.g. using namespace)
 */
#define BIO_FEATURE_FUNCTION_BODY(functionName)                                \
BIO_ID_FUNCTION_BODY(                                                          \
    functionName,                                                              \
    bio::FeaturePerspective::Instance(),                                       \
    bio::Feature)
