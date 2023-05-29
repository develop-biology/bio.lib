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

#include "bio/neural/organ/Brain.h"

namespace bio {
namespace neural {

Brain::~Brain()
{

}

Code Brain::CreateDefaultProteins()
{
	Add< molecular::Protein* >(new molecular::Protein("PreSetup"));
	Add< molecular::Protein* >(new molecular::Protein("CreateNeuropils"));
	Add< molecular::Protein* >(new molecular::Protein("CreateSynapses"));
	Add< molecular::Protein* >(new molecular::Protein("CreateNeurons"));
	Add< molecular::Protein* >(new molecular::Protein("CreateNeuralFramework"));
	Add< molecular::Protein* >(new molecular::Protein("PostSetup"));
	return Organ::CreateDefaultProteins();
}

Code Brain::CacheProteins()
{
	mcPreSetup = GetByName< molecular::Protein* >("PreSetup");
	mcCreateNeuropils = GetByName< molecular::Protein* >("CreateNeuropils");
	mcCreateSynapses = GetByName< molecular::Protein* >("CreateSynapses");
	mcCreateNeurons = GetByName< molecular::Protein* >("CreateNeurons");
	mcCreateNeuralFramework = GetByName< molecular::Protein* >("CreateNeuralFramework");
	mcPostSetup = GetByName< molecular::Protein* >("PostSetup");

	return Organ::CacheProteins();
}

Code Brain::PreSetup()
{
	return (*mcPreSetup)();
}

Code Brain::CreateNeuropils()
{
	return (*mcCreateNeuropils)();
}

Code Brain::CreateSynapses()
{
	return (*mcCreateSynapses)();
}

Code Brain::CreateNeurons()
{
	return (*mcCreateNeurons)();
}

Code Brain::CreateNeuralFramework()
{
	return (*mcCreateNeuralFramework)();
}

Code Brain::PostSetup()
{
	return (*mcPostSetup)();
}

} // namespace neural
} // namespace bio
