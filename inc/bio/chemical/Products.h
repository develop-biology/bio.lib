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

#include "Types.h"

namespace bio {
namespace chemical {

/**
 * Products are the result of a chemical::Reaction.
 * As such, they include Substances (in order to chain Reactions)
 * And they include a Code, indicating the result of the reaction.
 * They can be treated as either Substances or a Code.
 */
class Products
{
public:

	/**
	 *
	 */
	Products();

	/**
	 * @param result
	 */
	Products(Code result);

	/**
	 * Assume m_result Success(), since we're given a value and no error to report.
	 * @param substances
	 */
	Products(const Substances& substances);

	/**
	 * @param result
	 * @param substances
	 */
	Products(
		Code result,
		const Substances& substances
	);

	/**
	 *
	 */
	virtual ~Products();

	/**
	 * @return *this as a Code.
	 */
	operator Code&();

	/**
	 * @return *this as Substances.
	 */
	operator Substances&();

protected:
	Substances m_substances;
	Code m_result;
};

} //chemical namespace
} //bio namespace
