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

#include "Gene.h"

namespace bio {
namespace genetic {

/**
 * RNA is responsible for interacting with Genes and making Expression possible.
 */
class RNA :
	public Class< RNA >,
	public chemical::LinearStructuralComponent< Gene* >
{
public:
	/**
	 * Ensure virtual methods point to Class implementations.
	 */
	BIO_DISAMBIGUATE_CLASS_METHODS(genetic,
		RNA)

	/**
	 * Standard ctors.
	 * NOTE: RNA does not always have to be Identifiable. We simply make it so for ease-of use.
	 */
	BIO_DEFAULT_IDENTIFIABLE_CONSTRUCTORS(genetic,
		RNA,
		&genetic::RNAPerspective::Instance(),
		filter::Genetic())

};

} //genetic namespace
} //bio namespace
