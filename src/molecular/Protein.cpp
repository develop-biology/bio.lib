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

#include "bio/molecular/Protein.h"
#include "bio/molecular/common/Filters.h"
#include "bio/molecular/common/Codes.h"
#include "bio/chemical/reaction/Excitation.h"

namespace bio {
namespace molecular {

void Protein::CommonConstructor()
{
	mSource = NULL;
}

Protein::~Protein()
{
}

Code Protein::Fold()
{
	Code ret = code::Success();
	BIO_EXCITATION_CLASS(Protein, Code) fold(&Protein::Fold);
	chemical::Emission result = ForEach< Protein* >(&fold);
	//We don't care about result right now.
	return ret;
}

Code Protein::RecruitChaperones(Vesicle* environment)
{
	this->chemical::EnvironmentDependent< Vesicle* >::SetEnvironment(environment);
	BIO_EXCITATION_CLASS(Protein, Code, Vesicle*) recruitChaperones(
		&Protein::RecruitChaperones,
		environment
	);
	chemical::Emission result = ForEach< Protein* >(&recruitChaperones);
	//We don't care about result right now.
	return code::Success();
}


Code Protein::Activate()
{
	Code ret = code::Success();
	BIO_EXCITATION_CLASS(Protein, Code) activate(&Protein::Activate);
	chemical::Emission result = ForEach< Protein* >(&activate);
	//We don't care about result right now.
	return ret;
}

Code Protein::operator()()
{
	return Activate();
}

Code Protein::SetSource(const DNA* source)
{
	mSource = source;
	return code::Success();
}

const DNA* Protein::GetSource() const
{
	return mSource;
}

} //molecular namespace
} //bio namespace
