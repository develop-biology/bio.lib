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

#include "Types.h"
#include "bio/physical/relativity/Perspective.h"

namespace bio {
namespace code {

//General
Code Success();

Code SuccessfullyReplaced();

Code NoErrorNoSuccess();

Code UnknownError();

Code GeneralFailure();

Code Invalid();

Code AlreadyExists();

Code Skip();

Code NotImplemented();

//Arguments
Code MissingArgument1();

Code MissingArgument2();

Code MissingArgument3();

Code MissingArgument4();

Code MissingArgument5();

Code MissingArgument6();

Code BadArgument1();

Code BadArgument2();

Code BadArgument3();

Code BadArgument4();

Code BadArgument5();

Code BadArgument6();

Code CouldNotFindValue1();

Code CouldNotFindValue2();

Code CouldNotFindValue3();

Code CouldNotFindValue4();

Code CouldNotFindValue5();

Code CouldNotFindValue6();

Code CouldNotFindValue7();

Code CouldNotFindValue8();

Code CouldNotFindValue9();

Code CouldNotFindValue10();

} //code namespace
} //bio namespace
