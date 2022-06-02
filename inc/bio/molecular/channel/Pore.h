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

#include "bio/molecular/Surface.h"
#include "TransMembraneDomain.h"

namespace bio {
namespace molecular {

/**
 * Pores are simply Surfaces with TransMembraneDomains. <br />
 * The role of a Pore is to facilitate, restrict, and / or monitor the transfer of Molecules in and out of Vesicles. <br />
 * In programming terms, Pores define APIs, where the program is the protected Vesicle contents and the application is the environment or another Vesicle interacting with the contents through a Pore: micro-APIs, if you will. <br />
 * It is up to you to define how you'd like to wrap the methods provided by TransMembraneDomain. <br />
 * See OpenChannel for a simple implementation. <br />
 * NOTE: we use "channel" to designate a Pore implementation and "pore" to designate an abstract, "disarmed" class. <br />
 * Channels "run on" Pores the way sockets "run on" ports; though here Pores scale precisely with Channels. <br />
 */
class Pore :
	public Surface,
	public Class< Pore >
{
public:

	/**
	 * Ensure virtual methods point to Class implementations. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(molecular,
		Pore)

	/**
	 * @param name
	 */
	Pore(
		const Name& name,
		Vesicle* environment = NULL
	);

	virtual ~Pore();

	/**
	 * Sets mEnvironment & mTransMembaneDomain.mInner. <br />
	 * Don't let the environment go out of scope or be deleted before *this! <br />
	 * @param environment
	 */
	virtual void SetEnvironment(Vesicle* environment);

protected:
	TransMembraneDomain mTransMembraneDomain;
};
} //molecular namespace
} //bio namespace
