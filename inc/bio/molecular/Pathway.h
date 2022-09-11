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

#include "bio/chemical/common/Class.h"
#include "bio/chemical/reaction/Reaction.h"
#include "bio/chemical/structure/motif/LinearMotif.h"

namespace bio {
namespace molecular {

/**
 * A Pathway is a series chemical::Reactions that accomplishes some larger goal. <br />
 * Pathways, are themselves Reactions and can be invoked using the same calls (e.g. chemical::Reaction::Initiate<MyPathway>. <br />
 * NOTE: in real life, pathways often include protein-catalyzed reactions. You can do that here too! <br />
 *
 * NOTE: while Pathways are Reactions, they should not have any Requirements / Reactants nor should they perform any Process of their own. Instead, they should rely entirely on the Reactions added to them. <br />
 * With the above stated, Pathways can be treated like Reactions for the purpose of being added to other Pathways ;) <br />
 *
 * Pathways are First-In-First-Out (FIFO) ordered. <br />
 * The first call to Add<Reaction*>() will be the first Reaction run by *this and the Reaction that determines the required Reactants to even begin *this Process. <br />
 * The last call to Add<Reaction*>() will be the last Reaction called and will determine the ultimate Products returned by *this Process, should it successfully run to completion. <br />
 * i.e. return last(middle(first(reactants))); <br />
 *
 * TODO: Add switching logic for Products Code. <br />
 */
class Pathway :
	public chemical::Class< Pathway >,
	public chemical::Reaction,
	public Horizontal< chemical::LinearMotif< chemical::Reaction* > >
{
public:
	/**
	 * Ensure virtual methods point to Class implementations. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(chemical, Pathway)

	/**
	 * Standard constructors. <br />
	 */
	BIO_DEFAULT_IDENTIFIABLE_CONSTRUCTORS(
		chemical,
		Pathway,
		filter::Chemical()
	)

	/**
	 *
	 */
	virtual ~Pathway();

	/**
	 * The Process of a Pathway is the series of Reactions it includes. <br />
	 * This is FIFO ordering (see class description for details). <br />
	 * @param reactants
	 * @return the Products from the last Reaction or a code::FailedReaction(), if any step did not succeed.
	 */
	virtual chemical::Products Process(chemical::Reactants* reactants);

	/**
	 * *this shouldn't have an Requirements / Reactants, so instead we check the first Reaction in *this. <br />
	 * @param toCheck
	 * @return whether or not the first Reaction in *this can use the given Substances.
	 */
	virtual bool ReactantsMeetRequirements(const chemical::Reactants* toCheck) const;
};

} //molecular namespace
} //bio namespace
