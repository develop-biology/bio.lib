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

#include "SynapseProtein.h"

using namespace bio;

/**
This class is a base for all outgoing NeuronInterface methods.
These include ProcessOutgoing. For more information on Neuron interface methods, see Synapse.h
*/
template <class CONNECTION_TYPE, class NEURON_TYPE>
class AxonProtein : public SynapseProtein<CONNECTION_TYPE>
{
public:
    /**
    TODO: Build test to make sure BAD NAME appears no where in running app.
    */
    AxonProtein(Name name = "BAD NAME") : SynapseProtein<CONNECTION_TYPE>(name), mPresynapticNeuron(NULL)
    {
        // this->SetLogFilter(log::Filt::NEURON);
    }
    virtual ~AxonProtein() {}

//START: molecular::Protein overrides

    /**
    Requires a Synapse* as caller and a Neuron* as arg
    */
    virtual Code ValidateArgs(cellular::Cell* caller, void* arg)
    {
        Code ret = SynapseProtein<CONNECTION_TYPE>::ValidateArgs(caller, arg);
        if (ret != code::Success())
        {
            return ret;
        }

        if (!mPresynapticNeuron)
        {
            mPresynapticNeuron = Cast<const NEURON_TYPE, const Neuron>(this->mSynapse->GetPresynapticNeuron());
            if (!mPresynapticNeuron)
            {
                return code::BadArgument2();
            }
        }
        return code::Success();
    }

//END: molecular::Protein overrides

protected:
    const NEURON_TYPE* mPresynapticNeuron;
};
