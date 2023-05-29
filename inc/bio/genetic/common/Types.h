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

#include "bio/molecular/common/Types.h"

namespace bio {

/**
 * Affinity is used to "recruit" various "enzymes". Effectively, these are numeric representations of functions. <br />
 * The enzymes can be a Protein, a Reaction, or simply an Excitation. <br />
 * The recruitment process is likely just invoking the enzyme. <br />
 */
BIO_ID_WITH_PERSPECTIVE(Affinity, uint8_t)

/**
 * Locations are places inside (or outside) of Expressors (e.g. Cells). <br />
 * Because places can be anything, we rely on the Translocator, Affinities, and chemical::Excitations in order to turn a Location into an actionable function. <br />
 * The Perspective for Locations is the Translocator. <br />
 */
BIO_ID(Location, uint8_t)

/**
 * TranscriptionFactors determine which Proteins are expressed in which Cells. <br />
 * They exist as unique numbers only, which are added to Expressors in order to direct Plasmid expression on startup. <br />
 * For example, if you want to add functionality to a Cell, you would start by defining YourFavoritePlasmid (genetic::Plasmid), which contains YourFavoriteProtein (molecular::Protein). You would then specify which TranscriptionFactors are required for YourFavoriteProtein in YourFavoriteGene. Once that is done, you can add the previous TranscriptionFactor to any number of Cells, all of which will begin expressing YourFavoriteProtein on startup. <br />
 * TranscriptionFactors thus allow you to pick and choose which features of your loaded modules you would like in your Cells. <br />
 * In practice, you'll likely be using other people's Plasmids, so TranscriptionFactors give you a level of control over how you want to consume external libraries in your networks. <br />
*/
BIO_ID_WITH_PERSPECTIVE(TranscriptionFactor, uint8_t)

/**
 * The PlasmidPerspective is an additional Perspective that allows Plasmid objects to be retrieved by human-readable, non-unique, and/or short Names. <br />
 * The PeriodicTable is incapable of associating common names (e.g. "MyPlasmid") with globally unique objects (e.g. MyPlasmid*). <br />
 */
BIO_TYPED_PERSPECTIVE_SINGLETON(PlasmidPerspective, Id)

} //bio namespace

namespace bio {
namespace genetic {

class RNA;

typedef ::bio::Arrangement< const RNA* > Transcriptome;

} //genetic namespace
} //bio namespace
