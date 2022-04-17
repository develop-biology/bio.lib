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

namespace bio {
namespace physical {

/**
 * physical::Symmetry only contains a Name, a type, and a value. <br />
 * See SymmetryTypes.h for what types of Symmetry are available on a physical level. <br />
 * Both Symmetry and SymmetryTypes are intended to grow with each namespace, as the complexity of what is being Rotated grows. <br />
 */
class Symmetry :
	public Identifiable< SymmetryType >
{
public:
	Symmetry(
		Name name,
		Name type
	);

	Symmetry(
		Name name,
		SymmetryType type
	);

	virtual ~Symmetry();

	Name GetType() const
	{
		return Identifiable< SymmetryType >::GetName();
	}

	SetValue(const ByteStream& bytes);

	AddSymmetry(Symmetry
	* symmetry); <br />

protected:
	Name m_name;
	ByteStream m_value;
};

} //physical namespace
} //bio namespace
