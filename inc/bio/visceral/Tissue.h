/*
 * This file is a part of the Biology project by eons LLC.
 * Biology (aka Develop Biology) is a framework for approaching software
 * development from a natural sciences perspective.
 *
 * Copyright (C) 2021 Séon O'Shannon & eons LLC
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

#include "bio/visceral/common/Class.h"
#include "bio/chemical/structure/LinearStructuralComponent.h"
#include "bio/molecular/EnvironmentDependent.h"

namespace bio {

namespace genetic {
class Plasmid;
}

namespace cellular{
class Cell;
}

namespace visceral {

/**
Tissues are a container for storing and manipulating many cells at once
*/
class Tissue :
	public Class<Tissue>,
	public chemical::LinearStructuralComponent< const genetic::Plasmid >,
	public chemical::LinearStructuralComponent< cellular::Cell >,
	public chemical::LinearStructuralComponent< Tissue >,
	public molecular::EnvironmentDependent<Tissue> //Not Organ because of LinearStructuralComponent.
{
public:

	Tissue();

	/**
	 * @param id Recommend name=PeriodicTable::Instance().GetIdFromType(*this).
	 */
	explicit Tissue(StandardDimension id);

	/**
	 * @param name Recommend name=PeriodicTable::Instance().GetNameFromType(*this).
	 */
	explicit Tissue(Name name);

	/**
	 * Copies all contents of the given Tissue into *this.
	 * NOTE: Cells are not functionally when copied. To make the new Cells in *this work, you must also call DifferentiateCells().
	 * @param toCopy
	 */
	Tissue(const Tissue& toCopy);

	/**
	 *
	 */
	virtual ~Tissue();

	/**
	 * For all Cells & sub-Tissues in *this:
	 *     1. Injects all Plasmid*s from *this
	 *     2. Transcribes & Translates all Genes
	 *     3. Folds all Proteins.
	 */
	virtual void DifferentiateCells();

};

} //organismal namespace
} //bio namespace
