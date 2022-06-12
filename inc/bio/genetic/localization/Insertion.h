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

#include "Localization.h"

namespace bio {
namespace genetic {

/**
 * Insertions are a type of Insertion which, instead of just finding a place, add something to a place. <br />
 * This is used to move Proteins to their correct position after Transcription. <br />
 *  <br />
 * NOTE: All Sites currently have a 1 to 1 mapping with Sites and should always be the same value. <br />
 * i.e. Cast< Site >(mySiteId) == SafelyAccess<InsertionSitePerspective>()->GetIdWithoutCreation(mySiteName) <br />
 */
class Insertion :
	public physical::Class< Insertion >,
	public Localization
{
public:

	/**
	 * Ensure virtual methods point to Class implementations. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(physical, Insertion)


	/**
	 * @param whatToInsert
	 * @param site
	 * @param name
	 */
	explicit Insertion(
		chemical::Substance* whatToInsert = NULL,
		Site site = InsertionSitePerspective::InvalidId(),
		const Name& name = NULL
	);

	/**
	 *
	 */
	virtual ~Insertion();

	/**
	 * Override of Localization system. <br />
	 * This is what does the inserting. <br />
	 * Will recurse upward, following mPrevious for as long as possible. <br />
	 * @param insertIn
	 * @return a Substance somewhere within the Substance provided or NULL.
	 */
	virtual chemical::Substance* Seek(chemical::Substance* insertIn) const;

	/**
	 * Tells *this to insert toInsert in its Localization. <br />
	 * @param toInsert
	 */
	virtual void InsertThis(chemical::Substance* toInsert);

	/**
	 * @return what *this will try to insert in its Localization
	 */
	virtual chemical::Substance* GetWhatWillBeInserted();

	/**
	 * Set mSite. <br />
	 * @param site
	 */
	virtual void SetSite(Site site);

protected:
	chemical::Substance* mToInsert;
};

} //genetic namespace
} //bio namespace
