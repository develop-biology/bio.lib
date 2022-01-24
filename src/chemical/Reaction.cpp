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

#include "bio/chemical/Reaction.h"
#include "bio/chemical/Codes.h"

namespace bio {
namespace chemical {

Reaction::Reaction()
	:
	physical::Class(
		this,
		new physical::Symmetry(
			TypeName<T>(),
			symmetry_Type::Operation()))
{

}


Reaction::Reaction(Id id)
	:
	physical::Class(
		this,
		new physical::Symmetry(
			TypeName<T>(),
			symmetry_Type::Operation())),
	Identifiable(
		name,
		&ReactionPerspective::Instance())
{

}


Reaction::Reaction(Name name)
	:
	physical::Class(
		this,
		new physical::Symmetry(
			TypeName<T>(),
			symmetry_Type::Operation())),
	Identifiable(
		name,
		&ReactionPerspective::Instance())
{
}


Reaction::Reaction(
	Name name,
	typename StructuralComponentImplementation<Reactant*>::Contents reactants)
	:
	physical::Class(
		this,
		new physical::Symmetry(
			PeriodicTable::Instance().GetNameFromType(*this),
			symmetry_Type::Operation())),
	LinearStructuralComponent<Reactant*>(reactants),
	Identifiable(
		name,
		&ReactionPerspective::Instance())
{
}

void Reaction::Require(Reactant* reactant)
{
	Add<Reactant*>(reactant);
}

void Reaction::Require(
	Name typeName,
	const Substance& substance)
{
	Require(
		new Reactant(
			typeName,
			substance
		));
}

void Reaction::Require(
	Name typeName,
	const StructuralComponent<Property>::Contents& properties,
	const StructuralComponent<State>::Contents& states)
{
	Require(
		new Reactant(
			typeName,
			properties,
			states
		));
}


bool Reaction::SubstancesCanReact(const Substances& toCheck) const
{
	for (
		Reactants::const_iterator rct = GetAll<Reactant*>()->begin();
		rct != GetAll<Reactant*>()->end();
		++rct
		)
	{
		for (
			Substances::iterator sbt = toCheck.begin();
			sbt != toCheck.end();
			++toCheck
			)
		{
			if (**rct != **sbt)
			{
				return false;
			}
		}
	}
	return true;
}

/*static*/ Reaction* Reaction::Initiate(StandardDimension id)
{
	return ReactionPerspective::Instance().GetTypeFromIdAs<Reaction*>(id);
}

Products Reaction::operator()(Substances& reactants) const
{
	//Always level 2, even if the user wants more (i.e. never less).
	BIO_SANITIZE_AT_SAFETY_LEVEL_2(SubstancesCanReact(reactants),
		return Process(),
		return code::FailedReaction());
}

/*static*/ Products Reaction::Attempt(
	StandardDimension id,
	Substances& reactants)
{
	BIO_SANITIZE_WITH_CACHE(ReactionPerspective::Instance().GetTypeFromIdAs<Reaction*>(id),
		return RESULT(reactants),
		return code::FailedReaction());
}

} //chemical namespace
} //bio namespace