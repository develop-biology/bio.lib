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

#include "bio/genetic/Plasmid.h"
#include "bio/genetic/Expressor.h"
#include "bio/genetic/protein/RNAPolymerase.h"

namespace bio {
namespace genetic {

Plasmid::~Plasmid()
{

}

molecular::Protein* Plasmid::GetRNAPolymerase()
{
	return GetProtein();
}

const molecular::Protein* Plasmid::GetRNAPolymerase() const
{
	return GetProtein();
}

void Plasmid::CommonConstructor()
{
	mProtein = new RNAPolymerase(this);
}

RNA* Plasmid::TranscribeFor(Expressor* expressor) const
{
	std::string rnaName = "mRNA_";
	rnaName += GetName().AsStdString();
	RNA* ret = new RNA(rnaName.c_str());
	molecular::Protein* polymerase = ForceCast< molecular::Protein* >(GetRNAPolymerase()->Clone());
	Id bindingSite = polymerase->GetIdFromName("RNA Binding Site");
	polymerase->RecruitChaperones(expressor);
	polymerase->Fold();
	polymerase->RotateTo(bindingSite)->Bind(*ret);
	polymerase->Activate();
	polymerase->RotateTo(bindingSite)->Release();
	delete polymerase;
	return ret;
}

} //genetic namespace
} //bio namespace
