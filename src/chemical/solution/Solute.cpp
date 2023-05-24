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
#include "bio/chemical/solution/Solution.h"
#include "bio/chemical/common/Diffusion.h"
#include "bio/chemical/reaction/Reaction.h"
#include "bio/chemical/mixture/Mix.h"

namespace bio {
namespace chemical {

Solute::~Solute()
{

}

void Solute::CommonConstructor()
{
	mDissolvedSubstance = NULL;
	mParentSolute = NULL;
	mIndexInParent = InvalidIndex();
	mDiffusionTime = diffusion::time::Destruction();
	mDiffusionEffort = diffusion::effort::Active();
}

Solute::Solute(Solute& other) :
	chemical::Class< Solute >(
		this,
		other.GetFilter()
	),
	mDissolvedSubstance(other.mDissolvedSubstance),
	mParentSolute(&other),
	mDiffusionTime(other.mDiffusionTime),
	mDiffusionEffort(other.mDiffusionEffort)
{
	mIndexInParent = other.Add(this);
	//environment is not copied.
}

Substance* Solute::GetDissolvedSubstance()
{
	return mDissolvedSubstance;
}

const Substance* Solute::GetDissolvedSubstance() const
{
	if (!mDissolvedSubstance)
	{
		if (mParentSolute)
		{
			return mParentSolute->GetDissolvedSubstance();
		}
		return NULL;
	}
	return mDissolvedSubstance;
}

Concentration Solute::GetConcentration() const
{
	return Size() + 1; //+1 to account for the Solute itself.
}


void Solute::Diffuse() const
{
	BIO_SANITIZE(mDissolvedSubstance,,return)
	if (GetEnvironment() &&
		(mDiffusionEffort == diffusion::effort::Active() || mDiffusionEffort == diffusion::effort::ActiveAndPassive())
	)
	{
		GetEnvironment()->Influx(*this);
	}
	else if (GetConcentration() > 1 &&
		(mDiffusionEffort == diffusion::effort::Passive() || mDiffusionEffort == diffusion::effort::ActiveAndPassive())
	)
	{
		for (
			SmartIterator cld = Begin();
			!cld.IsAfterEnd();
			++cld
		) {
			cld.As< Solute* >()->MixWith(*this);
		}
	}
}

Code Solute::Crest()
{
	if (mDiffusionTime != diffusion::time::Interval())
	{
		return code::NoErrorNoSuccess();
	}
	Diffuse();
	return code::Success();
}

Code Solute::MixWith(const bio::chemical::Solute& other)
{
	return MixWith(other.GetDissolvedSubstance());
}

Code Solute::MixWith(const bio::chemical::Substance* other)
{
	BIO_SANITIZE(other,,return code::BadArgument1())
	Reactants reactants;
	reactants.Add< Substance* >(mDissolvedSubstance);
	reactants.Add< Substance* >(const_cast< Substance* >(other)); //other won't be modified, but we need to cast away the const to add it to the reactants.
	Reaction::Attempt< Mix >(&reactants);
	return code::Success();
}

void Solute::SetEnvironment(Solution* environment)
{
	//Nothing need be done here.
	//In the past, we considered allowing each Solution to maintain its own Id <-> Name mapping (i.e. be a Perspective).
	//For that to be possible, we must set the Id of *this to match the name mapping of the environment.
	//However, this pattern was foregone in favor of Mix, Collapse, & Interference, which all allow Substances to be intelligently combined.
	//Now, if you would like to combine a Solute in one Solution with a Solute of a different Name in another Solution, you may engage the Mix machinery manually (i.e. just invoke Mix with both Solutes).
	this->EnvironmentDependent< Solution* >::SetEnvironment(environment);
}

Index Solute::GetIndexInParentSolution() const
{
	return mIndexInParent;
}

void Solute::Destructor()
{
	if (mDiffusionTime == diffusion::time::Destruction() || mDiffusionTime == diffusion::time::Interval())
	{
		Diffuse();
	}

	if (mParentSolute && mIndexInParent)
	{
		mParentSolute->Erase(mIndexInParent);
	}

	if (mDissolvedSubstance)
	{
		delete mDissolvedSubstance;
		mDissolvedSubstance = NULL;
	}

	Solution* env = GetEnvironment();
	if (env)
	{
		env->RemoveById< Solute* >(GetId());
	}

	delete this;
}

void Solute::SetIndexInParentSolution(Index index)
{
	mIndexInParent = index;
}

void Solute::SetDiffusionTime(const bio::DiffusionTime& diffusionTime)
{
	mDiffusionTime = diffusionTime;
}

DiffusionTime Solute::GetDiffusionTime() const
{
	return mDiffusionTime;
}

void Solute::SetDiffusionEffort(const DiffusionEffort& diffusionEffort)
{
	mDiffusionEffort = diffusionEffort;
}

DiffusionEffort Solute::GetDiffusionEffort() const
{
	return mDiffusionEffort;
}

} //chemical namespace
} //bio namespace
