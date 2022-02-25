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

#include "bio/cellular/Tissue.h"
#include "bio/cellular/Cell.h"

namespace bio {
namespace cellular {

Tissue::~Tissue()
{

}

Code Tissue::DifferentiateCells()
{
	Code ret = code::Success();
	for (
		chemical::Structure< Cell* >::Contents::iterator cel = GetAll< Cell* >()->begin();
		cel != GetAll< Cell* >()->end();
		++cel
		)
	{
		(*cel)->SetEnvironment(this);
		(*cel)->Import< genetic::Plasmid* >(this);
		if ((*cel)->ExpressGenes() != code::Success() && ret == code::Success())
		{
			ret = code::UnknownError();
		}
	}

	for (
		chemical::Structure< Tissue* >::Contents::iterator tis = GetAll< Tissue* >()->begin();
		tis != GetAll< Tissue* >()->end();
		++tis
		)
	{
		(*tis)->SetEnvironment(this);
		if ((*tis)->DifferentiateCells() != code::Success() && ret == code::Success())
		{
			ret = code::UnknownError();
		}
	}
	return ret;
}

} //cellular namespace
} //bio namespace
