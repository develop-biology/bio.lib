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

/**
 * Singleton interface makes the constructor private so that there is only one instance ever created, which is by Instance() <br />
 * we also override default copy constructor and assignment operator so that nobody can make a copy of the singleton  (otherwise it wouldn't be a singleton). We don't define them, so these methods will give a link error if used. <br />
 * @param className the name of the new singleton
 * @param baseClass the name of the class the new singleton will inherit from (i.e. what to make into a singleton).
*/
#define BIO_SINGLETON(className, baseClass)                                    \
class className :                                                              \
    public baseClass,                                                          \
	virtual public ThreadSafe                                                  \
{                                                                              \
public:                                                                        \
    static className& Instance()                                               \
    {                                                                          \
        static className instance;                                             \
        return instance;                                                       \
    }                                                                          \
private:                                                                       \
    className()                                                                \
    {                                                                          \
    }                                                                          \
    className(className const &);                                              \
    void operator=(className const &);                                         \
};
