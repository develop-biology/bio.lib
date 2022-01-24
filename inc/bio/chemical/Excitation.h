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

#include "bio/physical/Class.h"
#include "Properties.h"

namespace bio {
namespace chemical {

/**
 * Base class for all Excitations; see below.
 * @tparam WAVE
 */
template <class EXCITATION, class WAVE>
class ExcitationBase :
	public physical::Class<EXCITATION>
{
	/**
	 *
	 */
public

	ExcitationBase()
	{

	}

	/**
	 *
	 */
	virtual
public

	ExcitationBase()
	{

	}

	/**
	 * Override of Wave method. See that class for details.
	 * Ensures *this will Resonate with WAVEs by stealing their Properties from the PeriodicTable.
	 * @return {property::Excitatory()}
	 */
	virtual Properties GetProperties() const
	{
		Properties ret = PeriodicTable::Instance().GetPropertiesOf<WAVE>();
		ret.push_back(property::Excitatory());
		return ret;
	}
};

#if BIO_CPP_VERSION >= 17

#include <tuple>
#include <functional>

/**
 * An Excitation is a Wave that stores a function pointer, i.e. a functor.
 * Excitations allow you to directly invoke a Wave's methods.
 * WaveFunctions can be useful in propagating operations through Wave networks (e.g. an Atom's Bonds). Doing so will likely involve Modulating an Excitation onto a carrier Wave that dictates what the function applies to, though the implementation of such a design is not dictated here, at the physical level.
 * @tparam WAVE
 * @tparam RETURN
 * @tparam ARGUMENTS
 */
template<class WAVE, typename RETURN, typename... ARGUMENTS>
class Excitation :
	public ExcitationBase<Excitation<WAVE,RETURN,ARGUMENTS...>, WAVE>
{
public:

	/**
	 *
	 */
	Excitation(RETURN(WAVE::*function)(ARGUMENTS...), const ARGUMENTS&... args)
		:
		m_function(function),
		m_args(args)
	{
	}

	/**
	 *
	 */
	virtual ~Excitation()
	{

	}

	/**
	 * @param wave the caller of m_function.
	 * @param args any arguments given to m_function. Only applicable for C++11 and onward.
	 * @return RETURN, whatever that is for *this; the result of calling m_function from wave.
	 */
	RETURN operator()(WAVE* wave)
	{
		std::tuple allArgs = std::tuple_cat(std::make_tuple(wave), m_args);
		return std::apply(m_function, allArgs);
	}

protected:
	RETURN (WAVE::*m_function)(ARGUMENTS...)
	std::tuple m_args;
};

#else

/**
 * An Excitation is a Wave that stores a function pointer, i.e. a functor.
 * Excitations allow you to directly invoke a Wave's methods.
 * WaveFunctions can be useful in propagating operations through Wave networks (e.g. an Atom's Bonds). Doing so will likely involve Modulating an Excitation onto a carrier Wave that dictates what the function applies to, though the implementation of such a design is not dictated here, at the physical level.
 * @tparam WAVE
 * @tparam RETURN
 */
template <class WAVE, typename RETURN>
class Excitation :
	public ExcitationBase<Excitation<WAVE, RETURN>, WAVE>
{
public:

	/**
	 *
	 */
	Excitation(RETURN(WAVE::*function)())
		:
		m_function(function)
	{
	}

	/**
	 *
	 */
	virtual ~Excitation()
	{

	}

	/**
	 * @param wave the caller of m_function.
	 * @return RETURN, whatever that is for *this; the result of calling m_function from wave.
	 */
	RETURN operator()(WAVE* wave)
	{
		return wave->m_function();
	}

protected:
	RETURN (WAVE::*m_function)()
};

/**
 * An Excitation is a Wave that stores a function pointer, i.e. a functor.
 * Excitations allow you to directly invoke a Wave's methods.
 * WaveFunctions can be useful in propagating operations through Wave networks (e.g. an Atom's Bonds). Doing so will likely involve Modulating an Excitation onto a carrier Wave that dictates what the function applies to, though the implementation of such a design is not dictated here, at the physical level.
 * @tparam WAVE
 * @tparam RETURN
 * @tparam ARGUMENT
 */
template <class WAVE, typename RETURN, typename ARGUMENT>
class Excitation :
	public ExcitationBase<Excitation<WAVE, RETURN, ARGUMENT>, WAVE>
{
public:

	/**
	 *
	 */
	Excitation(
		RETURN(WAVE::*function)(ARGUMENT),
		ARGUMENT arg)
		:
		m_function(function),
		m_arg(arg)
	{
	}

	/**
	 *
	 */
	virtual ~Excitation()
	{

	}

	/**
	 * @param wave the caller of m_function.
	 * @return RETURN, whatever that is for *this; the result of calling m_function from wave.
	 */
	RETURN operator()(WAVE* wave)
	{
		return wave->m_function(m_arg);
	}

protected:
	RETURN (WAVE::*m_function)(ARGUMENT);

	ARGUMENT m_arg;
};

#endif
} //chemical namespace
} //bio namespace
