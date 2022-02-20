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

#include "bio/cellular/Cell.h"
#include "bio/molecular/Protein.h"
#include "bio/cellular/common/Filters.h"
#include "bio/cellular/common/Properties.h"
#include "bio/log/Engine.h"
#include <string.h>
#include <algorithm>

using namespace bio;
using namespace cellular;

Cell::Cell(Name name, organismal::Organism* organism) :
	genetic::Expressor(),
    log::Writer(NULL, filter::Cellular()),
	physical::Identifiable<StandardDimension>(name, &CellPerspective::Instance())
{
}

Cell::Cell(const Cell& toCopy) :
    log::Writer(toCopy),
	genetic::Expressor(toCopy)
{

}
Cell::~Cell()
{

}

Cell* Cell::Clone() const
{
	return new Cell(*this);
}


bool Cell::operator==(const Cell& other) const
{
	return genetic::Expressor::operator==(other);
}

void ImportAll(const Cell& other)
{
	gentic::Expressor::ImportAll(other);
}

void Cell::SetLogEngine(log::Engine* logEngine)
{
	log::Writer::SetLogEngine(logEngine);
	ForEach<Protein*>(
		chemical::Reaction::Iniate<SetLogEngine>,
		this
	);
}

