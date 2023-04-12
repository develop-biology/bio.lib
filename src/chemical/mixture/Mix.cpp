/*
 * This file is a part of the Biology project by eons LLC.
 * Biology (aka Develop Biology) is a framework for approaching software
 * development from a natural sciences perspective.
 *
 * Copyright (C) 2023 Séon O'Shannon & eons LLC
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

#include "bio/chemical/mixture/Mix.h"

namespace bio {
namespace chemical {

Mix::Mix() :
	chemical::Class< Mix >(
		this,
		"Mix",
		filter::Chemical(),
		symmetry_type::Operation()
	),
	Reaction("Mix")
{

}

Mix::~Mix()
{

}

Products Mix::Process(Reactants* reactants) const
{
	SmartIterator sub = reactants->Covalent< LinearMotif< Substance* > >::Object()->GetAllImplementation()->Begin();
	Substance* primeSubstance = sub.As< Substance* >();
	const Miscibility* miscibility = NULL
	for (
		++sub;
		!sub.IsAfterEnd();
		++sub
	)
	{
		const Wave* substanceWave = sub.As< Substance* >()->AsWave();

		//Each Property has its own Miscibility and is Superposed separately.
		const Properties properties = physical::Wave::GetResonnanceBetween(primeSubstance->AsWave(), substanceWave);
		for (
			SmartIterator prp = properties.Begin();
			!prp.IsAfterEnd();
			++prp
			)
		{
			miscibility = MiscibilityPerspective::Instance()->GetTypeFromIdAs< Miscibility* >(Cast< PropertyDimension >(prp.As< Property >()));
			BIO_SANITIZE(miscibility,,continue)
			
			//The miscibility must perform the appropriate cast of sub.
			//Superpose should now be able to ForceCast the displacement to what it expects.
			const Wave* displacement = miscibility->GetDisplacement(substanceWave);
	
			//Interference gives us the Superposition for the primeSubstance's Symmetry, and thus determines how the Superposed Wave will Collapse.
			primeSubstance->Superpose(displacement, miscibility->GetInterference());
		}
	}
}

bool Mix::ReactantsMeetRequirements(const Reactants* toCheck) const
{
	//Lengthy call to be a bit more optimized than the easier GetCount< Substance* >() method.
	toCheck->Covalent< LinearMotif< Substance* > >::Object()->GetCountImplementation() > 1;
}

} //chemical namespace
} //bio namespace
