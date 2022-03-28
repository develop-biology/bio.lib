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

#include "bio/physical/Identifiable.h"

namespace bio {
namespace chemical {

/**
 * Linear is a ____pointer implementation for Biology.
 *
 * This class is used by LinearStructuralComponents, see that class for more info.
 *
 * The term "linear" comes from the idea that instead of a 0 dimensional pile of types, as are StructuralComponents, *this can be ordered along at least 1 dimension (i.e. the StandardDimension). In other words, LinearStructuralComponents contain logic for handling their CONTENT_TYPE by Id, Name, and other aspects innate to the physical::Identifiable<StandardDimension>.
 *
 * Current features:
 * 1. Shared: determines whether or not m_component will be deleted with *this.
 *
 * Future features:
 * 1. Const: determines whether or not *this can be changed.
 *
 * NOTE: Linear is NOT VIRTUAL!
 * This is done to save space in lists. Because we do not need a vtable, we shan't have one!
 *
 * NOTE: we should support Dimensions other than the StandardDimension. However, the limitations of Atom::Bonds prevent us from indexing more than 1 template variable from the ____Interface.
 * StandardDimension here, mirrors what is used by chemical::Class and does not require any additional template specialization.
 * Plus, not supporting other Dimensions makes for cleaner inheritance / downstream code.
 * Support for other Dimensions may be added in a future release.
 */
class Linear
{
public:
	/**
	 * @param component
	 * @param shared
	 */
	Linear(physical::Identifiable< StandardDimension >* component, bool shared = false);

	/**
	 * Will delete m_component iff !m_shared.
	 */
	~Linear();

	/**
	 * NOTE: Comparison should be handled by Identifiable, i.e. by Id.
	 * @param component
	 * @return whether the component of *this matches the given component.
	 */
	bool operator==(const physical::Identifiable< StandardDimension >* component) const;

	/**
	 * NOTE: Comparison should be handled by Identifiable, i.e. by Id.
	 * @param other
	 * @return whether the other's component matches that of *this.
	 */
	bool operator==(const Linear& other) const;

	/**
	 * @return m_component
	 */
	operator physical::Identifiable< StandardDimension >*();

	/**
	 * @return m_component
	 */
	operator const physical::Identifiable< StandardDimension >*() const;

	/**
	 * @return m_component
	 */
	physical::Identifiable< StandardDimension >& operator*();

	/**
	 * @return m_component
	 */
	const physical::Identifiable< StandardDimension >& operator*() const;

	/**
	 * @return m_component
	 */
	physical::Identifiable< StandardDimension >* operator->();

	/**
	 * @return m_component
	 */
	const physical::Identifiable< StandardDimension >* operator->() const;

protected:
	physical::Identifiable< StandardDimension >* m_component;
	bool m_shared;
};

} //chemical namespace
} //bio namespace
