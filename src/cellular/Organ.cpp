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

#include "bio/cellular/Organ.h"
#include "bio/cellular/Tissue.h"

namespace bio {
namespace cellular {

Organ::~Organ()
{

}

Code Organ::CacheProteins()
{
	mcGrowTissues = GetByName< molecular::Protein* >("GrowTissues");
	return cellular::Class< Organ >::CacheProteins();
}

Code Organ::CreateDefaultProteins()
{
	Add< molecular::Protein* >(new molecular::Protein("GrowTissues"));
	return cellular::Class< Organ >::CreateDefaultProteins();
}

Code Organ::GrowTissues()
{
	return (*mcGrowTissues)();
}

Code Organ::SpecializeTissues()
{
	Code ret = code::Success();
	Container* tissues = GetAll< Tissue* >();
	BIO_SANITIZE(tissues, ,
		return code::CouldNotFindValue1())
	Tissue* tissue;
	for (
		SmartIterator tis = tissues->Begin();
		!tis.IsAfterEnd();
		++tis
		)
	{
		tissue = tis;
		tissue->Import< genetic::Plasmid* >(As< chemical::LinearMotif< genetic::Plasmid* >* >());
		if (tissue->ExpressGenes() != code::Success() && ret == code::Success())
		{
			ret = code::UnknownError();
		}
		if (tissue->DifferentiateCells() != code::Success() && ret == code::Success())
		{
			ret = code::UnknownError();
		}
	}
	return ret;
}

} //cellular namespace
} //bio namespace
