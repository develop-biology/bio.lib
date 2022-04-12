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

#include "bio/chemical/reaction/Products.h"
#include "bio/physical/common/Codes.h"

namespace bio {
namespace chemical {

Products::Products()
	:
	m_result(CodePerspective::InvalidId())
{

}

Products::Products(Code result)
	:
	m_result(result)
{

}

Products::Products(const Substances* substances)
	:
	m_result(code::Success()),
	m_substances(*substances)
{

}

Products::Products(const Reactants* reactants)
	:
	m_result(code::Success()),
	m_substances(reactants->GetAllAsVector< Substance* >())
{

}

Products::Products(
	Code result,
	const Substances* substances
)
	:
	m_result(result),
	m_substances(*substances)
{

}

Products::~Products()
{

}

Products::operator Code()
{
	return m_result;
}

Products::operator Substances()
{
	return m_substances;
}

Products::operator Reactants()
{
	return Reactants(m_substances);
}

bool Products::operator!=(const Code code) const
{
	return m_result != code;
}

bool Products::operator==(const Code code) const
{
	return m_result == code;
}

} //chemical namespace
} //bio namespace
