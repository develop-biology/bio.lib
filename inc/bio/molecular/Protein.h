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

#include "bio/log/Writer.h"
#include "bio/chemical/structure/LinearStructuralComponent.h"
#include "Molecule.h"
#include "bio/molecular/common/Types.h"
#include "bio/molecular/common/Class.h"

namespace bio {
namespace molecular {

class Vesicle;
class DNA;

/**
 * Please read the documentation for Molecule.h to understand how Proteins can best be used.
 * In short, Proteins provide functions for the Biology programming syntax.
 * Proteins are Molecules and can be stored inside Surfaces. This means they will work with Define<Protein>("MyFunction", *myProtein).
 * Proteins can thus be Transferred, reflected, and treated as any other Molecule, chemical::Substance, physical::Wave, etc.
 * NOTE: Because Proteins are Molecules, they are more than just simple functors; they can store variables, facilitate in Transferring variables, and act as any other data structure.
 */
class Protein :
	virtual public Molecule,
	public Class<Protein>,
	public chemical::LinearStructuralComponent< Protein >,
	public EnvironmentDependent<Vesicle>
{
public:

	/**
	 * @param name
	 * @param source
	 */
	explicit Protein(Name name = ProteinPerspective::InvalidName(), DNA* source = NULL);

	/**
	 *
	 */
	virtual ~Protein();

	/**
	 * RecruitChaperones can be used to make sure that the environment has all the necessary components for *this to function before it Fold()s.
	 * If this method returns Success, *this and its sub-Proteins will be Folded.
	 *
	 * Override this method to add whatever start-up checks you need.
	 * It is recommended to call the parent method AFTER your own checks, which will call RecruitChaperones for all sub-Proteins.
	 *
	 * For example:
	 * virtual Code RecruitChaperones(Vesicle* environment)
	 * {
	 *     BIO_SANITIZE(MyChecks(),,return code::GeneralFailure());
	 *     return Protein::RecruitChaperones(environment);
	 * }
	 *
	 * This also sets m_environment to the environment provided.
	 *
	 * @param environment
	 * @return result of chaperone recruitment & whether or not Fold should be called.
	*/
	virtual Code RecruitChaperones(Vesicle* environment);

	/**
	 * Fold will ensure *this is functional in the Molecular environment (Vesicle)
	 * This means it will make sure all Molecules, Reactions, and other Proteins needed are present.
	 * It is your responsibility to make this true for your Proteins.
	 *
	 * This is to be called ONCE on startup.
	 * It is highly recommended, though not enforced, to call Protein::Fold at the end of any overwritten function. This will call initialize for any sub proteins. If that is not done, some proteins may not be initialized and the behavior will be undefined. (follow the RecruitChaperones example).
	 *
	 * NOTE: If RecruitChaperones does not return code::Success(), Fold will not be called at all.
	 *
	 * @return result of Folding.
	 */
	virtual Code Fold();


	/**
	 * The () operator is the primary function that will be called when accessing *this.
	 * Calling Protein::operator() is referred to as "activating" the Protein.
	 * This should be overwritten to add whatever functionality is desired.
	 * It is likely desirable, though not enforced to call Protein::operator() at the end of any overwritten functions. This will cause all sub-Proteins to be activated.
	 * @return result of activation.
	*/
	virtual Code operator()();

	/**
	 * Proteins originate from DNA.
	 * This allows us to tell 2 otherwise identical Proteins apart.
	 * For example, Protein with name "DoMyAction" gets replaced with a "DoMyAction" from an updated Gene. Both Proteins will return true on == comparison but may have different functionality which would only be known by getting the m_source.
	 * @return the m_source of *this.
	 */
	virtual const DNA* GetSource() const;

protected:
	const DNA* m_source;
};

} //molecular namespace
} //bio namespace
