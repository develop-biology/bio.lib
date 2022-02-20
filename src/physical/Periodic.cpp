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

#include "bio/physical/Periodic.h"
#include "bio/physical/Properties.h"
#include "bio/physical/Symmetry.h"
#include "bio/physical/common/SymmetryTypes.h"
#include "bio/physical/Time.h"

namespace bio {
namespace physical {

/*static*/ TimeUS Periodic::GetDefaultInterval()
{
	return 200000;
}

Properties Periodic::GetClassProperties()
{
	Properties ret;
	ret.push_back(
		property::Periodic());
	return ret;
}

Periodic::Periodic(TimeUS interval)
	:
	Class(
		this,
		new Symmetry(
			"m_interval",
			symmetry_type::DefineVariable())),
	m_interval(
		interval
	),
	m_lastPeakTimestamp(
		0
	)
{
}

Periodic::~Periodic()
{
}

void Periodic::SetInterval(TimeUS interval)
{
	m_interval = interval;
}

TimeUS Periodic::GetInterval() const
{
	return m_interval;
}

Timestamp Periodic::GetTimeLastPeaked() const
{
	return m_lastPeakTimestamp;
}

float Periodic::GetIntervalInSeconds() const
{
	return (static_cast<float>(m_interval)) / 1000000.0f;
}

void Periodic::SetLastPeakTimestamp(Timestamp lastPeak)
{
	m_lastPeakTimestamp = lastPeak;
}

Symmetry* Periodic::Spin() const
{
	m_symmetry->AccessValue()->Set(
		m_interval
	);
	return Wave::Spin();
}

Code Periodic::Reify(Symmetry* symmetry)
{
	BIO_SANITIZE(symmetry, ,
		return code::BadArgument1());
	m_interval = symmetry->GetValue();
	return code::Success();
}

void Periodic::InitializeImplementation(ByteStreams args)
{
	BIO_SANITIZE(args.size() == 1 && args[0].Is(
		m_interval
	), ,
		return);
	m_interval = args[0];
}

Properties Periodic::GetProperties() const
{
	return GetClassProperties();
}

void Periodic::CheckIn()
{
	Timestamp now = GetCurrentTimestamp();
	if (now - GetTimeLastPeaked() < GetInterval())
	{
		return;
	}
	Peak();
	SetLastPeakTimestamp(
		now
	);
}

} //physical namespace
} //bio namespace
