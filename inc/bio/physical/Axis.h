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

namespace bio {
namespace physical {

class Axis
{
public:
	/**
	 *
	 */
	Axis();

	/**
	 *
	 */
	virtual ~Axis();

	/**
	 * Used as a bad return value by other methods.
	 * @return "FAILED"
	 */
	static std::string Failed();

	/**
	 * Translate the given Symmetry into something intelligible by another system.
	 * @param symmetry
	 * @return text, possibly a command (like mysql), structured values (like json), an entire program (like python), etc. or Failed().
	 */
	virtual std::string Rotate(Symmetry* symmetry) const;

	/**
	 * Translate the given text into something intelligible by this system.
	 * @return Symmetry* which can be Reified or NULL.
	 */
	virtual Symmetry* Rotate(std::string) const;

	/**
	 * jsonAxis | myWave
	 * @param particle
	 * @return Rotate(particle->Spin()) or Failed().
	 */
	virtual std::string operator|(Wave* particle) const;

	/**
	 * (*myWave) | jsonAxis("...");
	 * @param encoded
	 * @return Rotate(encoded) or NULL.
	 */
	virtual Symmetry* operator()(std::string encoded) const;

protected:

	/**
	 * Encode a single Symmetry
	 * Override this to add handlers (e.g. a switch(symmetry->GetId()) for your own SymmetryTypes, then call the parent method to handle other SymmetryTypes.
	 * Decoding is not so easy and requires overloading Rotate(std::string) directly (if we know which parts of the string are which Symmetries, then there would be no need to decode at the Axis level).
	 * @param symmetry
	 * @return the text corresponding to the given, singular Symmetry or Failed().
	 */
	virtual std::string Encode(Symmetry* symmetry) const;

};

} //physical namespace
} //bio namespace