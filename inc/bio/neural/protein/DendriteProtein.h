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

#include "CallerDependentProtein.h"

namespace bio {
namespace neural {

template <class DENDRITE_TYPE, class POSTSYNAPTIC_NEURON_TYPE>
class DendriteProtein:
	public molecular::Class< DendriteProtein< DENDRITE_TYPE, POSTSYNAPTIC_NEURON_TYPE > >,
	virtual public CallerDependentProtein< DENDRITE_TYPE >
{
public:
    
	/**
	 * Disambiguate all class methods. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(molecular, BIO_SINGLE_ARG(DendriteProtein< DENDRITE_TYPE, POSTSYNAPTIC_NEURON_TYPE >))

	BIO_DEFAULT_IDENTIFIABLE_CONSTRUCTORS(
		molecular,
		DendriteProtein,
		filter::Neural()
	)
	
	/**
	 * Protein method; see that class for details. <br />
	 * @param environment Vesicle to Recruit Chaperones from.
	 * @return Success() if *this is ready to Fold()
	 */
	virtual Code RecruitChaperones(molecular::Vesicle* environment)
	{
		return CallerDependentProtein< DENDRITE_TYPE >::RecruitChaperones(environment);
	}

	/**
	 * Protein method; see that class for details. <br />
	 * @return Success if *this is ready to Activate()
	 */
	virtual Code Fold()
	{
		Code ret = CallerDependentProtein< DENDRITE_TYPE >::Fold();
		mPostsynapticNeuron = ChemicalCast< POSTSYNAPTIC_NEURON_TYPE* >(this->mCaller->GetPostsynapticNeuron());
		BIO_SANITIZE(mPostsynapticNeuron, , return code::GeneralFailure())
		return ret;
	}
	
protected:
      POSTSYNAPTIC_NEURON_TYPE* mPostsynapticNeuron;
};

} // namespace neural
} // namespace bio
