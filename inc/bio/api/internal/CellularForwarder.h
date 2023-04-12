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

#include "bio/cellular/Cell.h"

namespace bio {

/**
 * Forward Crest() to some other method in some other class. <br />
 * The only requirements are that the given method return a bio::Code and take no arguments. <br />
 * @tparam CALLER class that implements the desired method.
 * @tparam CLOBBER a cellular::Class<> to forward from.
 */
template < class CALLER, class CLOBBER >
class CellularForwarder:
	public cellular::Class< CellularForwarder< CALLER, CLOBBER > >,
	public CLOBBER
{
public:

	/**
	 * Ensure virtual methods point to Class implementations. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(cellular, CellularForwarder)

	/**
	 *
	 */
	CellularForwarder() :
		cellular::Class< CellularForwarder< CALLER, CLOBBER > >(
			this,
			filter::Cellular()
		),
		mForwardedCrest(NULL),
		mCaller(NULL)
	{
	}

	/**
	 * @param name
	 */
	explicit CellularForwarder(const Name& name) :
		cellular::Class< CellularForwarder< CALLER, CLOBBER > >(
			this,
			name,
			filter::Cellular()
		),
		mForwardedCrest(NULL),
		mCaller(NULL)
	{
	}

	/**
	 * @param id
	 */
	explicit CellularForwarder(const Id& id) :
		cellular::Class< CellularForwarder< CALLER, CLOBBER > >(
			this,
			id,
			filter::Cellular()
		),
		mForwardedCrest(NULL),
		mCaller(NULL)
	{
	}

	/**
	 *
	 */
	virtual ~CellularForwarder()
	{
		BIO_ASSERT(mForwardedCrest);
		delete mForwardedCrest;
	}

	/**
	 * Set what Crest() should become. <br />
	 * @param forwardTo
	 */
	void SetCrestFunction(
		bio::Code (CALLER::*forwardTo)(),
		CALLER* caller)
	{
		mForwardedCrest = new BIO_EXCITATION_CLASS(CALLER, Code)(forwardTo);
		mCaller = caller;
	}

	/**
	 * Make Crest call something else. <br />
	 */
	virtual Code Crest() BIO_FINAL
	{
		Code ret = (*mForwardedCrest)(mCaller);

		//Then also do this.
		cellular::Class< CellularForwarder< CALLER, CLOBBER > >::Crest();

		return ret;
	}

private:
	BIO_EXCITATION_CLASS(CALLER, Code)* mForwardedCrest;
	CALLER* mCaller;
};

} //bio namespace
