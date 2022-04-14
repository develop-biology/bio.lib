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
	Container* cells = GetAll< Cell* >();
	BIO_SANITIZE(cells,,return code::CouldNotFindValue1())
	Cell* cellBuffer;
	for (
		SmartIterator cel = cells->Begin();
		!cel.IsAtEnd();
		++cel
		)
	{
		cellBuffer = cel;
		cellBuffer->SetEnvironment(this);
		cellBuffer->Import< genetic::Plasmid* >(this);
		if (cellBuffer->ExpressGenes() != code::Success() && ret == code::Success())
		{
			ret = code::UnknownError();
		}
	}

	Container* tissues = GetAll< Tissue* >();
	BIO_SANITIZE(tissues,,return code::CouldNotFindValue1())
	Tissue* tissueBuffer;
	for (
		SmartIterator tis = tissues->Begin();
		!tis.IsAtEnd();
		++tis
		)
	{
		tissueBuffer = tis;
		tissueBuffer->SetEnvironment(this);
		if (tissueBuffer->DifferentiateCells() != code::Success() && ret == code::Success())
		{
			ret = code::UnknownError();
		}
	}
	return ret;
}

} //cellular namespace
} //bio namespace
