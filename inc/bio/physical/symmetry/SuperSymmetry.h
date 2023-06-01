/*
 * This file is a part of the Biology project by eons LLC.
 * Biology (aka Develop Biology) is a framework for approaching software
 * development from a natural sciences perspective.
 *
 * Copyright (C) 2023 Séon O'Shannon & eons LLC
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

#include "Symmetry.h"
#include "bio/physical/common/Superpositions.h"

namespace bio {
namespace physical {

/**
 * SuperSymmetry extends Symmetry by adding a Superposition and Interference. <br />
 * SuperSymmetry enables recursive Interference patterns. <br />
 * This class should not be used directly. <br />
 * See Interference.h for more information. <br />
 */
class SuperSymmetry :
	public Symmetry
{
public:
	/**
	 * @param symmetry
	 * @param superposition
	 * @param interference
	 */
	SuperSymmetry(
		const Id& symmetry,
		const Superposition& superposition = 0,
		Interference* interference = NULL
	);

	/**
	 * Deletes mInterference if it is not NULL.
	 */
	virtual ~SuperSymmetry();

	/**
	 * @return  mSuperposition
	 */
	const Superposition& GetSuperposition() const;

	/**
	 * @param superposition
	 */
	void SetSuperposition(const Superposition& superposition);

	/**
	 * @return  mInterference
	 */
	const Interference* GetInterference() const;

	/**
	 * @param interference
	 */
	void SetInterference(Interference* interference);

protected:
	Superposition mSuperposition;
	Interference* mInterference;
};

} // namespace physical
} // namespace bio
