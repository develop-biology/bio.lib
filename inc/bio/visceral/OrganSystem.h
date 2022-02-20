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

#pagma once

#include "bio/visceral/common/Class.h"
#include "bio/chemical/structure/LinearStructuralComponent.h"
#include "bio/molecular/EnvironmentDependent.h"

namespace bio {

class Organism;

namespace visceral {

class Organ;

/**
 * OrganSystems are very similar to Organs. The only difference is that they contain logic for combining multiple Organs.
 * For example, the heart relies on vasculature to move blood around the body. This requires a Heart Organ as well as integrations into all other Tissues in order to supply the BloodStream. In order to accommodate this functionality, we must invasively add functionality to other Organs.
 * Because the modification of existing Organs can lead to more errors and make debugging more difficult, it is not recommended that you take advantage of OrganSystem machinary unless you absolutely need to. However, if you do need to make system wide modifications, these methods are available to you.
 */
class OrganSystem :
	Class<OrganSystem>,
	public chemical::LinearArrangment<Organ*>,
	public molecular::EnvironmentDependent<Organism>
{
public:

	/**
	 *
	 */
	OrganSystem();

	/**
	 * @param id
	 */
	OrganSystem(StandardDimension id);

	/**
	 * @param name
	 */
	OrganSystem(Name name);


	/**
	 * Copies all of the given OrganSystem into *this.
	 * However, Cells, when copied, must be Differentiated. So, you must call Organogenesis() after copying in order to get a functional OrganSystem.
	 * This can get messy, so it's best to avoid trying to copy OrganSystems at all.
	 * Besides, each OrganSystem should meet a unique need of the Organism. If 2 OrganSystems are similar enough that they can be copied, they should probably be merged into a single system.
	 * @param toCopy
	 */
	OrganSystem(const OrganSystem& toCopy);


	/**
	 *
	 */
	virtual ~OrganSystem();

	/**
	 * Create all Organs and, if necessary, link them.
	 * Call the parent method (OrganSystem::Organogenesis()) when done to initialize all Organs (calls Organ::BuildMobilome() and Organ::SpecializeTissues()).
	 */
	virtual void Organogenesis();
};

} //visceral namespace
} //bio namespace
