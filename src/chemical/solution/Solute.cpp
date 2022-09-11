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

#include "bio/chemical/solution/Solute.h"

namespace bio {
namespace chemical {

Solute::~Solute()
{

}

void Solute::CommonConstructor()
{
	mConcentration = 0;
	mSolventAccessor = 0;
	mParentSolvent = NULL;
	mIdInParent = 0; //InvalidId().
}

Concentration Solute::GetConcentration() const
{
	return mConcentration;
}

void Solute::IncrementConcentration() const
{
	++mConcentration;
}

void Solute::DecrementConcentration() const //const required to decrement const Solutes.
{
	--mConcentration;
	if (!mConcentration)
	{
		const_cast< Solute* >(this)->Destructor();
	}
}

void Solute::SetConcentration(Concentration toSet) const
{
	mConcentration = toSet;
	if (!mConcentration)
	{
		const_cast< Solute* >(this)->Destructor();
	}
}

void Solute::Resolve() const
{
	if (mParentSolvent && mIdInParent)
	{
		mParentSolvent->Dissolve(this, mIdInParent);
	}
}

void Solute::SetEnvironment(Solvent* environment)
{
	Name myName = GetName();
	SetPerspective(environment);
	SetName(myName); //preserve Name, not Id.
	EnvironmentDependent< Solvent* >::SetEnvironment(environment);
}

void Solute::Destructor()
{
	if (mParentSolvent && mIdInParent)
	{
		Resolve();
		Solute* parentSolute = mParentSolvent->GetById(mIdInParent);
		BIO_SANITIZE(parentSolute, parentSolute->DecrementConcentration(),)
	}
	Solvent* env = GetEnvironment();
	if (env)
	{
		env->RemoveById< Solute* >(GetId());
	}
	delete this;
}

} //chemical namespace
} //bio namespace
