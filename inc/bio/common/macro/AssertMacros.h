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

#include <assert.h>
#include "LanguageMacros.h"

/**
 * Simple wrapper around assert. May get more complicated if platform specific overrides become necessary.  <br />
 */

#define BIO_ASSERT(condition)    assert(condition);

#if BIO_CPP_VERSION < 17
#define BIO_STATIC_ASSERT(condition)
#else
#define BIO_STATIC_ASSERT(condition) static_assert(condition);
#endif
