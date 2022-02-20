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

#pragma once

#include "bio/physical/ThreadedPeriodic.h"
#include "bio/chemical/structure/LinearStructuralComponent.h"
#include "bio/visceral/common/Class.h"

namespace bio {

class Organism

/**
 * Habitat: Is a home!
 * Habitat: (say it with me) Is a home!
 *
 * This is where your Organisms live!
 * Each Habitat is it's own thread. Once your Organisms are Adapted to *this, you can Start() the thread and they'll come to life!
 */

class Habitat :
	public visceral::Class<Habitat>,
	public chemical::LinearStructuralComponent<Organism*>,
	public physical::ThreadedPeriodic,
{
public:
	
	/**
	 * @param name what shall we call this new land?
	 */
	Habitat(Name name);


	/**
	 * @param id does our new land have an Id?
	 */
	Habitat(StandardDimension id);

	/**
	 *
	 */
	Habitat();

	/**
	 * Copies all contents of the given Habitat into *this.
	 * NOTE: All copied Organisms need to be Adapted in order to start living in this new environment.
	 * @param toCopy
	 */
	Habitat(const Habitat& toCopy);

	/**
	 *
	 */
	virtual ~Habitat();

	/**
	 * Causes each Organism to undergo Morphogenesis, after which, they will be ready to live here.
	 */
	virtual void AdaptInhabitants();
};

} //bio namespace
