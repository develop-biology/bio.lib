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

#include "Linear.h"
#include "bio/common/container/Arrangement.h"

namespace bio {
namespace physical {

/**
 * Lines are Linear Arrangements. <br />
 *
 * NOTE: We reserve Position 0 as invalid. <br />
 *
 * @tparam STORE
 */
class Line :
	public Arrangement< Linear >
{
public:

	/**
	 * Like Containers, Lines may only be constructed explicitly to avoid ambiguity when passing numbers to a function with 1 or many argument signatures.
	 * @param expectedSize
	 */
	explicit Line(Index expectedSize = 2);

	/**
	 * Copy ctor for pointers. <br />
	 * Dereferences other then Imports all contents from other into *this. <br />
	 * @param other
	 */
	Line(const Container* other);

	/**
	 *
	 */
	virtual ~Line();

	/**
	 * Get the position of an Identifiable< StandardDimension >* with the given name in *this. 
	 * @param name
	 * @return an Index matching the given name or InvalidIndex().
	 */
	virtual Index SeekToName(Name name);

	/**
	 * Get the position of an Identifiable< StandardDimension >* with the given id in *this. 
	 * @param id
	 * @return an Index matching the given id or InvalidIndex().
	 */
	virtual Index SeekToId(StandardDimension id);

	/**
	 * We want to return an Identifiable< StandardDimension >*, not a Linear. <br />
	 * @param index
	 * @return an Identifiable< StandardDimension >* from the Linear at the given Index.
	 */
	ByteStream Access(const Index index);

	/**
	 * We want to return an Identifiable< StandardDimension >*, not a Linear. <br />
	 * @param index
	 * @return an Identifiable< StandardDimension >* from the Linear at the given Index.
	 */
	const ByteStream Access(const Index index) const;

	/**
	 * Since we operate on Identifiable< StandardDimension >*, not Linears, we want to treat the external datum as Identifiable< StandardDimension >*. <br />
	 * @param internal
	 * @param external
	 * @return whether or not the Linear at the given Index is equal to the provided Identifiable< StandardDimension >*.
	 */
	virtual bool AreEqual(
		Index internal,
		const ByteStream external
	) const;

	/**
	 * Convenience wrapper around OptimizedAccess. <br />
	 * @param index
	 * @return the given position casted to an Identifiable< StandardDimension >*
	 */
	virtual Identifiable< StandardDimension >* LinearAccess(Index index);

	/**
	 * Convenience wrapper around OptimizedAccess. <br />
	 * @param index
	 * @return the given position casted to an Identifiable< StandardDimension >*
	 */
	virtual const Identifiable< StandardDimension >* LinearAccess(Index index) const;

protected:
	Iterator* mTempItt;
};

} //physical namespace
} //bio namespace
