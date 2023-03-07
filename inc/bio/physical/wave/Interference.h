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

#include "bio/physical/common/Types.h"
#include "bio/physical/shape/Line.h"

namespace bio {
namespace physical {

/**
 * Interference represents the strategy for combining 2 (or more) Waves. <br />
 * Because Waves do not have a concept of physical separation, their point of incidence (where they are Superpose()ed) does not have any bearing on the properties of the Waves (e.g. phase or angle), as it would in reality. Thus, the role of Interference is to determine what those properties should be and how the 2+ Waves should combine. <br />
 * For example, Interference could cause one Wave to cancel out another by setting the first to 0 or false; this would be like destructive interference. Another Interference could cause the same 2 Waves to add together, like constructive interference. <br />
 * To accommodate Waves of arbitrary complexity (i.e. the number of Wave components), Interference can be recursive. <br />
 * When accessing the Superposition of a given Symmetry, the Wave Superposing others may choose to also access the sub-interference for that Symmetry. This is done on a case-by-case basis, according to the implementation of Superpose. <br />
 * Here, Symmetries can be thought of as Wave components. <br />
 */
class Interference
{
public: 
    Interference();
    virtual ~Interference();

    /**
     * Get the Superposition for the given Symmetry. <br />
     * @param symmetry The Symmetry to get the Superposition for.
     * @return The Superposition for the given Symmetry.
     */
    virtual Superposition GetSuperpositionFor(const Id& symmetry) const;

    /**
     * Get the Interference for the given Symmetry. <br />
     * @param symmetry The Symmetry to get the Interference for.
     * @return The Interference for the given Symmetry.
     */
    virtual Interference* GetInterferenceFor(const Id& symmetry) const;

    /**
     * Set the Superposition for the given Symmetry. <br />
     * Creates a SuperSymmetry for the given Symmetry if one does not already exist. <br />
     * @param symmetry The Symmetry to set the Superposition for.
     * @param superposition The Superposition to set.
     */
    virtual void SetSuperpositionFor(const Id& symmetry, const Superposition& superposition);

    /**
     * Set the Interference for the given Symmetry. <br />
     * Creates a SuperSymmetry for the given Symmetry if one does not already exist; uses the superposition::Complex(). <br />
     * @param symmetry The Symmetry to set the Interference for.
     * @param interference The Interference to set.
     */
    virtual void SetInterferenceFor(const Id& symmetry, Interference* interference);

protected:
	virtual const SuperSymmetry* GetSuperSymmetryFor(const Id& symmetry) const;
	virtual SuperSymmetry* GetSuperSymmetryFor(const Id& symmetry);

    /**
     * Internal map of Symmetry Id to Superposition pairs, plus whatever else SuperSymmetry provides. <br />
     */
    Line mSuperSymmetries;
};

} // namespace physical
} // namespace bio 
