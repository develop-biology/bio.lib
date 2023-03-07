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

#include "bio/common/Types.h"
#include "bio/common/type/IsPrimitive.h"
#include "bio/physical/cache/CachedId.h"
#include "bio/physical/macro/Macros.h"
#include "bio/physical/relativity/Perspective.h"
#include "bio/physical/relativity/TypedPerspective.h"

namespace bio {

/**
 * Id type used for all Ids until Named classes can be properly derived from. See physical::Identifiable.h for more info. <br />
 * Id is NOT used when the type itself is the id, e.g. for Code, etc. <br />
 */
BIO_ID_WITH_PERSPECTIVE(Id, uint32_t)

/**
 * Value returned by many bio methods. <br />
 * You may make your own Codes by using the macro defined in common/Codes.h <br />
 */
BIO_ID_WITH_PERSPECTIVE(Code, uint8_t)

/**
 * States determine the condition of an object (e.g. a chemical::Substance) at runtime. <br />
 * The most common State is Enabled() (see "bio/chemical/States.h") <br />
 */
BIO_ID_WITH_PERSPECTIVE(State, uint8_t)

/**
 * Properties are feature flags that give some hint of what a Wave can do (i.e. be cast as). <br />
 * For example, having the Periodic() Property means the class derives from Periodic and thus implements the Peak() method. <br />
 * You might not know that the derived Wave included inheritance of Periodic without being able to GetResonanceBetween() the Wave* and the Periodic() Property. See Wave.h for more info on Resonance. <br />
 * <br />
 * While the State of an object might change often, the Properties should remain constant. However, that is not enforced. The properties of water change when its chemical state changes from liquid to solid, so the Properties of your objects could change in whatever way you'd like, though doing so is generally not recommended. <br />
 */
BIO_ID_WITH_PERSPECTIVE_WITH_PLURAL(Property, Properties, uint8_t)

/**
 * SymmetryTypes determine what to do with a particular Symmetry. <br />
 * They should contain the generic building blocks of all Axes (e.g. programming languages) such that Waves can be Spun around any Axis. <br />
 */
BIO_ID_WITH_PERSPECTIVE(SymmetryType, uint8_t)

/**
 * Filters are used for logging and taking subsets of data. <br />
 */
BIO_ID_WITH_PERSPECTIVE(Filter,	uint8_t)

/**
 * Superpositions determine how multiple Waves combine. <br />
 * For example, a Superposition could act typically and sum the 2 waves, or it could defy the principle of superposition and establish at least one wave as a soliton. <br />
 * See Wave::Superpose() for more info. <br />
 */
BIO_ID_WITH_TYPED_PERSPECTIVE(Superposition, uint8_t)

} //bio namespace

namespace bio {
namespace physical {

class Symmetry;
//typedef ::bio::Arrangement<Symmetry*> Symmetries; //From Wave.h
BIO_PERSPECTIVE_SINGLETON(SymmetryPerspective, Id)

} //physical namespace
} //bio namespace
