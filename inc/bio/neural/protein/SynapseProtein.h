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

#include "DendriteProtein.h"
#include "AxonProtein.h"

namespace bio {
namespace neural {

template <class SYNAPSE_TYPE, class PRESYNAPTIC_NEURON_TYPE, class POSTSYNAPTIC_NEURON_TYPE>
class SynapseProtein:
	public molecular::Class< SynapseProtein< SYNAPSE_TYPE, PRESYNAPTIC_NEURON_TYPE, POSTSYNAPTIC_NEURON_TYPE > >,
	public DendriteProtein< SYNAPSE_TYPE, PRESYNAPTIC_NEURON_TYPE >,
	public AxonProtein< SYNAPSE_TYPE, POSTSYNAPTIC_NEURON_TYPE >
{
public:

	/**
	 * Disambiguate all class methods. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(molecular, BIO_SINGLE_ARG(SynapseProtein< SYNAPSE_TYPE, PRESYNAPTIC_NEURON_TYPE, POSTSYNAPTIC_NEURON_TYPE >))

	BIO_DEFAULT_IDENTIFIABLE_CONSTRUCTORS(
		molecular,
		SynapseProtein,
		filter::Neural()
	)

	/**
	 * Protein method; see that class for details. <br />
	 * @param environment Vesicle to Recruit Chaperones from.
	 * @return Success() if *this is ready to Fold()
	 */
	virtual Code RecruitChaperones(molecular::Vesicle* environment)
	{
		Code ret = DendriteProtein< SYNAPSE_TYPE, PRESYNAPTIC_NEURON_TYPE >::RecruitChaperones(environment);
		BIO_SANITIZE(ret == code::Success() || ret == code::NoErrorNoSuccess(), , return ret)
		ret = AxonProtein< SYNAPSE_TYPE, POSTSYNAPTIC_NEURON_TYPE >::RecruitChaperones(environment);
		BIO_SANITIZE(ret == code::Success() || ret == code::NoErrorNoSuccess(), , return ret)
		return ret;
	}

	/**
	 * Protein method; see that class for details. <br />
	 * @return Success if *this is ready to Activate()
	 */
	virtual Code Fold()
	{
		Code ret = DendriteProtein< SYNAPSE_TYPE, PRESYNAPTIC_NEURON_TYPE >::Fold();
		BIO_SANITIZE(ret == code::Success() || ret == code::NoErrorNoSuccess(), , return ret)
		ret = AxonProtein< SYNAPSE_TYPE, POSTSYNAPTIC_NEURON_TYPE >::Fold();
		BIO_SANITIZE(ret == code::Success() || ret == code::NoErrorNoSuccess(), , return ret)
		return ret;
	}
};

} // namespace neural
} // namespace bio