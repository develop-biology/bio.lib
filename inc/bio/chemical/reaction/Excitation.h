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

#include "bio/physical/common/Class.h"
#include "bio/physical/macro/Macros.h"
#include "bio/chemical/common/Properties.h"
#include "bio/chemical/PeriodicTable.h"

#if BIO_CPP_VERSION >= 17

	#include <tuple>
	#include <functional>

#endif

namespace bio {
namespace chemical {

/**
 * Base class for all Excitations; see below. <br />
 *
 * Main documentation for all Excitation classes will be provided here. <br />
 *
 * For the difference between Excitation and Reaction, see Reaction.h <br />
 * The main thing Excitations can do, which Reactions can't, is be used in LinearStructureInterface::ForEach<>(). <br />
 *
 * An Excitation is a Wave that stores a function pointer, i.e. a functor. <br />
 * Excitations allow you to directly invoke a Wave's methods. <br />
 * Excitations can be useful in propagating operations through Wave networks (e.g. an Atom's Bonds). Doing so will likely involve Modulating an Excitation onto a carrier Wave that dictates what the function applies to. <br />
 */
class ExcitationBase :
	public physical::Class< ExcitationBase >
{
public:
	/**
	 * Ensure virtual methods point to Class implementations. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(physical, ExcitationBase)

	/**
	 *
	 */
	ExcitationBase()
		:
		physical::Class< ExcitationBase >(this)
	{

	}

	virtual ~ExcitationBase()
	{

	}

	/**
	 * All Excitations share the "Excitatory" Property. <br />
	 * @return {property::Excitatory()}
	 */
	static Properties GetClassProperties()
	{
		Properties ret;
		ret.Add(property::Excitatory());
		return ret;
	}

	/**
	 * Override of Wave method. See that class for details. <br />
	 * NOTE: this should be overloaded to ensure *this will Resonate with WAVEs by stealing their Properties from the PeriodicTable. <br />
	 * @return {property::Excitatory()}
	 */
	virtual Properties GetProperties() const
	{
		return GetClassProperties();
	}

	/**
	 * Creating a new and proper Excitation is preferred to Editing Arguments; however, we support the latter nonetheless. <br />
	 * @param position
	 * @param newVal
	 */
	virtual void EditArg(
		uint8_t position,
		ByteStream& newVal
	)
	{
		//nop
	}

	/**
	 * Invoke an Excitation, regardless of what the template parameters are. <br />
	 * Since we have no idea what the return value will be, we simply place it in ret as a void*. <br />
	 * @param wave
	 * @param ret
	 */
	virtual void CallDown(
		physical::Wave* wave,
		ByteStream* ret
	) const
	{
		//nop
	}
};

#if BIO_CPP_VERSION >= 17

/**
 * See ExcitationBase for docs. <br />
 * @tparam WAVE
 * @tparam RETURN
 * @tparam ARGUMENTS
 */
template < class WAVE, typename RETURN, typename... ARGUMENTS >
class Excitation :
	public physical::Class< Excitation< WAVE, RETURN, ARGUMENTS... > >,
	public ExcitationBase
{
public:

	/**
	 * Ensure virtual methods point to Class implementations. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(physical, 		BIO_SINGLE_ARG(Excitation< WAVE, RETURN, ARGUMENTS... >))

	/**
	 *
	 */
	Excitation(
		RETURN(WAVE::*function)(ARGUMENTS...),
		const ARGUMENTS& ... args
	)
		:
		physical::Class< Excitation< WAVE, RETURN, ARGUMENTS... > >(this),
		mFunction(function),
		mArgs(args...)
	{
	}

	/**
	 *
	 */
	virtual ~Excitation()
	{

	}

	/**
	 * Override of Wave method. See that class for details. <br />
	 * Ensures *this will Resonate with WAVEs by stealing their Properties from the PeriodicTable. <br />
	 * @return {whatever Properties WAVE has, property::Excitatory()}
	 */
	virtual Properties GetProperties() const
	{
		Properties ret = SafelyAccess<PeriodicTable>()->GetPropertiesOf< WAVE >();
		ret.Import(ExcitationBase::GetClassProperties());
		return ret;
	}

	/**
	 * Creating a new and proper Excitation is preferred to Editing Arguments; however, we support the latter nonetheless. <br />
	 * @param position
	 * @param newVal
	 */
	virtual void EditArg(
		uint8_t position,
		ByteStream& newVal
	)
	{
		//TODO...
	}

	/**
	 * @param wave the caller of mFunction.
	 * @param args any arguments given to mFunction. Only applicable for C++11 and onward.
	 * @return RETURN, whatever that is for *this; the result of calling mFunction from wave.
	 */
	RETURN operator()(WAVE* wave) const
	{
		std::tuple< WAVE*, ARGUMENTS... > allArgs = ::std::tuple_cat(
			std::make_tuple(wave),
			mArgs
		);
		return ::std::apply(
			mFunction,
			allArgs
		);
	}

	/**
	 * Override of ExcitationBase; see above. <br />
	 */
	virtual void CallDown(
		physical::Wave* wave,
		ByteStream* ret
	) const
	{
		ret->Set(this->operator()(ForceCast< WAVE* >(wave)));
	}

protected:
	RETURN (WAVE::*mFunction)(ARGUMENTS...);

	std::tuple< ARGUMENTS... > mArgs;
};

#else

/**
 * See ExcitationBase for docs. <br />
 * @tparam WAVE
 * @tparam RETURN
 */
template < class WAVE, typename RETURN >
class ExcitationWithoutArgument :
	public physical::Class< ExcitationWithoutArgument< WAVE, RETURN > >,
	public ExcitationBase
{
public:

	/**
	 * Ensure virtual methods point to Class implementations. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(physical, 		BIO_SINGLE_ARG(ExcitationWithoutArgument< WAVE, RETURN >))

	/**
	 *
	 */
	ExcitationWithoutArgument(RETURN(WAVE::*function)())
		:
		physical::Class< ExcitationWithoutArgument< WAVE, RETURN > >(this),
		mFunction(function)
	{
	}

	/**
	 *
	 */
	virtual ~ExcitationWithoutArgument()
	{

	}

	/**
	 * Override of Wave method. See that class for details. <br />
	 * Ensures *this will Resonate with WAVEs by stealing their Properties from the PeriodicTable. <br />
	 * @return {whatever Properties WAVE has, property::Excitatory()}
	 */
	virtual Properties GetProperties() const
	{
		Properties ret = SafelyAccess<PeriodicTable>()->GetPropertiesOf< WAVE >();
		ret.Import(ExcitationBase::GetClassProperties());
		return ret;
	}


	/**
	 * @param wave the caller of mFunction.
	 * @return RETURN, whatever that is for *this; the result of calling mFunction from wave.
	 */
	RETURN operator()(WAVE* wave) const 
	{
		return (wave->*mFunction)();
	}

	/**
	 * Override of ExcitationBase; see above. <br />
	 */
	virtual void CallDown(
		physical::Wave* wave, 
		ByteStream* ret 
	) const
	{
		ret->Set(this->operator()(ForceCast< WAVE* >(wave))); 
	}

protected:
	RETURN (WAVE::*mFunction)();
};

/**
 * See ExcitationBase for docs <br />
 * @tparam WAVE
 * @tparam RETURN
 * @tparam ARGUMENT
 */
template < class WAVE, typename RETURN, typename ARGUMENT >
class ExcitationWithArgument :
	public physical::Class< ExcitationWithArgument< WAVE, RETURN, ARGUMENT > >,
	public ExcitationBase
{
public:

	/**
	 * Ensure virtual methods point to Class implementations. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(physical, 		BIO_SINGLE_ARG(ExcitationWithArgument< WAVE, RETURN, ARGUMENT >))


	/**
	 *
	 */
	ExcitationWithArgument(
		RETURN(WAVE::*function)(ARGUMENT),
		ARGUMENT arg
	)
		:
		physical::Class< ExcitationWithArgument< WAVE, RETURN, ARGUMENT > >(this),
		mFunction(function),
		mArg(arg)
	{
	}

	/**
	 *
	 */
	virtual ~ExcitationWithArgument()
	{

	}

	/**
	 * Override of Wave method. See that class for details. <br />
	 * Ensures *this will Resonate with WAVEs by stealing their Properties from the PeriodicTable. <br />
	 * @return {whatever Properties WAVE has, property::Excitatory()}
	 */
	virtual Properties GetProperties() const
	{
		Properties ret = SafelyAccess<PeriodicTable>()->GetPropertiesOf< WAVE >();
		ret.Import(ExcitationBase::GetClassProperties());
		return ret;
	}

	/**
	 * Creating a new and proper Excitation is preferred to Editing Arguments; however, we support the latter nonetheless. <br />
	 * @param position
	 * @param newVal
	 */
	virtual void EditArg(
		uint8_t position,
		ByteStream& newVal
	)
	{
		BIO_SANITIZE(position,
			return,);
		mArg = newVal.As< ARGUMENT >();
	}

	/**
	 * @param wave the caller of mFunction.
	 * @return RETURN, whatever that is for *this; the result of calling mFunction from wave.
	 */
	RETURN operator()(WAVE* wave) const 
	{
		return (wave->*mFunction)(mArg);
	}

	/**
	 * Override of ExcitationBase; see above. <br />
	 */
	virtual void CallDown(
		physical::Wave* wave, 
		ByteStream* ret 
	) const
	{
		ret->Set(this->operator()(ForceCast< WAVE* >(wave))); 
	}

protected:
	RETURN (WAVE::*mFunction)(ARGUMENT);

	ARGUMENT mArg;
};

/**
 * See ExcitationBase for docs <br />
 * @tparam WAVE
 * @tparam RETURN
 * @tparam ARGUMENT1
 * @tparam ARGUMENT2
 */
template < class WAVE, typename RETURN, typename ARGUMENT1, typename ARGUMENT2 >
class ExcitationWithTwoArguments :
	public physical::Class< ExcitationWithTwoArguments< WAVE, RETURN, ARGUMENT1, ARGUMENT2 > >,
	public ExcitationBase
{
public:

	/**
	 * Ensure virtual methods point to Class implementations. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(physical, 		BIO_SINGLE_ARG(ExcitationWithTwoArguments< WAVE, RETURN, ARGUMENT1, ARGUMENT2 >))


	/**
	 *
	 */
	ExcitationWithTwoArguments(
		RETURN(WAVE::*function)(
			ARGUMENT1,
			ARGUMENT2
		),
		ARGUMENT1 arg1,
		ARGUMENT2 arg2
	)
		:
		physical::Class< ExcitationWithTwoArguments< WAVE, RETURN, ARGUMENT1, ARGUMENT2 > >(this),
		mFunction(function),
		mArg1(arg1),
		mArg2(arg2)
	{
	}

	/**
	 *
	 */
	virtual ~ExcitationWithTwoArguments()
	{

	}

	/**
	 * Override of Wave method. See that class for details. <br />
	 * Ensures *this will Resonate with WAVEs by stealing their Properties from the PeriodicTable. <br />
	 * @return {whatever Properties WAVE has, property::Excitatory()}
	 */
	virtual Properties GetProperties() const
	{
		Properties ret = SafelyAccess<PeriodicTable>()->GetPropertiesOf< WAVE >();
		ret.Import(ExcitationBase::GetClassProperties());
		return ret;
	}

	/**
	 * Creating a new and proper Excitation is preferred to Editing Arguments; however, we support the latter nonetheless. <br />
	 * @param position
	 * @param newVal
	 */
	virtual void EditArg(
		uint8_t position,
		ByteStream& newVal
	)
	{
		switch (position)
		{
			case 0:
				mArg1 = newVal.As< ARGUMENT1 >();
				break;
			case 1:
				mArg2 = newVal.As< ARGUMENT2 >();
				break;
		}
	}

	/**
	 * @param wave the caller of mFunction.
	 * @return RETURN, whatever that is for *this; the result of calling mFunction from wave.
	 */
	RETURN operator()(WAVE* wave) const 
	{
		return (wave->*mFunction)(
			mArg1,
			mArg2
		);
	}

	/**
	 * Override of ExcitationBase; see above. <br />
	 */
	virtual void CallDown(
		physical::Wave* wave, 
		ByteStream* ret 
	) const
	{
		ret->Set(this->operator()(ForceCast< WAVE* >(wave))); 
	}

protected:
	RETURN (WAVE::*mFunction)(
		ARGUMENT1,
		ARGUMENT2
	);

	ARGUMENT1 mArg1;
	ARGUMENT2 mArg2;
};


#endif
} //chemical namespace
} //bio namespace
