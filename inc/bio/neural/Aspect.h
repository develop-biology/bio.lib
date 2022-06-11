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

#include "bio/neural/Types.h"
#include "bio/common/Named.h"

//TODO: rename to NeuralTransmitter.

namespace bio {

class Aspect: public Named<AspectId>
{
public:
    Aspect(Name name = AspectTracker::InvalidName());

    unsigned int GetCount() const {return m_count;}
    void SetCount(unsigned int count) {m_count = count;}
    void IncrementCount() {++m_count;}
    void DecrementCount() {--m_count;}

    bool ShouldReset() const {return m_shouldReset;}
    void SetReset(bool should) {m_shouldReset = should;}

protected:
    unsigned int m_count;
    bool m_shouldReset;
};
typedef std::vector<Aspect> Aspects;

namespace aspect 
{
    AspectId Value();
    AspectId Threshold();
} //aspect namespace

} //bio namespace

/**
To make defining aspects easier, use this macro to define the function body of your Aspect Function().
This will assign a value to a string that is identical to your FunctionName e.g. AspectTracker::Instance().NameFromId(Value()) would give "Value"
Necessitates that functionName be a part of the "aspect" namespace and that any other namespaces are already specified (e.g. using namespace)
*/
#define BIO_TRACK_ASPECT(functionName)                                         \
BIO_TRACK_FUNCTION(                                                            \
    aspect,                                                                    \
    functionName,                                                              \
    bio::AspectTracker::Instance(),                                            \
    bio::AspectId)
