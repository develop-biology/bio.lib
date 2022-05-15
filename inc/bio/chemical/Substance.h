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

#include "bio/chemical/common/Types.h"
#include "bio/chemical/common/States.h"
#include "bio/chemical/common/Properties.h"
#include "bio/chemical/common/Class.h"
#include "bio/chemical/common/Filters.h"
#include "bio/chemical/macros/Macros.h"
#include "bio/chemical/structure/Structure.h"
#include "bio/chemical/structure/motif/UnorderedMotif.h"

namespace bio {
namespace chemical {

/**
 * A chemical::Substance is just about everything. <br />
 * Substances have Properties and States and that's it. <br />
 * Substances start Enable()d. <br />
 */
class Substance :
	virtual public Structure,
	public chemical::Class< Substance >,
	public UnorderedMotif< Property >,
	public UnorderedMotif< State >
{
public:

	BIO_DEFAULT_IDENTIFIABLE_CONSTRUCTORS_WITH_CTOR_COMMON(chemical,
		Substance,
		&SubstancePerspective::Instance(),
		filter::Chemical())

	/**
	 * Ensure virtual methods point to Class implementations. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(chemical,
		Substance)

	/**
	 * @param properties
	 * @param states
	 */
	Substance(
		const typename UnorderedMotif< Property >::Contents* properties,
		const typename UnorderedMotif< State >::Contents* states
	);

	/**
	 *
	 */
	virtual ~Substance();

	/**
	 * Helper method for setting the Enabled() State. <br />
	 * May be overridden to add custom behavior. <br />
	 */
	virtual void Enable();

	/**
	 * Helper method for removing the Enabled() State. <br />
	 * May be overridden to add custom behavior. <br />
	 */
	virtual void Disable();

	/**
	 * Helper method for checking the Enabled() State. <br />
	 * May be overridden to add custom checks. <br />
	 * @return whether or not *this has the Enabled() State.
	 */
	virtual bool IsEnabled() const;

private:
	void CtorCommon();
};

} //chemical namespace
} //bio namespace
