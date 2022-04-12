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

#include "AbstractMotif.h"
#include "bio/chemical/macros/Macros.h"
#include "bio/chemical/common/Class.h"
#include "bio/physical/common/Filters.h"
#include "bio/physical/arrangement/Arrangement.h"
#include <vector>
#include <algorithm>

namespace bio {
namespace chemical {

/**
 * UnorderedMotif classes have Content classes stored within them.
 * They are simple containers.
 */
template < typename CONTENT_TYPE >
class UnorderedMotif :
	public chemical::Class< UnorderedMotif< CONTENT_TYPE > >,
	public AbstractMotif
{
public:

	typedef physical::Arrangement< CONTENT_TYPE > Contents;

	/**
	 * Ensure virtual methods point to Class implementations.
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(chemical, UnorderedMotif< CONTENT_TYPE >)

	/**
	 *
	 */
	UnorderedMotif()
		:
		chemical::Class< UnorderedMotif< CONTENT_TYPE > >(this) //TODO: Define Symmetry.
	{
		this->m_contents = new Contents(4);
	}

	/**
	 * @param contents
	 */
	UnorderedMotif(const Contents* contents)
		:
		chemical::Class< UnorderedMotif< CONTENT_TYPE > >(this) //TODO: Define Symmetry.
	{
		this->m_contents = new Contents(*contents);
	}

	/**
	 * @param toCopy
	 */
	UnorderedMotif(const UnorderedMotif< CONTENT_TYPE >* toCopy) :
		chemical::Class< UnorderedMotif< CONTENT_TYPE > >(this) //TODO: Define Symmetry.
	{
		this->m_contents = new Contents(*toCopy->m_contents);
	}

	/**
	 *
	 */
	virtual ~UnorderedMotif()
	{
		//NOTE: Children are responsible for clearing m_contents (e.g. through ClearImplementation().
	}

	/**
	 * Clears the contents of *this.
	 */
	virtual void ClearImplementation()
	{
		this->m_contents->Clear();
	}

	/**
	 * Implementation for counting Contents.
	 * @return the number of Contents in *this.
	 */
	virtual unsigned long GetCountImplementation() const
	{
		return this->m_contents->GetNumberOfElements();
	}

	/**
	 * Adds content to *this.
	 * @param content
	 * @return the added content or 0.
	 */
	virtual CONTENT_TYPE AddImplementation(const CONTENT_TYPE content)
	{
		CONTENT_TYPE ret = this->m_contents->Access(this->m_contents->Add(content));
		return ret;
	}

	/**
	 * Removes content from *this and deletes it.
	 * @param content
	 */
	virtual CONTENT_TYPE RemoveImplementation(const CONTENT_TYPE content)
	{
		Index toErase = this->m_contents->SeekTo(content);
		CONTENT_TYPE ret = this->m_contents->Access(toErase);
		this->m_contents->Erase(toErase);
		return ret;
	}

	/**
	 * Check for content.
	 * @param content
	 * @return whether or not the given content exists in *this
	 */
	virtual bool HasImplementation(const CONTENT_TYPE content) const
	{
		return this->m_contents->Has(content);
	}

	/**
	 * Copy the contents of another container into *this.
	 * @param other
	 */
	virtual void ImportImplementation(const UnorderedMotif< CONTENT_TYPE >* other)
	{
		BIO_SANITIZE(other, ,
			return);

		this->m_contents->Import(other->GetAllImplementation());
	}

	/**
	 * Gives the number of matching contents between *this & other.
	 * @param other
	 * @return quantity overlap with other.
	 */
	virtual unsigned int GetNumMatchingImplementation(const Container* other) const
	{
		BIO_SANITIZE(other, ,
			return 0);

		unsigned int ret = 0;
		for (
			SmartIterator otr = other->End();
			!otr.IsAtBeginning();
			--otr
			)
		{
			if (this->HasImplementation(*otr))
			{
				++ret;
			}
		}
		return ret;
	}

	/**
	 * Check for all contents
	 * @param content
	 * @return whether or not the given contents exists in *this
	 */
	virtual bool HasAllImplementation(const Container* contents) const
	{
		BIO_SANITIZE(contents, ,
			return false);
		return this->GetNumMatchingImplementation(contents) == contents->GetNumberOfElements();
	}

	/**
	 * Get the Contents of *this as a string.
	 * @param separator e.g. ", ", the default, or just " ".
	 * @return the Contents of *this as a string.
	 */
	virtual std::string GetStringFromImplementation(std::string separator = ", ")
	{
		std::string ret = "";
		SmartIterator cnt = this->m_contents->Begin();
		while (true)
		{
			ret += string::From< CONTENT_TYPE >(*cnt);
			++cnt;
			if (cnt.IsAtEnd())
			{
				break;
			}
			ret += separator;
		}
		return ret;
	}
};

} //chemical namespace
} //bio namespace
