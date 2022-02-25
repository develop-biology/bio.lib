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

#include "bio/chemical/common/Types.h"
#include "bio/chemical/common/States.h"
#include "bio/chemical/common/Properties.h"
#include "bio/chemical/common/Class.h"
#include "bio/chemical/macros/Macros.h"
#include "bio/chemical/common/Filters.h"
#include "bio/chemical/structure/StructuralComponent.h"

namespace bio {
namespace chemical {


/**
 * Apparently, C++ forbids direct inheritance of the same base class, even when using different templates.
 * i.e. public StructuralComponent< Property >, public StructuralComponent< State > is not valid because Property & State are the same type / size.
 * To work around this, we create 2 distinct ____Structure base classes for Substance to derive from.
 */
class PropertyStructure: public StructuralComponent< Property >
{
public:
	/**
	 *
	 */
	PropertyStructure();

	/**
	 * @param properties
	 */
	explicit PropertyStructure(
		const typename StructuralComponent< Property >::Contents& properties
	);

	virtual ~PropertyStructure();
};

/**
 * Apparently, C++ forbids direct inheritance of the same base class, even when using different templates.
 * i.e. public StructuralComponent< Property >, public StructuralComponent< State > is not valid because Property & State are the same type / size.
 * To work around this, we create 2 distinct ____Structure base classes for Substance to derive from.
 */
class StateStructure: public StructuralComponent< State >
{
public:
	/**
	 *
	 */
	StateStructure();

	/**
	 * @param properties
	 */
	explicit StateStructure(
		const typename StructuralComponent< State >::Contents& states
	);

	virtual ~StateStructure();
};

/**
 * A chemical::Substance is just about everything.
 * Substances have Properties and States and that's it.
 * Substances start Enable()d.
 */
class Substance :
	public chemical::Class< Substance >,
	public PropertyStructure,
	public StateStructure
{
public:
	/**
	 * Ensure virtual methods point to Class implementations.
	 */
	BIO_DISAMBIGUATE_CLASS_METHODS(chemical,
		Substance)

	/**
	 *
	 */
	Substance();

	/**
	 * @param name
	 * @param perspective
	 */
	Substance(
		Name name,
		physical::Perspective< StandardDimension >* perspective,
		Filter filter = filter::Chemical());

	/**
	 * @param id
	 * @param perspective
	 */
	Substance(
		Id id,
		physical::Perspective< StandardDimension >* perspective,
		Filter filter = filter::Chemical());

	/**
	 * @param properties
	 * @param states
	 */
	explicit Substance(
		const typename StructuralComponent< Property >::Contents& properties,
		const typename StructuralComponent< State >::Contents& states
	);

	/**
	 *
	 */
	virtual ~Substance();

	/**
	 * Helper method for setting the Enabled() State.
	 * May be overridden to add custom behavior.
	 */
	virtual void Enable();

	/**
	 * Helper method for removing the Enabled() State.
	 * May be overridden to add custom behavior.
	 */
	virtual void Disable();

	/**
	 * Helper method for checking the Enabled() State.
	 * May be overridden to add custom checks.
	 * @return whether or not *this has the Enabled() State.
	 */
	virtual bool IsEnabled() const;

private:
	void CtorCommon();
};

} //chemical namespace
} //bio namespace
