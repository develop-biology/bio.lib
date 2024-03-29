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

#include "bio/physical/Periodic.h"
#include "bio/physical/Properties.h"
#include "bio/physical/symmetry/Symmetry.h"
#include "bio/physical/common/SymmetryTypes.h"
#include "bio/physical/Time.h"

namespace bio {
namespace physical {

/*static*/ Milliseconds Periodic::GetDefaultInterval()
{
	return 200;
}

Properties Periodic::GetClassProperties()
{
	Properties ret;
	ret.Add(
		property::Periodic());
	return ret;
}

Periodic::Periodic(Milliseconds interval)
	:
	Class(
		this,
		new Symmetry(
			"mInterval",
			symmetry_type::Value())),
	mInterval(interval),
	mLastCrestTimestamp(0)
{
}

Periodic::~Periodic()
{
}

Code Periodic::SetInterval(Milliseconds interval)
{
	mInterval = interval;
	return code::Success();
}

Milliseconds Periodic::GetInterval() const
{
	return mInterval;
}

Timestamp Periodic::GetTimeLastCrested() const
{
	return mLastCrestTimestamp;
}

float Periodic::GetIntervalInSeconds() const
{
	return (static_cast<float>(mInterval)) / 1000.0f;
}

void Periodic::SetLastCrestTimestamp(Timestamp lastCrest)
{
	mLastCrestTimestamp = lastCrest;
}

const Symmetry* Periodic::Spin() const
{
	mSymmetry->AccessValue()->Set(mInterval);
	return Wave::Spin();
}

Code Periodic::Refiy(const Symmetry* symmetry)
{
	BIO_SANITIZE(symmetry, , return code::BadArgument1());
	mInterval = symmetry->GetValue();
	return code::Success();
}

void Periodic::InitializeImplementation(ByteStreams& args)
{
	BIO_SANITIZE(args[args.GetEndIndex()].Is(mInterval), , return);
	mInterval = args[args.GetEndIndex()];
}

Properties Periodic::GetProperties() const
{
	return GetClassProperties();
}

bool Periodic::CheckIn()
{
	Timestamp now = GetCurrentTimestamp();
	if (now - GetTimeLastCrested() < GetInterval())
	{
		return false;
	}
	Crest();
	SetLastCrestTimestamp(now);
	return true;
}

} //physical namespace
} //bio namespace
