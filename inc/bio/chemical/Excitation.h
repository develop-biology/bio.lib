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
#include "bio/physical/Macros.h"
#include "Properties.h"

namespace bio {
namespace chemical {

/**
 * Base class for all Excitations; see below.
 *
 * Main documentation for all Excitation classes will be provided here.
 *
 * For the difference between Excitation and Reaction, see Reaction.h
 * The main thing Excitations can do which Reactions can't is be used in LinearStructureInterface::ForEach<>().
 *
 * An Excitation is a Wave that stores a function pointer, i.e. a functor.
 * Excitations allow you to directly invoke a Wave's methods.
 * Excitations can be useful in propagating operations through Wave networks (e.g. an Atom's Bonds). Doing so will likely involve Modulating an Excitation onto a carrier Wave that dictates what the function applies to.
 */
class ExcitationBase :
	public physical::Class< ExcitationBase >
{
public:
	/**
	 * Ensure virtual methods point to Class implementations.
	 */
	BIO_DISAMBIGUATE_CLASS_METHODS(physical,
		ExcitationBase)

	/**
	 *
	 */
	ExcitationBase() :
		physical::Class< ExcitationBase >(this)
	{

	}

	virtual ~ExcitationBase()
	{

	}

	/**
	 * All Excitations share the "Excitatory" Property.
	 * @return {property::Excitatory()}
	 */
	static Properties GetClassProperties()
	{
		Properties ret;
		ret.push_back(property::Excitatory());
		return ret;
	}

	/**
	 * Override of Wave method. See that class for details.
	 * NOTE: this should be overloaded to ensure *this will Resonate with WAVEs by stealing their Properties from the PeriodicTable.
	 * @return {property::Excitatory()}
	 */
	virtual Properties GetProperties() const
	{
		return GetClassProperties();
	}

	/**
	 * Invoke an Excitation, regardless of what the template parameters are.
	 * Since we have no idea what the return value will be, we simply place it in ret as a void*.
	 * @param wave
	 * @param ret
	 */
	virtual void CallDown(physical::Wave* wave, void* ret)
	{
		//nop
	}
};

#if BIO_CPP_VERSION >= 17

