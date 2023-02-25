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

#include "bio/physical/Wave.h"
#include "bio/physical/common/Types.h"
#include "bio/physical/symmetry/Symmetry.h"
#include "bio/physical/common/Codes.h"
#include "bio/common/macro/Macros.h"
#include <algorithm>

namespace bio {
namespace physical {

Wave::Wave(
	Symmetry* symmetry
)
	:
	mSymmetry(symmetry),
	mSignal(NULL)
{
}

Wave::~Wave()
{
	if (mSymmetry)
	{
		delete mSymmetry;
		mSymmetry = NULL;
	}
}

Wave* Wave::Clone() const
{
	return new Wave(
		*this
	);
}

Symmetry* Wave::Spin() const
{
	return mSymmetry;
}

Code Wave::Reify(
	Symmetry* symmetry
)
{
	(*mSymmetry) = *symmetry;
	return code::Success();
}

void Wave::operator|(
	Symmetry* symmetry
)
{
	Reify(
		symmetry
	);
}

Code Wave::Attenuate(const Wave* other)
{
	return code::NotImplemented();
}

Code Wave::Disattenuate(const Wave* other)
{
	return code::NotImplemented();
}

Wave* Wave::Modulate(
	Wave* signal
)
{
	mSignal = signal;
	return this;
}

Wave* Wave::Demodulate()
{
	return mSignal;
}

const Wave* Wave::Demodulate() const
{
	return mSignal;
}

Wave* Wave::operator*(
	Wave* signal
)
{
	return Modulate(
		signal
	);
}

Wave* Wave::operator*()
{
	return Demodulate();
}

const Wave* Wave::operator*() const
{
	return Demodulate();
}

void Wave::operator+(
	const Wave* other
)
{
	Attenuate(other);
}

void Wave::operator-(
	const Wave* other
)
{
	Disattenuate(other);
}

Properties Wave::GetProperties() const
{
	Properties ret;
	return ret;
}

/*static*/ Properties Wave::GetResonanceBetween(
	const Wave* wave,
	const Properties& properties
)
{
	class TempWave :
		public Wave
	{
	public:
		TempWave(Properties p)
			:
			mProperties(p)
		{
		}

		~TempWave()
		{
		}

		virtual Properties GetProperties() const
		{
			return mProperties;
		}

		Properties mProperties;
	};

	ConstWaves waves;
	waves.Add(wave);
	TempWave twave(properties);
	ByteStream overt;
	overt.Set< Wave* >(&twave);
	waves.Add(overt);
	return GetResonanceBetween(waves);
}

/*static*/ Properties Wave::GetResonanceBetween(
	const Wave* wave1,
	const Wave* wave2
)
{
	ConstWaves waves;
	waves.Add(
		wave1
	);
	waves.Add(
		wave2
	);
	return GetResonanceBetween(
		waves
	);
}

/*static*/ Properties Wave::GetResonanceBetween(
	ConstWaves waves
)
{
	Properties overlap;
	BIO_SANITIZE(waves.Size() && waves[0], ,
		return overlap);
	overlap = waves[0].As< Wave* >()->GetProperties();
	BIO_SANITIZE_AT_SAFETY_LEVEL_1(waves.Size() > 1, ,
		return overlap);

	for (
		SmartIterator wav = waves.Begin()++;
		!wav.IsAfterEnd();
		++wav
		)
	{
		Properties wavProperties = wav.As< Wave* >()->GetProperties();
		for (
			SmartIterator prp = overlap.Begin();
			!prp.IsAfterEnd();
			++prp
			)
		{
			if (!wavProperties.Has(prp.As< Property >()))
			{
				overlap.Erase(prp);
			}
		}
	}
	return overlap;
}

} //physical namespace
} //bio namespace
