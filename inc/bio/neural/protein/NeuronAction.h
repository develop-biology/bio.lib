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

/**
Unneeded, but added for simplicity.
There will be none or few children of *this that do not need this.
*/
#include "bio/neural/Neuron.h"

#include "bio/cellular/Protein.h"

using namespace bio;

template <class NEURON_TYPE>
class NeuronProtein : public molecular::Protein
{
public:
    /**
    TODO: Build test to make sure BAD NAME appears no where in running app.
    */
    NeuronProtein(Name name = "BAD NAME") : molecular::Protein(name), mNeuron(NULL)
    {
        SetLogFilter(log::Filt::NEURON);
    }
    virtual ~NeuronProtein() {}

//START: molecular::Protein overrides

    virtual Code ValidateArgs(cellular::Cell* caller, void* arg)
    {
        Code ret = molecular::Protein::ValidateArgs(caller, arg);
        if (ret != code::Success())
        {
            return ret;
        }

        if (!mNeuron)
        {
            mNeuron = Cast<NEURON_TYPE, cellular::Cell>(caller);
            if (!mNeuron)
            {
                return code::BadArgument1();
            }
        }

        return code::Success();
    }

//END: molecular::Protein overrides

protected:
    NEURON_TYPE* mNeuron;
};
