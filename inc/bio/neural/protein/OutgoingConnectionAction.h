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
class OutgoingSynapseProtein : public SynapseProtein<CONNECTION_TYPE>
{
public:
    /**
    TODO: Build test to make sure BAD NAME appears no where in running app.
    */
    OutgoingSynapseProtein(Name name = "BAD NAME") : SynapseProtein<CONNECTION_TYPE>(name), m_sourceNeuron(NULL)
    {
        // this->SetLogFilter(log::Filt::NEURON);
    }
    virtual ~OutgoingSynapseProtein() {}

//START: cellular::Protein overrides

    /**
    Requires a Synapse* as caller and a Neuron* as arg
    */
    virtual ReturnCode ValidateArgs(cellular::Cell* caller, void* arg)
    {
        ReturnCode ret = SynapseProtein<CONNECTION_TYPE>::ValidateArgs(caller, arg);
        if (ret != ret::Success())
        {
            return ret;
        }

        if (!m_sourceNeuron)
        {
            m_sourceNeuron = Cast<const NEURON_TYPE, const Neuron>(this->m_synapse->GetSourceNeuron());
            if (!m_sourceNeuron)
            {
                return ret::BadArgument2();
            }
        }
        return ret::Success();
    }

//END: cellular::Protein overrides

protected:
    const NEURON_TYPE* m_sourceNeuron;
};
