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

#pragma once

#include "bio/chemical/solution/Solvent.h"
#include "bio/chemical/solution/Solute.h"

namespace bio {
namespace chemical {

Solute* Solvent::Diffuse(const Id& soluteId, const Id& targetSolvent=0)
{
	Solute* solute;

	//The caller is us.
	if (!targetSolvent || targetSolvent == GetId())
	{
		for (
			SmartIterator slt = GetAll< Solute* >()->Begin();
			!slt.IsAfterEnd();
			++slt)
		{
			solute = slt;
			if (solute->GetId() != soluteId || solute->GetAccessorId() != GetId())
			{
				continue;
			}
			solute->IncrementConcentration();
			return solute;
		}
		return NULL;
	}

	//The caller is not us.
	for (
		SmartIterator slt = GetAll< Solute* >()->Begin();
		!slt.IsAfterEnd();
		++slt)
	{
		solute = slt;
		if (solute->GetId() != soluteId || solute->GetAccessorId() != targetSolvent)
		{
			continue;
		}
		solute->IncrementConcentration();
		return solute;
	}

	//The caller is not us and hasn't been seen before.
	Solute* toClone = Diffuse(soluteId, 0);
	BIO_SANITIZE(toClone,,return NULL)
	solute = toClone->Clone();
	solute->SetConcentration(1);
	Add< Solute* >(solute);
	return solute;
}

const Solute* Solvent::Diffuse(const Id& soluteId, const Id& targetSolvent=0) const
{
	const Solute* solute;
	for (
		SmartIterator slt = GetAll< Solute* >()->Begin();
		!slt.IsAfterEnd();
		++slt)
	{
		solute = slt;
		if (solute->GetId() != soluteId || solute->GetAccessorId() != targetSolvent)
		{
			continue;
		}
		solute->IncrementConcentration();
		return solute;
	}

	//The given targetSolvent did not previous access this through a non-const call.
	//Let's give them our Solute, since they promised not to change it.
	for (
		SmartIterator slt = GetAll< Solute* >()->Begin();
		!slt.IsAfterEnd();
		++slt)
	{
		solute = slt;
		if (solute->GetId() != soluteId ||solute->GetAccessorId() != GetId())
		{
			continue;
		}
		return solute;
	}

	//The soluteId could not be found in *this.
	return NULL;
}

Solute* Solvent::Diffuse(const Name& soluteName, const Id& targetSolvent=0)
{
	return Diffuse(GetIdWithoutCreation(soluteName), targetSolvent);
}

const Solute* Solvent::Diffuse(const Name& soluteName, const Id& targetSolvent=0) const
{
	return Diffuse(GetIdWithoutCreation(soluteName), targetSolvent);
}

Solute* Solvent::operator[](const Id& soluteId)
{
	return Diffuse(soluteId, 0);
}

const Solute* Solvent::operator[](const Id& soluteId) const
{
	return Diffuse(soluteId, 0);
}

Solute* Solvent::operator[](const Name& soluteName)
{
	return Diffuse(soluteName, 0);
}

const Solute* Solvent::operator[](const Name& soluteName) const
{
	return Diffuse(soluteName, 0);
}

Solute* Solvent::operator[](const Id& soluteId, const Id& targetSolvent)
{
	return Diffuse(soluteId, targetSolvent);
}

const Solute* Solvent::operator[](const Id& soluteId, const Id& targetSolvent) const
{
	return Diffuse(soluteId, targetSolvent);
}

Solute* Solvent::operator[](const Name& soluteName, const Id& targetSolvent)
{
	return Diffuse(soluteId, targetSolvent);
}

const Solute* Solvent::operator[](const Name& soluteName, const Id& targetSolvent) const
{
	return Diffuse(soluteId, targetSolvent);
}

Solute* Solvent::AddImplementation(Solute* content)
{
	BIO_SANITIZE(content,,return NULL)
	content->SetEnvironment(this);
	return LinearMotif< Solute* >::AddImplementation(content);
}

Code Solvent::InsertImplementation(
	Solute* toAdd,
	const Position position = BOTTOM,
	const Id optionalPositionArg = 0, //i.e. invalid.
	const bool transferSubContents = false
)
{
	BIO_SANITIZE(toAdd,,return NULL);
	toAdd->SetEnvironment(this);
	return LinearMotif< Solute* >::InsertImplementation(toAdd, position, optionalPositionArg, transferSubContents);
}

} //chemical namespace
} //bio namespace
