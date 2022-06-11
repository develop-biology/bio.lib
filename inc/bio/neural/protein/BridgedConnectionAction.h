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

#include "IncomingSynapseProtein.h"
#include "OutgoingSynapseProtein.h"

using namespace bio;

/**
Bridged Synapse Proteins have access to both the Incoming and Outgoing SynapseProtein members.
These types of synapses are useful for implementing new synapse types that utilize the API of your custom neuron type, but should also be flexible enough to accommodate any configuration of neurons you would like.
DO NOT USE THESE LIGHTLY!
Because you can pass data straight from the source to the target, it may be tempting to bypass the synapse system altogether. Don't do that. Meaning to say, be careful that the use of these proteins doesn't add in extra points of failure or bugs to your system.

NOTE: This class could be implemented by deriving from Incoming and Outgoing SynapseProteins. However, those classes would have to virtually inherit SynapseProtein, which causes c-style upcasts of classes that inherit from I/O SynapseProteins to fail when RTTI is disabled.
*/
template <class SynapseType, class SourceNeuronType, class TargetNeuronType>
class BridgedSynapseProtein: SynapseProtein<SynapseType>
{
public:

    /**
    TODO: Build test to make sure BAD NAME appears no where in running app.
    */
    BridgedSynapseProtein(Name name = "BAD NAME") : SynapseProtein(name)
    {

    }
    virtual ~BridgedSynapseProtein() 
    {

    }

//START: cellular::Protein overrides

    /**
    Requires a Synapse* as caller
    */
    virtual ReturnCode ValidateArgs(cellular::Cell* caller, void* arg)
    {
        ReturnCode ret = SynapseProtein::ValidateArgs(caller, arg);
        if (ret != ret::Success())
        {
            return ret;
        }

        if (!m_sourceNeuron) 
        {
            m_sourceNeuron = static_cast<const SourceNeuronType*>(m_synapse->GetSourceNeuron());
            if (!m_sourceNeuron)
            {
                return ret::BadArgument2();
            }
        }

        if (!m_targetNeuron) 
        {
            m_targetNeuron = static_cast<TargetNeuronType*>(m_synapse->GetTargetNeuron());
            if (!m_targetNeuron)
            {
                return ret::BadArgument2();
            }
        }

        return ret::Success();
    }

//END: cellular::Protein overrides

protected:
    const SourceNeuronType* m_sourceNeuron;
    TargetNeuronType* m_targetNeuron;
};
