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
#include "Reactants.h"
#include "Products.h"
#include "bio/chemical/common/Class.h"
#include "bio/chemical/common/Filters.h"
#include "bio/chemical/common/States.h"
#include "bio/chemical/structure/motif/LinearMotif.h"
#include "bio/physical/Identifiable.h"
#include "bio/common/type/TypeName.h"

namespace bio {
namespace chemical {

/**
 * A chemical::Reaction takes in Reactants and produces Products, performing some work in between. <br />
 * For more info, see Reactants.h, Reactant.h (singular), Products.h, and Substance.h <br />
 * This is basically a fancy functor that takes advantage of Atom::Bonds and Substance Properties and States to do argument validation. <br />
 *
 * Reactions, like those in real life, will often deal with the changing of chemical Bonds and Properties. <br />
 * Reactions are for turning iron into gold. All lesser magicks can be performed with Excitation! <br />
 * In a more real sense, Excitations should not make or break Bonds (except in rare circumstances) whereas Reactions, when fully Processed, should change the Reactants in some significant way, hence their return as Products. <br />
 * Another difference between Reactants and Excitations is that Excitations act ON a Wave (i.e. wave->someMethod()) while Reactions act WITH Substances. <br />
 * Additionally, Excitations store their arguments as members, requiring each kind of Excitation function call to be a new object (e.g. to call firstObject->method(firstArg) and secondObject->method(firstArg) is 1 Excitation while firstObject->method(secondArg) would require a new Excitation). <br />
 * Reactions, on the other hand, are more traditional functors and do not (by default) maintain any state between calls, meaning the same Reaction object can be used for all invokations. <br />
 * Both Excitation and Reaction are functors but the behavior and minimum requirements for using each are different. <br />
 *
 * A real life corollary: <br />
 * Imagine a sound wave hitting a wall. This could be modeled as an Excitation: the energy from the air molecules excite those in the wall, passing energy between them. We might write this as `soundEnergyTransferExcitation = EnergyTransferExcitation(sound); soundEnergyTransferExcitation(wall)`, which could produce code like `wall->ExchangeEnergy(sound)` and could be used on anything the sound wave hit (e.g. `soundEnergyTransferExcitation(floor)`). <br />
 * A similar, but inappropriate Reaction could be `EnergyTransferReaction(wall, sound)` which might produce code like `wall->ExchangeEnergy(sound); return {wall, sound}`, saying that a sound wave hitting a wall creates a new wall and sound wave. The same Reaction would have to be called again for each object the sound wave impacted. <br />
 * A more appropriate Reaction would be something like `Burn(fuel, air)`, which might return `{flame, ash, smoke, air}`, where you could then  check the flame->GetColor(), smoke->GetSmell(), reactantAir->GetQuantity("Oxygen") - productAir->GetQuantity("Oxygen"), and so on. <br />
 * The inappropriate Excitation analog, `airBurnExcitation = BurnExcitation(air); airBurnExcitation(fuel)`, would call `fuel->burn(air)`, which could work but would require that anything capable of being burned implement the burn method. <br />
 *
 * While the semantic difference between the implementations of Excitation and Reaction is subtle and possibly pedantic, the use case of calling a class method vs calling a function with a class should be clear cut. <br />
 * In reality, you'll probably want to use Excitations in your Reactions and vice-versa. For instance, hitting a wall with a nuclear blast would be far more complex than modeling a sound wave. <br />
 * See Excitation.h for more info. <br />
 *
 * To make a Reaction, you must overload Process (virtual chemical::Products Process(chemical::Substances& reactants) = 0;) <br />
 * Then, preferably in your constructor, state the Require()ments. <br />
 * Each Require()d Reactant* will be checked against the reactants (Substances&) provided to *this. 
 * ORDER MATTERS! The reactants must follow the same order as the Required Reactants! <br />
 *
 * To invoke a Reaction, use the operator(), providing reactants. <br />
 * For example: MyReaction r; r(MyReactants); <br />
 * Doing so will do all necessary input checking and then call Process(), if all is good. Otherwise no Products are returned and you will get a code::FailedReaction(). <br />
 *
 * Other ways to invoke a reaction include: <br />
 * Reaction::Attempt<MyReaction>(myReactants); <br />
 * myReaction = Reaction::Initiate<MyReaction>(); myReaction(myReactants); <br />
 *
 */
class Reaction :
	public chemical::Class< Reaction >,
	virtual public Structure //for use in LinearMotifs downstream, we must have interface methods available, even though Reaction does not directly contain anything.
{
public:

	/**
	 * Ensure virtual methods point to Class implementations. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(chemical, Reaction)

	/**
	 * Standard ctors. <br />
	 */ BIO_DEFAULT_IDENTIFIABLE_CONSTRUCTORS(chemical,
		Reaction,
		&ReactionPerspective::Instance(),
		filter::Chemical(),
		symmetry_type::Operation())

	/**
	 * @param name
	 * @param reactants
	 */
	explicit Reaction(
		const Name& name,
		const Reactants* reactants
	);

	/**
	 * Add a required Reactant to *this. <br />
	 * NOTE: ORDER MATTERS! <br />
	 * The order of requirements MUST be the order of arguments provided to operator(). <br />
	 * @param reactant
	 */
	virtual void Require(Reactant* reactant);

	/**
	 * Wrapper around Require(Reactant*). <br />
	 * Constructs Reactant from args <br />
	 * @param typeName
	 * @param substance
	 */
	void Require(
		const Name& typeName,
		const Substance* substance
	);

	/**
	 * Wrapper around Require(Reactant*). <br />
	 * Constructs Reactant from args <br />
	 * @param typeName
	 * @param properties
	 * @param states
	 */
	void Require(
		const Name& typeName,
		const typename UnorderedMotif< Property >::Contents* properties,
		const typename UnorderedMotif< State >::Contents* states
	);

	/**
	 * Wrapper around Require(Reactant*). <br />
	 * Constructs Reactant from args, adding the Enabled() State as a requirement. <br />
	 * NOTE: T should not have pointer type (unless you're using **). <br />
	 * @tparam T the T* which will be used in the Reaction (without the *)
	 */
	template < typename T >
	void Require()
	{
		Properties empty;
		States enabled;
		enabled.Add(state::Enabled());
		Require(
			type::TypeName< T >(),
			empty,
			enabled
		);
	}

	/**
	 * Wrapper around Require(Reactant*). <br />
	 * Constructs Reactant from args <br />
	 * @tparam T of the T* which will be used in the Reaction (without the *)
	 * @param substance the T* to use (with the *).
	 */
	template < typename T >
	void Require(const T* substance)
	{
		Require(
			type::TypeName< T >(),
			substance
		);
	}

	/**
	 * Wrapper around Require(Reactant*). <br />
	 * Constructs Reactant from args <br />
	 * @tparam T the T* which will be used in the Reaction (without the *)
	 */
	template < typename T >
	void Require(
		const typename UnorderedMotif< Property >::Contents* properties,
		const typename UnorderedMotif< State >::Contents* states
	)
	{
		Require(
			type::TypeName< T >(),
			properties,
			states
		);
	}

	/**
	 * User defined reaction process. <br />
	 * The actual work of *this. <br />
	 * @param reactants
	 * @return Products containing a Code and some set of new Substances or just the reactants. Up to you!
	 */
	virtual Products Process(Reactants* reactants) const
	{

		//        YOUR CODE GOES HERE

		return Products(code::NotImplemented());
	}

	/**
	 * Checks if the given Substances match the Reactants in *this. <br />
	 * ORDER MATTERS! <br />
	 * NOTE: toCheck may have MORE substances than just the reactants needed for this->Process but must have AT LEAST the required Reactants. <br />
	 * @param toCheck
	 * @return true if all Substances match; false otherwise.
	 */
	virtual bool ReactantsMeetRequirements(const Reactants* toCheck) const;

	/**
	 * A Reaction takes in some Reactants and checks if they match the Reactants for *this. <br />
	 * If the inputs check out, the Reaction occurs and the products are returned. <br />
	 * @param reactants
	 * @return Products of this->Process(...) or empty Products with a code::FailedReaction() Code.
	 */
	virtual Products operator()(Reactants* reactants) const;

	/**
	 * Get a Reaction! <br />
	 * This should be used to avoid unnecessary new and deletes. <br />
	 * Use when the TypeName does not match the Name or when you have the id of the Reaction you want. <br />
	 * @param id
	 * @return a Reaction* with the given id or NULL.
	 */
	static const Reaction* Initiate(const Id& id);

	/**
	 * Get a Reaction! <br />
	 * This should be used to avoid unnecessary new and deletes. <br />
	 * This only works for Reactions that have a name matching their type (i.e. were constructed with name=SafelyAccess<PeriodicTable>()->GetNameFromType(*this)), which is true for all Reactions in the core Biology framework. <br />
	 * @tparam T
	 * @return a Reaction* of the given type or NULL, if no Reaction exists matching the TypeName of the given T.
	 */
	template < typename T >
	static const T* Initiate()
	{
		const T* ret = SafelyAccess<ReactionPerspective>()->template GetTypeFromNameAs< T >(type::TypeName< T >());
		BIO_SANITIZE_AT_SAFETY_LEVEL_1(ret,
			return ret,
			return NULL);
	}

	/**
	 * Invokes a reaction of the given id using the provided reactants. <br />
	 * @tparam T a reaction type
	 * @param reactants the reactants to provide to T.
	 * @return operator() of the given reaction; else reactants.
	 */
	template < typename T >
	static Products Attempt(Reactants* reactants)
	{
		BIO_SANITIZE_WITH_CACHE(Initiate< T >(),
			return (*Cast< const T* >(RESULT))(reactants),
			return Products(
				code::NotImplemented(),
				reactants
			));
	}

	template < typename T >
	static Products Attempt(Substances& substances)
	{
		Reactants reactants(substances);
		BIO_SANITIZE_WITH_CACHE(Initiate< T >(),
			return (*Cast< const T* >(RESULT))(&reactants),
			return Products(
				code::NotImplemented(),
				reactants
			));
	}

	/**
	 * Ease of use helper for invoking Reactions without creating a vector <br />
	 * (Mostly useful for C++98) <br />
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
		Substances substances;
		substances.Add(reactant1);
		substances.Add(reactant2);
		substances.Add(reactant3);
		Reactants reactants(substances);
		return Attempt< T >(&reactants);
	}

protected:
	Reactants mRequiredReactants;
};

} //chemical namespace
} //bio namespace
