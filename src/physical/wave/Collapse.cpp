/*
 * This file is a part of the Biology project by eons LLC.
 * Biology (aka Develop Biology) is a framework for approaching software
 * development from a natural sciences perspective.
 *
 * Copyright (C) 2023 Séon O'Shannon & eons LLC
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

#include "bio/physical/wave/Collapse.h"

namespace bio {
namespace physical {

Collapse::Collapse(const Superposition& applyTo)
{
	SuperpositionPerspective::AssociateType(applyTo, this);
}

Collapse::~Collapse()
{

}

ByteStream& Collapse::operator()(ConstWaves& waves) const
{
	return 0;
}

/*static*/ ByteStream& Collapse::Measure(
	const Superposition& superposition,
	ConstWaves& waves
)
{
	Collapse* collapse = SuperpositionPerspective::GetTypeFromIdAs<Collapse*>(superposition);
	BIO_SANITIZE(collapse, , return 0);
	return (*collapse)(waves);
}


} // namespace physical
} // namespace bio
