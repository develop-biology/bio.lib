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
 * For ease of use for defining Singleton Perspectives. <br />
 */
#define BIO_PERSPECTIVE_SINGLETON(className, dimension)                        \
BIO_SINGLETON(className, ::bio::physical::Perspective<dimension>)

/**
 * For ease of use for defining Singleton TypedPerspectives. <br />
 */
#define BIO_TYPED_PERSPECTIVE_SINGLETON(className, dimension)                  \
BIO_SINGLETON(className, ::bio::physical::TypedPerspective<dimension>)

/**
 * Ease of use for defining Ids. <br />
 * For more on DIMENSIONs, Ids, etc., see Perspective.h and Identifiable.h <br />
 * NOTE: this method MUST be called from the ::bio namespace (see BIO_STRONG_TYPEDEF for why). <br />
 */
#define BIO_ID_WITH_PLURAL(className, pluralName, dimension)                   \
BIO_STRONG_TYPEDEF(dimension, className, 0)                                    \
typedef ::bio::Arrangement<className> pluralName;

/**
 * Define a BIO_ID with "classNames" as plural (e.g. Ids) <br />
 * NOTE: this method MUST be called from the ::bio namespace (see BIO_STRONG_TYPEDEF for why). <br />
 */
#define BIO_ID(className, dimension)                                           \
BIO_ID_WITH_PLURAL(className, className##s, dimension)

/**
 * To make defining ids easier, use this macro to define the function body of your Id Function(). <br />
 * This will assign a value to a string that is identical to your FunctionName e.g. SafelyAccess<MyPerspective>()->GetNameFromId(Value()) would give "Value". <br />
 * Necessitates that functionName be a part of any namespaces are already specified (e.g. using namespace somewhere above a call to this macro). <br />
 */
#define BIO_ID_FUNCTION_BODY(functionName, perspective, dimension)             \
dimension functionName()                                                       \
{                                                                              \
    static ::bio::CachedId< dimension >                                        \
        s##functionName(#functionName, perspective);                           \
    return s##functionName;                                                    \
}

/**
 * This is the preferred design pattern if using singletons and a custom dimension <br />
 * NOTE: this method MUST be called from the ::bio namespace (see BIO_STRONG_TYPEDEF for why). <br />
 */
#define BIO_ID_WITH_PERSPECTIVE(className, dimension)                          \
BIO_ID(className, dimension)                                                   \
BIO_PERSPECTIVE_SINGLETON(className##Perspective, className);

/**
 * For when the plural of className isn't "classNames" (e.g. Properties or Axes) <br />
 * NOTE: this method MUST be called from the ::bio namespace (see BIO_STRONG_TYPEDEF for why). <br />
 */
#define BIO_ID_WITH_PERSPECTIVE_WITH_PLURAL(className, pluralName, dimension)  \
BIO_ID_WITH_PLURAL(className, pluralName, dimension)                           \
BIO_PERSPECTIVE_SINGLETON(className##Perspective, className);

/**
 * For when you'd like to define a BIO_ID with a TypedPerspective. <br />
 */
#define BIO_ID_WITH_TYPED_PERSPECTIVE(className, dimension)                    \
BIO_ID(className, dimension)                                                   \
BIO_TYPED_PERSPECTIVE_SINGLETON(className##Perspective, className);

/**
 * For when you'd like to define a BIO_ID with a TypedPerspective and a custom plural. <br />
 */
#define BIO_ID_WITH_TYPED_PERSPECTIVE_WITH_PLURAL(                             \
	className,                                                                 \
	pluralName,                                                                \
	dimension)                                                                 \
BIO_ID_WITH_PLURAL(className, pluralName, dimension)                           \
BIO_TYPED_PERSPECTIVE_SINGLETON(className##Perspective, className);
