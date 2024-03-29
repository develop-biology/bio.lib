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

#include "bio/neural/cell/StemCell.h"

namespace bio {
namespace neural {

StemCell::~StemCell()
{

}

Code StemCell::CacheProteins()
{
	return cellular::Cell::CacheProteins();
}

Code StemCell::CreateDefaultProteins()
{
	return cellular::Cell::CreateDefaultProteins();
}

Code StemCell::ResetAllPossiblePotentials()
{
	Code code;
	for(
		SmartIterator pot = GetAll< MembranePotential* >()->Begin();
		!pot.IsAfterEnd();
		++pot
	) {
		code = pot.As< MembranePotential* >()->Reset();
		BIO_SANITIZE(code == code::Success() || code == code::NoErrorNoSuccess(), , return code)
	}
	return code::Success();
}

} //neural namespace
} //bio namespace
