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

#include "bio/cellular/common/Class.h"
#include "bio/cellular/common/Types.h"
#include "bio/cellular/common/Filters.h"
#include "bio/cellular/macros/Macros.h"
#include "bio/cellular/Organelle.h"
#include "bio/cellular/Cell.h"
#include "bio/genetic/Plasmid.h"
#include "bio/molecular/EnvironmentDependent.h"
#include "bio/chemical/structure/LinearStructuralComponent.h"

namespace bio {
namespace cellular {

class Cell;

/**
 * Tissues are a container for storing and manipulating many cells at once
 * Tissues can hold other Tissues, so create them in any structure you need.
 * Once a group of Tissues is functioning as desired, it is recommended that you package them into an Organ.
 */
class Tissue :
	public Class<Tissue>,
	public chemical::LinearStructuralComponent< genetic::Plasmid* >,
	public chemical::LinearStructuralComponent< Cell* >,
	public chemical::LinearStructuralComponent< Tissue* >,
	public molecular::EnvironmentDependent<Tissue> //Not Organ.
{
public:

	/**
	 * Ensure virtual methods point to Class implementations.
	 */
	BIO_DISAMBIGUATE_CLASS_METHODS(cellular,
		Tissue)

	/**
	 * Standard ctors.
	 */
	 BIO_DEFAULT_IDENTIFIABLE_CONSTRUCTORS(cellular,
		Tissue,
		&TissuePerspective::Instance(),
		filter::Cellular())


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
	virtual Code DifferentiateCells();

};

} //cellular namespace
} //bio namespace