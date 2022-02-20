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

#include "bio/molecular/Protein.h"
#include "bio/molecular/common/Filters.h"
#include "bio/molecular/reactions/ActivateProtein.h"
#include "bio/molecular/reactions/FoldProtein.h"
#include "bio/molecular/reactions/RecruitChaperonesForProtein.h"
#include "bio/physical/common/Codes.h"
#include "bio/chemical/reactions/SetLogEngine.h"

namespace bio {
namespace molecular {

Protein::Protein(
	Name name,
	DNA* source)
	:
	chemical::LinearStructuralComponent<Protein, StandardDimension>(
		name,
		&ProteinPerspective::Instance()),
	Writer(
		NULL,
		filter::Molecular()),
	m_source(source),
	m_environment(NULL)
{
}

Protein::~Protein()
{
}

Protein* Protein::Clone() const
{
	return new Protein(*this);
}

bool Protein::operator==(const Protein& other)
{
	return GetSource() == other.GetSource() && Molecule::operator==(other)
}

bool operator==(const StandardDimension& id) const
{
	return Identifiable<StandardDimension>::operator==(id);
}

const DNA* GetSource() const
{
	return m_source;
}

Code Protein::Fold()
{
	Code ret = code::Success();
	ForEach<Protein*>(chemical::Reaction::Initiate<FoldProtein>());
	return ret; //TODO: Get Code from children?
}

Code Protein::RecruitChaperones(Vesicle* environment)
{
	SetEnvironment(environment);
	ForEach<Protein*>(
		chemical::Reaction::Initiate<RecruitChaperonesForProtein>(),
		environment
	);
	return code::Success();//TODO: Get Code from children?
}


Code Protein::operator()()
{
	Code ret = code::Success();
	ForEach<Protein*>(chemical::Reaction::Initiate<ActivateProtein>());
	return ret; //TODO: Get Code from children?
}

void Protein::SetLogEngine(log::Engine* logEngine)
{
	log::Writer::SetLogEngine(logEngine);
	ForEach<Protein*>(
		chemical::Reaction::Iniate<SetLogEngine>,
		this
	);
}

} //molecular namespace
} //bio namespace
