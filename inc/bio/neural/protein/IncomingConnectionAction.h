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
This class is a base for all incoming NeuronInterface methods.
These include ProcessAddition and ProcessRemoval. For more information on Neuron interface methods, see Synapse.h
*/
template <class CONNECTION_TYPE, class NEURON_TYPE>
class IncomingSynapseProtein : public SynapseProtein<CONNECTION_TYPE>
{
public:
    /**
    TODO: Build test to make sure BAD NAME appears no where in running app.
    */
    IncomingSynapseProtein(Name name = "BAD NAME") : SynapseProtein<CONNECTION_TYPE>(name), m_targetNeuron(NULL)
    {
        // this->SetLogFilter(log::Filt::NEURON);
    }
    virtual ~IncomingSynapseProtein() {}

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

        if (!m_targetNeuron)
        {
            m_targetNeuron = Cast<NEURON_TYPE, Neuron>(this->m_synapse->GetTargetNeuron());
            if (!m_targetNeuron)
            {
                return ret::BadArgument2();
            }
        }
        return ret::Success();
    }

//END: cellular::Protein overrides

protected:
      NEURON_TYPE* m_targetNeuron;
};
