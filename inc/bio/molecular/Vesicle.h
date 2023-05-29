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

#include "bio/chemical/solution/Solution.h"
#include "bio/chemical/solution/Solute.h"
#include "Molecule.h"
#include "bio/molecular/common/Types.h"

namespace bio {
namespace molecular {

/**
 * Vesicles define an inside / outside dichotomy between the Surfaces on the "outside" of *this and the Solutes (likely all Molecules) on the inside of *this. <br />
 * NOTE: The Molecules inside *this can, themselves, be Vesicles (though they don't have to be). <br />
 * <br />
 * Functionally, what Vesicle does is implement the "protected" keyword by using chemical::Solution mechanics. <br />
 * Thus, Vesicles may only interact with each other through what they expose on their Surfaces. <br />
 * <br />
 * Another way to think of Vesicles is as a "vacuum" whereby Molecules can exist in an isolated context. <br />
 * For example think of one Molecule on one side of a room and another on the other side; the Vesicle would be the room, and what is ultimately responsible for defining the scale by which the Molecule's separation can be measured. <br />
 * Furthermore, in a real-world Solution, such as water, some of the H2O molecules may decompose into H+ and OH-, forming small alkaline and acidic pockets which later combine back into neutral H2O. Small differences like this are natural and expected within a homogeneous Solution. We can thus quantify homogeneity as the inability to discern, probabilistically, the location of a significantly varied region, like a small cluster of H+ ions. Once we can say that a varied region is more likely to be in one place or another, we start to define a new Solution. For example, if all H2O molecules split and move to H+ on one side of a container and OH- on the other, we can say that we have one Solution of H+ and a separate Solution of OH-. This is another definition of Vesicle: a space which has a probability of containing some Solute which is different from the surrounding spaces. In our water example, the Vesicle has no hydrophobic encapsulation (as is typical in the real-world definition of "vesicle") and is instead simply an invisible line drawn between the H+ and OH-. This highlights the variability in Surfaces a Vesicle may have: completely transparent or completely opaque, both with nearly no Surface, or some highly specific opacity with a rich ecosystem of Surfaces. <br />
 * <br />
 * Based on the above description, there is no need to use a Solution anywhere in your code. This is true. Don't use Solutions when you can use Vesicles.
 *
 */
class Vesicle :
	public Class< Vesicle >,
	virtual protected chemical::Solution,
	virtual public Molecule
{
	friend class TransMembraneDomain;
	friend BIO_EXCITATION_CLASS(chemical::LinearMotif< Molecule* >, Molecule*, Name); //For genetic Localization and Insertion.

public:

	/**
	 * Ensure virtual methods point to Class implementations. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(molecular, Vesicle)

	/**
	 * Standard constructors. <br />
	 */
	BIO_DEFAULT_IDENTIFIABLE_CONSTRUCTORS(
		molecular,
		Vesicle,
		filter::Molecular()
	)

	/**
	 * @param toCopy
	 */
	Vesicle(const Vesicle& toCopy);

	/**
	 *
	 */
	virtual ~Vesicle();

	//Additionally disambiguate Solution vs Structure methodsr
};

} //molecular namespace
} //bio namespace
