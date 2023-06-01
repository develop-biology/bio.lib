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

	virtual Code CacheProteins() BIO_FINAL
	{
		return this->::bio::cellular::Class< CellularForwarder >::CacheProteins();
	}

	virtual Code CreateDefaultProteins() BIO_FINAL
	{
		return this->::bio::cellular::Class< CellularForwarder >::CreateDefaultProteins();
	}
	virtual ::bio::physical::Wave* Clone() const
	{
		return this->::bio::cellular::Class< CellularForwarder >::Clone();
	}
	::bio::physical::Wave* AsWave()
	{
		return this->::bio::cellular::Class< CellularForwarder >::AsWave();
	}
	const ::bio::chemical::Atom* AsAtom() const
	{
		return this->::bio::cellular::Class< CellularForwarder >::AsAtom();
	}
	::bio::chemical::Atom* AsAtom()
	{
		return this->::bio::cellular::Class< CellularForwarder >::AsAtom();
	}
	const ::bio::physical::Wave* AsWave() const
	{
		return this->::bio::cellular::Class< CellularForwarder >::AsWave();
	}
	operator ::bio::physical::Wave*()
	{
		return this->::bio::cellular::Class< CellularForwarder >::operator ::bio::physical::Wave*();
	}
	virtual bool RegisterProperties(const ::bio::Properties& properties) BIO_FINAL
	{
		return this->::bio::cellular::Class< CellularForwarder >::RegisterProperties(properties);
	}
	virtual ::bio::Properties GetProperties() const BIO_FINAL
	{
		return this->::bio::cellular::Class< CellularForwarder >::GetProperties();
	}
	template < typename AS >
	operator AS()
	{
		return this->::bio::cellular::Class< CellularForwarder >::template As< AS >();
	}
	virtual const log::Writer* AsLogWriter() const BIO_FINAL
	{
		return this->::bio::cellular::Class< CellularForwarder >::AsLogWriter();
	}
	virtual void Log(
		::bio::LogLevel level,
		const char* format,
		va_list args
	) const BIO_FINAL
	{
		return this->::bio::cellular::Class< CellularForwarder >::Log(
			level,
			format,
			args
		);
	}
	virtual ::bio::Code SetInterval(::bio::Milliseconds interval) BIO_FINAL
	{
		return this->::bio::cellular::Class< CellularForwarder >::SetInterval(interval);
	}
	virtual ::bio::physical::Wave* Modulate(::bio::physical::Wave* signal) BIO_FINAL
	{
		return this->::bio::cellular::Class< CellularForwarder >::Modulate(signal);
	}
	virtual ::bio::physical::Wave* Demodulate() BIO_FINAL
	{
		return this->::bio::cellular::Class< CellularForwarder >::Demodulate();
	}
	virtual const ::bio::physical::Wave* Demodulate() const BIO_FINAL
	{
		return this->::bio::cellular::Class< CellularForwarder >::Demodulate();
	}
	virtual ::bio::physical::Wave* Superpose(
		const ::bio::physical::ConstWaves& displacement,
		::bio::physical::Interference* pattern
	) BIO_FINAL
	{
		return this->::bio::cellular::Class< CellularForwarder >::Superpose(
			displacement,
			pattern
		);
	}
	virtual bool Superpose(
		const ::bio::physical::Wave* displacement,
		::bio::physical::Interference* pattern
	) BIO_FINAL
	{
		return this->::bio::cellular::Class< CellularForwarder >::Superpose(
			displacement,
			pattern
		);
	}
	virtual ::bio::Code Attenuate(const ::bio::physical::Wave* other) BIO_FINAL
	{
		return this->::bio::cellular::Class< CellularForwarder >::Attenuate(other);
	}
	virtual ::bio::Code Disattenuate(const ::bio::physical::Wave* other) BIO_FINAL
	{
		return this->::bio::cellular::Class< CellularForwarder >::Disattenuate(other);
	}
	template < typename ENVIRONMENT >
	void SetEnvironment(ENVIRONMENT environment)
	{
		return this->::bio::cellular::Class< CellularForwarder >::template SetEnvironment< ENVIRONMENT >(environment);
	}
	template < typename ENVIRONMENT >
	ENVIRONMENT GetEnvironment()
	{
		return this->::bio::cellular::Class< CellularForwarder >::template GetEnvironment< ENVIRONMENT >();
	}
	template < typename ENVIRONMENT >
	const ENVIRONMENT GetEnvironment() const
	{
		return this->::bio::cellular::Class< CellularForwarder >::template GetEnvironment< ENVIRONMENT >();
	}
	virtual ::bio::Code Apoptose() BIO_FINAL
	{
		return this->::bio::cellular::Class< CellularForwarder >::Apoptose();
	}
	virtual bool CheckIn() BIO_FINAL
	{
		return this->::bio::cellular::Class< CellularForwarder >::CheckIn();
	}

private:
	BIO_EXCITATION_CLASS(CALLER, Code)* mForwardedCrest;
	CALLER* mCaller;
};

} //bio namespace
