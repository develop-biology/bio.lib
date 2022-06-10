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

#include "bio/physical/shape/Linear.h"

namespace bio {
namespace physical {

Linear::Linear(
	Identifiable< Id >* component,
	bool shared
)
	:
	mComponent(component),
	mShared(shared)
{

}

Linear::Linear(const Linear& toCopy) :
	mComponent(toCopy.mComponent),
	mShared(true)
{

}

#if BIO_CPP_VERSION >= 11
Linear::Linear(const Linear&& toMove) :
	mComponent(toMove.mComponent),
	mShared(toMove.mShared)
{

}
#endif

Linear::~Linear()
{
	if (!mShared && mComponent)
	{
		delete mComponent;
		mComponent = NULL;
	}
}

Identifiable< Id >& Linear::operator*()
{
	return *mComponent;
}

const Identifiable< Id >& Linear::operator*() const
{
	return *mComponent;
}

Linear::operator Identifiable< Id >*()
{
	return mComponent;
}

Linear::operator const Identifiable< Id >*() const
{
	return mComponent;
}

bool Linear::operator==(const Linear& other) const
{
	return *mComponent == *other.mComponent;
}

bool Linear::operator==(const Identifiable< Id >* component) const
{
	BIO_SANITIZE(component, ,
		return false)
	return *mComponent == *component;
}

Identifiable< Id >* Linear::operator->()
{
	return mComponent;
}

const Identifiable< Id >* Linear::operator->() const
{
	return mComponent;
}

bool Linear::IsShared() const
{
	return mShared;
}

void Linear::SetShared(bool shouldShare)
{
	mShared = shouldShare;
}

} //physical namespace
} //bio namespace
