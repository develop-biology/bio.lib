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

#include "bio/chemical/reaction/Reaction.h"
#include "bio/chemical/reaction/Reactant.h"
#include "bio/chemical/common/Codes.h"
#include "bio/chemical/common/Filters.h"

namespace bio {
namespace chemical {

Reaction::Reaction(
	const Name& name,
	const Reactants* reactants
)
	:
	chemical::Class< Reaction >(
		this,
		name,
		filter::Chemical(),
		symmetry_type::Operation()),
	mRequiredReactants(*reactants)
{
}

void Reaction::Require(Reactant* reactant)
{
	mRequiredReactants.Add< Substance* >(reactant);
}

void Reaction::Require(
	const Name& typeName,
	const Substance* substance
)
{
	Require(
		new Reactant(
			typeName,
			substance
		));
}

void Reaction::Require(
	const Name& typeName,
	const UnorderedMotif< Property >& properties,
	const UnorderedMotif< State >& states
)
{
	Require(
		new Reactant(
			typeName,
			properties,
			states
		));
}


bool Reaction::ReactantsMeetRequirements(const Reactants* toCheck) const
{
	return toCheck->HasAll< Substance* >(mRequiredReactants.GetAll< Substance* >());
}

/*static*/ const Reaction* Reaction::Initiate(const AtomicNumber& id)
{
	BIO_SANITIZE_WITH_CACHE(SafelyAccess< PeriodicTable >()->GetTypeFromIdAs< Reaction* >(id),
		return Cast< Reaction* >(RESULT),
		return NULL);
}

Products Reaction::operator()(Reactants* reactants) const
{
	//Always level 2, even if the user wants more (i.e. never less).
	BIO_SANITIZE_AT_SAFETY_LEVEL_1(ReactantsMeetRequirements(reactants),
		return Process(reactants),
		return code::FailedReaction());
}

} //chemical namespace
} //bio namespace
