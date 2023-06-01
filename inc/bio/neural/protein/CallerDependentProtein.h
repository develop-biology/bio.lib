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

#include "bio/neural/common/Filters.h"
#include "bio/molecular/Protein.h"

namespace bio {
namespace neural {

template < class CALLER >
class CallerDependentProtein :
	public molecular::Class< CallerDependentProtein< CALLER > >,
	public molecular::Protein
{
public:

	/**
	 * Disambiguate all class methods. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(molecular, CallerDependentProtein< CALLER >)

	BIO_DEFAULT_IDENTIFIABLE_CONSTRUCTORS(
		molecular,
		CallerDependentProtein,
		filter::Neural()
	)
	
	/**
	 * Protein method; see that class for details. <br />
	 * @param environment Vesicle to Recruit Chaperones from.
	 * @return Success() if *this is ready to Fold()
	 */
	virtual Code RecruitChaperones(molecular::Vesicle* environment)
	{
		BIO_SANITIZE(environment, , return code::GeneralFailure())
		mCaller = ChemicalCast< CALLER* >(environment);
		BIO_SANITIZE(mCaller, , return code::GeneralFailure())
		
		return molecular::Protein::RecruitChaperones(environment);
	}

	/**
	 * Protein method; see that class for details. <br />
	 * @return Success if *this is ready to Activate()
	 */
	virtual Code Fold()
	{
		return molecular::Protein::Fold();
	}
	
protected:
	CALLER* mCaller;
};

} // namespace neural
} // namespace bio
