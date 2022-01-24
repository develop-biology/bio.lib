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

#include "bio/chemical/Class.h"
#include "bio/chemical/Reaction.h"
#include "bio/chemical/Properties.h"
#include "bio/chemical/States.h"

namespace bio {
namespace chemical {

/**
 * SetLogEngine is a chemical::Reaction that takes 2 LoggerObjects
 * The log engine of the 1st is set to that of the 2nd (i.e 1->SetLogEngine(2->GetLogEngine))
 */
class SetLogEngine :
	virtual public Reaction,
	public Class<SetLogEngine>
{
public:
	/**
	 *
	 */
	SetLogEngine();

	/**
	 *
	 */
	virtual ~SetLogEngine();

	/**
	 * Do the actual work.
	 * See Reaction.h (in bio/chemical/) for more info and the class description for what *this does.
	 * @param reactants
	 * @return reactants
	 */
	virtual Products Process(Substances& reactants);
};

} //chemical namespace
} //bio namespace