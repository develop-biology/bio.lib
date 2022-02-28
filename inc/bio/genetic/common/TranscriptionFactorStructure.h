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
#include "bio/chemical/common/Class.h"
#include "bio/chemical/structure/StructuralComponent.h"

namespace bio {
namespace genetic {

/**
 * chemical::StructuralComponent<TranscriptionFactor> is an ambiguous base of Expressor & Gene, so we use an intermediate class to disambiguate.
 */
class TranscriptionFactorStructure :
	public chemical::Class< TranscriptionFactorStructure >,
	public chemical::StructuralComponent< TranscriptionFactor >
{
public:
	/**
	 * Ensure virtual methods point to Class implementations.
	 */
	BIO_DISAMBIGUATE_CLASS_METHODS(chemical,
		TranscriptionFactorStructure)

	TranscriptionFactorStructure()
		:
		chemical::Class< TranscriptionFactorStructure >(this)
	{
	}

	virtual ~TranscriptionFactorStructure()
	{
	}
};

} //genetic namespace
} //bio namespace
