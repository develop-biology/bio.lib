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

#include "bio/common/VirtualBase.h"
#include "bio/common/macros/Macros.h"

namespace bio {

VirtualBase::VirtualBase()
	:
	mHasBeenInitialized(false)
{

}

VirtualBase::~VirtualBase()
{

}


void VirtualBase::Initialize(ByteStreams args)
{
	BIO_SANITIZE_AT_SAFETY_LEVEL_2(mHasBeenInitialized, ,
		return);
	InitializeImplementation(args);
	mHasBeenInitialized = true;
}

void VirtualBase::Initialize(ByteStream arg1)
{
	BIO_SANITIZE_AT_SAFETY_LEVEL_2(mHasBeenInitialized, ,
		return);
	ByteStreams args;
	args.Add(arg1);
	Initialize(args);
}

void VirtualBase::Initialize(
	ByteStream arg1,
	ByteStream arg2
)
{
	BIO_SANITIZE_AT_SAFETY_LEVEL_2(mHasBeenInitialized, ,
		return);
	ByteStreams args;
	args.Add(arg1);
	args.Add(arg2);
	Initialize(args);
}

void VirtualBase::Initialize(
	ByteStream arg1,
	ByteStream arg2,
	ByteStream arg3
)
{
	BIO_SANITIZE_AT_SAFETY_LEVEL_2(mHasBeenInitialized, ,
		return);
	ByteStreams args;
	args.Add(arg1);
	args.Add(arg2);
	args.Add(arg3);
	Initialize(args);
}

} //bio namespace
