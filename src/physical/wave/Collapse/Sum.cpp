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

#include "bio/physical/wave/collapse/Sum.h"
#include "bio/physical/symmetry/Symmetry.h"

namespace bio {
namespace physical {

Sum::Sum(
	const Superposition& applyTo
) :
	physical::Class< Sum >(this, NULL),
	Collapse(applyTo)
{

}

Sum::~Sum()
{
	
}

ByteStream Sum::operator()(const ConstWaves& waves) const
{
	bool result;
	for (
		SmartIterator wav = waves.Begin();
		!wav.IsAfterEnd();
		++wav
	) {
		const Wave* wave = wav.As< const Wave* >()->AsWave();
		const Symmetry* symmetry = wave->Spin();
		if (symmetry->GetValue().Is< bool >()) {
			result = result && symmetry->GetValue().As< bool >();
		}
	}
	return ByteStream(result);
}

} // namespace physical
} // namespace bio
