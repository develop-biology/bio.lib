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
#include "Substance.h"
#include "Products.h"
#include "structure/LinearStructuralComponent.h"
#include "bio/physical/Indentified.h"
#include "bio/physical/Class.h"

#inclued "bio/common/TypeName.h"

namespace bio {
namespace chemical {

class Reactant;

/**
 * A chemical::Reaction takes in Reactants and produces Products, performing some work in between.
 * This is basically a fancy functor that takes advantage of Atom::Bonds and Substance Properties and States to do argument validation.
 *
 * To make a Reaction, you must overload Process (virtual chemical::Products Process(chemical::Substances& reactants) = 0;)
 * Then, preferably in your ctor, state the Require()ments.
 * Each Require()d Reactant* will be checked against the reactants (Substances&) provided to *this.
 * ORDER MATTERS! The reactants must follow the same order as the Required Reactants!
 *
 * To invoke a Reaction, use the operator(), providing reactants.
 * For example: MyReaction r; r(MyReactants);
 * Doing so will do all necessary input checking and then call Process(), if all is good. Otherwise no Products are returned and you will get a code::code::FailedReaction().
 *
 * Other ways to invoke a reaction include:
 * Reaction::Attempt<MyReaction>(MyReactants)
 * ForEach<MyReactant>(Reaction::Initiate<MyReaction>()) (see LinearStructuralComponent.h)
 *
 */
class Reaction :
	virtual public physical::Identifiable< StandardDimension >,
	public physical::Class< Reaction >,
	public LinearStructuralComponent< Reactant* >
{
public:

	/**
	 * Ensure virtual methods point to Class implementations.
	 */
	BIO_DISAMBIGUATE_CLASS_METHODS(physical,
		Reaction)

	/**
	 *
	 */
	Reaction();

	/**
	 * @param id
	 */
	explicit Reaction(Id id);

	/**
	 * @param name
	 */
	explicit Reaction(Name name);

	/**
	 * @param name
	 * @param reactants
	 */
	explicit Reaction(
		Name name,
		typename StructuralComponentImplementation< Reactant* >::Contents reactants
	);

	/**
	 * Add a required Reactant to *this.
	 * NOTE: ORDER MATTERS!
	 * The order of requirements MUST be the order of arguments provided to operator().
	 * @param reactant
	 */
	virtual void Require(Reactant* reactant);

	/**
	 * Wrapper around Require(Reactant*).
	 * Constructs Reactant from args
	 * @param typeName
	 * @param substance
	 */
	void Require(
		Name typeName,
		const Substance& substance
	);

	/**
	 * Wrapper around Require(Reactant*).
	 * Constructs Reactant from args
	 * @param typeName
	 * @param properties
	 * @param states
	 */
	void Require(
		Name typeName,
		typename const StructuralComponent< Property >::Contents& properties,
		typename const StructuralComponent< State >::Contents& states
	);

	/**
	 * Wrapper around Require(Reactant*).
	 * Constructs Reactant from args, adding the Enabled() State as a requirement.
	 * NOTE: T should not have pointer type (unless you're using **).
	 * @tparam T the T* which will be used in the Reaction (without the *)
	 */
	template < typename T >
	void Require()
	{
		Properties empty;
		States enabled;
		enabled.push_back(Enabled());
		Require(
			TypeName< T >(),
			empty,
			enabled
		);
	}

	/**
	 * Wrapper around Require(Reactant*).
	 * Constructs Reactant from args
	 * @tparam T the T* which will be used in the Reaction (without the *)
	 * @param substance
	 */
	template < typename T >
	void Require(const T& substance)
	{
		Require(
			TypeName< T >(),
			substance
		);
	}

	/**
	 * Wrapper around Require(Reactant*).
	 * Constructs Reactant from args
	 * @tparam T the T* which will be used in the Reaction (without the *)
	 */
	template < typename T > void Require(typename const StructuralComponent< Property >::Contents properties&,

	typename const StructuralComponent< State >::Contents states
	&)
	{
		Require(
			TypeName< T >(),
			properties,
			states
		);
	}

	/**
	 * User defined reaction process.
	 * The actual work of *this.
	 * @param reactants
	 * @return Products containing a Code and some set of new Substances or just the reactants. Up to you!
	 */
	virtual Products Process(Substances& reactants) = 0;

	/**
	 * Checks if the given Substances match the Reactants in *this.
	 * ORDER MATTERS!
	 * NOTE: toCheck may have MORE substances than just the reactants needed for this->Process but must have AT LEAST the required reactants.
	 * @param toCheck
	 * @return true if all Substances match; false otherwise.
	 */
	virtual bool SubstancesCanReact(const Substances& toCheck) const;

	/**
	 * A Reaction takes in some Substances and checks if they match the Reactants for *this.
	 * If the inputs check out, the Reaction occurs and the products are returned.
	 * @param reactants
	 * @return Products of this->Process(...) or empty Products with a code::FailedReaction() Code.
	 */
	virtual Products operator()(Substances& reactants) const;

	/**
	 * Get a Reaction!
	 * This should be used to avoid unnecessary new and deletes.
	 * Use when the TypeName does not match the Name or when you have the id of the Reaction you want.
	 * @param id
	 * @return a Reaction* with the given id or NULL.
	 */
	static Reaction* Initiate(StandardDimension id);

	/**
	 * Get a Reaction!
	 * This should be used to avoid unnecessary new and deletes.
	 * This only works for Reactions that have a name matching their type (i.e. were constructed with name=PeriodicTable::Instance().GetNameFromType(*this)), which is true for all Reactions in the core Biology framework.
	 * @tparam T
	 * @return
	 */
	template < typename T >
	static T* Initiate()
	{
		T* ret = ReactionPerspective::Instance().GetTypeFromNameAs< T >(TypeName(T));
		BIO_SANITIZE_AT_SAFETY_LEVEL_2(ret,
			return ret,
			return new T());
		//TODO: address memory leaks when T->GetName() != TypeName<T>().
	}

	/**
	 * Invokes a reaction of the given id using the provided reactants.
	 * @tparam T a reaction type
	 * @param reactants the reactants to provide to T.
	 * @return operator() of the given reaction; else reactants.
	 */
	template < typename T >
	static Products Attempt(Substances& reactants)
	{
		BIO_SANITIZE_WITH_CACHE(Iniate< T >(),
			return (*Cast< T* >(RESULT))(reactants),
			return reactants);
	}

	/**
	 * Ease of use helper for invoking Reactions without creating a vector
	 * (Mostly useful for C++98)
	 * @tparam T
	 * @param reactant1
	 * @param reactant2
	 * @param reactant3
	 * @return Attempt<T> with the given reactants.
	 */
	template < typename T >
	static Products Attempt(
		Substance* reactant1,
		Substance* reactant2 = NULL,
		Substance* reactant3 = NULL
	)
	{
		Substances reactants;
		reactants.push_back(reactant1);
		reactants.push_back(reactant2);
		reactants.push_back(reactant3);
		return Attempt< T >(reactants);
	}
};

} //chemical namespace
} //bio namespace