	#include <tuple>
	#include <functional>

/**
 * See ExcitationBase for docs.
 * @tparam WAVE
 * @tparam RETURN
 * @tparam ARGUMENTS
 */
template< class WAVE, typename RETURN, typename... ARGUMENTS >
class Excitation :
	public ExcitationBase,
	public physical::Class< Excitation< WAVE,RETURN,ARGUMENTS...> >
{
public:

	/**
	 * Ensure virtual methods point to Class implementations.
	 */
	BIO_DISAMBIGUATE_CLASS_METHODS(physical, BIO_SINGLE_ARG(Excitation<WAVE,RETURN,ARGUMENTS...>))

	/**
	 *
	 */
	Excitation(RETURN(WAVE::*function)(ARGUMENTS...), const ARGUMENTS&... args)
		:
		physical::Class< Excitation<WAVE,RETURN,ARGUMENTS...> >(this),
		m_function(function),
		m_args(args)
	{
	}

	/**
	 *
	 */
	virtual Excitation()
	{

	}

	/**
	 * Override of Wave method. See that class for details.
	 * Ensures *this will Resonate with WAVEs by stealing their Properties from the PeriodicTable.
	 * @return {whatever Properties WAVE has, property::Excitatory()}
	 */
	virtual Properties GetProperties() const
	{
		Properties ret = PeriodicTable::Instance().GetPropertiesOf< WAVE >();
		ret.insert(ret.end(), ExcitationBase::GetClassProperties().begin(), ExcitationBase::GetClassProperties().end());
		return ret;
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

	/**
	 * Override of ExcitationBase; see above.
	 */
	virtual void CallDown(physical::Wave* wave, void* ret)
	{
		ret = &((*this)(Cast<WAVE*>(wave)));
	}

protected:
	RETURN (WAVE::*m_function)(ARGUMENTS...)
	std::tuple m_args;
};

#else

/**
 * See ExcitationBase for docs.
 * @tparam WAVE
 * @tparam RETURN
 */
template < class WAVE, typename RETURN >
class ExcitationWithoutArgument :
	public ExcitationBase,
	public physical::Class< ExcitationWithoutArgument< WAVE, RETURN > >
{
public:

	/**
	 * Ensure virtual methods point to Class implementations.
	 */
	BIO_DISAMBIGUATE_CLASS_METHODS(physical,
		BIO_SINGLE_ARG(ExcitationWithoutArgument< WAVE, RETURN >))

	/**
	 *
	 */
	ExcitationWithoutArgument(RETURN(WAVE::*function)())
		:
		physical::Class< ExcitationWithoutArgument< WAVE, RETURN > >(this),
		m_function(function)
	{
	}

	/**
	 *
	 */
	virtual ~ExcitationWithoutArgument()
	{

	}

	/**
	 * Override of Wave method. See that class for details.
	 * Ensures *this will Resonate with WAVEs by stealing their Properties from the PeriodicTable.
	 * @return {whatever Properties WAVE has, property::Excitatory()}
	 */
	virtual Properties GetProperties() const
	{
		Properties ret = PeriodicTable::Instance().GetPropertiesOf< WAVE >();
		ret.insert(ret.end(), ExcitationBase::GetClassProperties().begin(), ExcitationBase::GetClassProperties().end());
		return ret;
	}


	/**
	 * @param wave the caller of m_function.
	 * @return RETURN, whatever that is for *this; the result of calling m_function from wave.
	 */
	RETURN operator()(WAVE* wave)
	{
		return wave->m_function();
	}

	/**
	 * Override of ExcitationBase; see above.
	 */
	virtual void CallDown(physical::Wave* wave, void* ret)
	{
		ret = &((*this)(Cast<WAVE*>(wave)));
	}

protected:
	RETURN (WAVE::*m_function)();
};

/**
 * See ExcitationBase for docs
 * @tparam WAVE
 * @tparam RETURN
 * @tparam ARGUMENT
 */
template < class WAVE, typename RETURN, typename ARGUMENT >
class ExcitationWithArgument :
	public ExcitationBase,
	public physical::Class< ExcitationWithArgument< WAVE, RETURN, ARGUMENT > >
{
public:

	/**
	 * Ensure virtual methods point to Class implementations.
	 */
	BIO_DISAMBIGUATE_CLASS_METHODS(physical,
		BIO_SINGLE_ARG(ExcitationWithArgument< WAVE, RETURN, ARGUMENT >))


	/**
	 *
	 */
	ExcitationWithArgument(
		RETURN(WAVE::*function)(ARGUMENT),
		ARGUMENT arg
	)
		:
		physical::Class< ExcitationWithArgument< WAVE, RETURN, ARGUMENT > >(this),
		m_function(function),
		m_arg(arg)
	{
	}

	/**
	 *
	 */
	virtual ~ExcitationWithArgument()
	{

	}

	/**
	 * Override of Wave method. See that class for details.
	 * Ensures *this will Resonate with WAVEs by stealing their Properties from the PeriodicTable.
	 * @return {whatever Properties WAVE has, property::Excitatory()}
	 */
	virtual Properties GetProperties() const
	{
		Properties ret = PeriodicTable::Instance().GetPropertiesOf< WAVE >();
		ret.insert(ret.end(), ExcitationBase::GetClassProperties().begin(), ExcitationBase::GetClassProperties().end());
		return ret;
	}

	/**
	 * @param wave the caller of m_function.
	 * @return RETURN, whatever that is for *this; the result of calling m_function from wave.
	 */
	RETURN operator()(WAVE* wave)
	{
		return wave->m_function(m_arg);
	}

	/**
	 * Override of ExcitationBase; see above.
	 */
	virtual void CallDown(physical::Wave* wave, void* ret)
	{
		ret = &((*this)(Cast<WAVE*>(wave)));
	}

protected:
	RETURN (WAVE::*m_function)(ARGUMENT);

	ARGUMENT m_arg;
};

#endif
} //chemical namespace
} //bio namespace
