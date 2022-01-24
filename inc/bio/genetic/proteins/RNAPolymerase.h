/*
 * This file is a part of the Biology project by eons LLC.
 * Biology (aka Develop Biology) is a framework for approaching software
 * development from a natural sciences perspective.
 *
 * Copyright (C) 2021 Séon O'Shannon & eons LLC
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

#include "bio/molecular/Protein.h"

namespace bio {
namespace genetic {

/**
 * Used for making Proteins from Genes. Technically, *this does the work of many proteins, including the ribosome. However, because it is the default way of reading a Plasmid, we name it based on
 */
class RNAPolymerase : public molecular::Protein
{
public:
	/**
	 * @param toTranscribe
	 */
	RNAPolymerase(Plasmid* toTranscribe);

	/**
	 *
	 */
	virtual ~RNAPolymerase();

	/**
	 * Assumes the m_environment is an Expressor. If it is not, *this will fail.
	 * Transcribes, then Translates all Genes from toTranscribe into the environment.
	 * @return result of all Transcription & Translation.
	 */
	virtual Code operator()();

	/**
	 * Required method from Wave. See that class for details.
	 * @return a new copy of *this.
	 */
	virtual RNAPolymerase* Clone() const;

	/**
	 * Required method from Wave. See that class for details.
	 * @return a Symmetrical image of *this
	 */
	virtual Symmetry* Spin() const;

	/**
	 * Required method from Wave. See that class for details.
	 * Reconstruct *this from the given Symmetry.
	 * @param symmetry
	 */
	virtual void Reify(Symmetry* symmetry);
};

} //genetic namespace
} //bio namespace