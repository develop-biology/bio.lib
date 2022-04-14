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
 * Sites are types of places.
 * The place itself should be specified by Name or Id.
 * Because places can be anything, we rely on Perspective::AssociateType and chemical::Excitation in order to turn a Site into an actionable function.
 * Sites are identical to Sites except they have an insertion method, rather than an extraction method.
 * See Localization.h for more info.
 */
BIO_ID(Site,
	uint8_t)

BIO_PERSPECTIVE_SINGLETON(LocalizationSitePerspective,
	Site)

BIO_PERSPECTIVE_SINGLETON(InsertionSitePerspective,
	Site)

/**
 * TranscriptionFactors determine which Proteins are expressed in which Cells.
 * They exist as unique numbers only, which are added to Expressors in order to direct Plasmid expression on startup.
 * For example, if you want to add functionality to a Cell, you would start by defining YourFavoritePlasmid (genetic::Plasmid), which contains YourFavoriteProtein (cellular::Protein). You would then specify which TranscriptionFactors are required for YourFavoriteProtein in YourFavoriteGene. Once that is done, you can add the previous TranscriptionFactor to any number of Cells, all of which will begin expressing YourFavoriteProtein on startup.
 * TranscriptionFactors thus allow you to pick and choose which features of your loaded modules you would like in your Cells.
 * In practice, you'll likely be using other people's Plasmids, so TranscriptionFactors give you a level of control over how you want to consume external libraries in your networks.
*/
BIO_ID_WITH_PERSPECTIVE(TranscriptionFactor,
	uint8_t)
} //bio namespace

namespace bio {
namespace genetic {

class RNA;

typedef std::vector< const RNA* > Transcriptome;

BIO_PERSPECTIVE_SINGLETON(RNAPerspective,
	StandardDimension)

BIO_PERSPECTIVE_SINGLETON(PlasmidPerspective,
	StandardDimension)
} //genetic namespace
} //bio namespace
