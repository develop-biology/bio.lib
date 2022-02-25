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

#include "bio/common/Random.h"

#if BIO_CPP_VERSION >= 11
#include <random>
#endif

namespace bio {

#if BIO_CPP_VERSION >= 11
static std::random_device rd;
static std::mt19937 gen(rd());
#endif

float random::NormalFloat(
	float mean,
	float standardDeviation)
{
	#if BIO_CPP_VERSION >= 11
	std::normal_distribution<float> dist
		{
			mean,
			standardDeviation
		};
	return dist(gen);
	#else
	return 0;
	#endif
}

float random::UniformFloat(
	float min,
	float max)
{
	#if BIO_CPP_VERSION >= 11
	std::uniform_real_distribution<float> dist(
		min,
		max
	);
	return dist(gen);
	#else
	return 0;
	#endif
}

} //bio namespace
