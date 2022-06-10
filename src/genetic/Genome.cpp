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

#include "bio/genetic/Genome.h"
#include "bio/genetic/Plasmid.h"
#include "bio/genetic/common/TranscriptionFactors.h"

namespace bio {
namespace genetic {

GenomeImplementation::GenomeImplementation()
	:
	genetic::Class< GenomeImplementation >(
		this,
		"Genome",
		NULL,
		filter::Genetic())
{
	Add< TranscriptionFactor >(transcription_factor::Genome());
}

GenomeImplementation::~GenomeImplementation()
{

}

void GenomeImplementation::CacheProteins()
{
	mcRegisterPlasmid = RotateTo("RegisterPlasmid")->As< molecular::Protein* >();
	mcFetchPlasmid = RotateTo("FetchPlasmid")->As< molecular::Protein* >();
	mcRegistrationSite = mcRegisterPlasmid->GetIdWithoutCreation("Plasmid Binding Site");
	mcNameSite = mcFetchPlasmid->GetIdWithoutCreation("Name Binding Site");
	mcIdSite = mcFetchPlasmid->GetIdWithoutCreation("Id Binding Site");
	mcFetchSite = mcFetchPlasmid->GetIdWithoutCreation("Return Site");
}

Id GenomeImplementation::RegisterPlasmid(Plasmid* toRegister)
{
	BIO_SANITIZE(toRegister, , return PlasmidPerspective::InvalidId())
	
	Id ret = PlasmidPerspective::InvalidId();
	mcRegisterPlasmid->RotateTo(mcRegistrationSite)->Bind(*toRegister);
	if (mcRegisterPlasmid->Activate() == code::Success())
	{
		ret = toRegister->GetId();
		BIO_SANITIZE(ret == mcRegisterPlasmid->RotateTo(mcRegistrationSite)->Probe< Plasmid >().GetId(),,)
	}
	return ret;
}

Plasmid* GenomeImplementation::FetchPlasmid(Id plasmidId)
{
	Plasmid* ret = NULL;
	mcFetchPlasmid->RotateTo(mcIdSite)->Bind(plasmidId);
	mcFetchPlasmid->Activate();
	ret = mcFetchPlasmid->RotateTo(mcFetchSite)->As< Plasmid* >();
	return ret;
}

Plasmid* GenomeImplementation::FetchPlasmid(Name plasmidName)
{
	Plasmid* ret = NULL;
	mcFetchPlasmid->RotateTo(mcNameSite)->Bind(plasmidName);
	mcFetchPlasmid->Activate();
	ret = mcFetchPlasmid->RotateTo(mcFetchSite)->As< Plasmid* >();
	return ret;
}

} //genetic namespace
} //bio namespace
