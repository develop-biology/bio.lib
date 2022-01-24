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

#include "Types.h"
#include "States.h"
#include "Properties.h"
#include "Class.h"
#include "bio/chemical/arrangment/StructuralComponent.h"

namespace bio {
namespace chemical {

/**
 * A chemical::Substance is just about everything.
 * Substances have Properties and States and that's it.
 * Substances start Enable()d.
 */
class Substance :
	virtual public physical::Identifiable<StandardDimension>,
	public Class<Substance>,
	public StructuralComponent<Property>,
	public StructuralComponent<State>
{
public:
	/**
	 *
	 */
	Substance();

	/**
	 * @param name
	 * @param perspective
	 */
	Substance(Name name, physical::Perspective<StandardDimension>* perspective, Filter filter = filter::Chemical());

	/**
	 * @param id
	 * @param perspective
	 */
	Substance(Id id, physical::Perspective<StandardDimension>* perspective, Filter filter = filter::Chemical());

	/**
	 * @param properties
	 * @param states
	 */
	explicit Substance(
		typename const StructuralComponent<Property>::Contents& properties,
		typename const StructuralComponent<State>::Contents& states);

	/**
	 *
	 */
	virtual ~Substance();

	/**
	 * This method does way more than it should reasonably be able to.
	 * Here, we take advantage of some of the Biology features that are starting to form. Primarily, we leverage physical::Properties, Bonds (per Atom), and Reactions to search through the pseudo-vtable of Atom, find all StructuralComponents in *this and attempt to Import the corresponding StructuralComponents of other.
	 * This method side-steps the typical inheritance encapsulation in order to prevent child classes from having to override this method and account for each new StructuralComponent they add. In other words, complexity here removes repeated code downstream.
	 * @param other
	 */
	virtual void ImportAll(const Substance* other);

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

	/**
	 * Checks if *this has the given Property.
	 * See physical/Types.h for more on Property.
	 * @param property
	 * @return if this->Has<Property>(property).
	 */
	virtual bool ProbeFor(Property property);

	/**
	 * Checks if *this has all the given Properties.
	 * See ProbeFor(Property).
	 * @param properties
	 * @return if this->HasAll<Property>(properties).
	 */
	virtual bool ProbeFor(Properties properties);

private:
	void CtorCommon();
};

} //chemical namespace
} //bio namespace