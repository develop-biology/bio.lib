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

#include "bio/neural/affinity/Affinity.h"
#include "bio/neural/cell/StemCell.h"

namespace bio {
namespace neural {

Affinity::~Affinity()
{

}

Affinity::Strength Affinity::GetStrengthOfAttractionTo(const physical::Wave* wave) const
{
	Strength ret = genetic::Affinity::GetStrengthOfAttractionTo(wave);
	const StemCell* stemCell = ChemicalCast< const StemCell* >(wave);
	BIO_SANITIZE(stemCell,,return ret)

	ret += MeasureAttractionAlong< Feature >(stemCell);

	//neural::Cells store MembranePotential*s, not Potentials, so we have to adapt MeasureAttractionAlong & GetAttractionFor.
	const Container* container = GetAll< physical::Attraction< Potential > >();
	BIO_SANITIZE(container,,return ret)
	for (
		SmartIterator pot = container->End();
		!pot.IsBeforeBeginning();
		--pot
	) {
		const physical::Attraction< Potential > attraction = pot.As< physical::Attraction< Potential > >();
		if(stemCell->GetById< MembranePotential* >(Id(attraction.GetDimension()))) {
			ret += attraction.GetForce();
		}
	}
}

} //neural namespace
} //bio namespace
