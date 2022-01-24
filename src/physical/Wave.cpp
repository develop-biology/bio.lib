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

#include "bio/physical/Wave.h"
#include "bio/common/Macros.h"

namespace bio {
namespace physical {

Wave::Wave(Symmetry* symmetry) :
	m_symmetry(symmetry),
	m_signal(NULL)
{
}

Wave::~Wave()
{
	BIO_SANITIZE_AT_SAFETY_LEVEL_2(m_symmetry, delete m_symmetry; m_symmetry=NULL,);
}

Wave* Wave::Clone() const
{
	return new Wave(*this);
}

Symmetry* Wave::Spin() const
{
	return m_symmetry;
}

void Wave::Reify(Symmetry* symmetry)
{
	(*m_symmetry) = *symmetry;
}

void Wave::operator|(Symmetry* symmetry)
{
	Reify(symmetry);
}

Wave* Wave::Modulate(Wave* signal)
{
	m_signal = signal;
}

Wave* Wave::Demodulate()
{
	return m_signal;
}

const Wave* Wave::Demodulate() const
{
	return m_signal;
}

Wave* Wave::operator%(Wave* signal)
{
	return Modulate(signal);
}

Wave* Wave::operator%()
{
	return Demodulate();
}

const Wave* Wave::operator%() const
{
	return Demodulate();
}

Wave* Wave::operator+(const Wave* other)
{
	return *this->Clone() += other;
}

Wave* Wave::operator-(const Wave* other)
{
	return *this->Clone() -= other;
}

void Wave::operator+=(const Wave* other)
{
	//nop
}

void Wave::operator-=(const Wave* other)
{
	//nop
}

Properties Wave::GetProperties() const
{
	Properties ret;
	return ret;
}

/*static*/ Properties Wave::GetResonanceBetween(
	const Wave* wave1,
	const Wave* wave2)
{
	Waves waves;
	waves.push_back(wave1);
	waves.push_back(wave2);
	return GetResonanceBetween(waves);
}

/*static*/ Properties Wave::GetResonanceBetween(Waves waves)
{
	Properties overlap;
	BIO_SANITIZE(waves.size() && waves[0],,return overlap);
	overlap = waves[0]->GetProperties();
	BIO_SANITIZE_AT_SAFETY_LEVEL_2(waves.size()>1,,return overlap);
	std::vector<Property::iterator> remBuffer;
	for (
		Waves::const_iterator wav = waves.begin()++;
		wav != waves.end();
		++wav)
	{
		BIO_SANITIZE(*wav,,continue);
		Properties wavProperties = (*wav)->GetProperties();
		remBuffer.clear();
		for (Properties::iterator prp = overlap.begin(); prp != overlap.end(); ++prp)
		{
			if (std::find(wavProperties.begin(), wavProperties.end(), *prp) == wavProperties.end())
			{
				remBuffer.push_back(prp)
			}
		}
		for (std::vector<Property::iterator>::iterator rem = remBuffer.begin(); rem != remBuffer.end(); ++rem)
		{
			overlap.erase(rem);
		}
	}
	return overlap;
}

};

} //physical namespace
} //bio namespace