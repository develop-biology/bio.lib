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

#include "bio/molecular/Protein.h"

namespace bio {
namespace genetic {

class Plasmid;

class RNA;

/**
 * Used for making RNA from Genes. <br />
 * RNA Polymerase must be fed an RNA molecule to add Gene*s to prior to each Activation(). See FeedRNA(), below. <br />
 */
class RNAPolymerase :
	public molecular::Protein
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
	 * Assumes the mEnvironment is an Expressor. If it is not, *this will fail. <br />
	 * Transcribes, then Translates all Genes from toTranscribe into the environment. <br />
	 * @return result of all Transcription & Translation.
	 */
	virtual Code Activate();

protected:
	Id mcRna;
};

} //genetic namespace
} //bio namespace
