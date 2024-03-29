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
#include "bio/chemical/macro/Macros.h"
#include "bio/chemical/common/Class.h"
#include "bio/physical/common/Filters.h"
#include "bio/common/container/Arrangement.h"
#include <vector>
#include <algorithm>

namespace bio {
namespace chemical {

/**
 * UnorderedMotif classes have Content classes stored within them. <br />
 * They are simple containers. <br />
 */
template < typename CONTENT_TYPE >
class UnorderedMotif :
	public chemical::Class< UnorderedMotif< CONTENT_TYPE > >,
	public AbstractMotif
{
public:

	typedef Arrangement< CONTENT_TYPE > Contents;

	/**
	 * Ensure virtual methods point to Class implementations. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(chemical, UnorderedMotif< CONTENT_TYPE >)

	/**
	 *
	 */
	UnorderedMotif()
		:
		chemical::Class< UnorderedMotif< CONTENT_TYPE > >(this) //TODO: Define Symmetry.
	{
		this->mContents = new Contents(4);
	}

	/**
	 * @param contents
	 */
	UnorderedMotif(const Contents* contents)
		:
		chemical::Class< UnorderedMotif< CONTENT_TYPE > >(this) //TODO: Define Symmetry.
	{
		this->mContents = new Contents(*contents);
	}

	/**
	 * @param toCopy
	 */
	UnorderedMotif(const UnorderedMotif< CONTENT_TYPE >* toCopy)
		:
		chemical::Class< UnorderedMotif< CONTENT_TYPE > >(this) //TODO: Define Symmetry.
	{
		this->mContents = new Contents(*toCopy->mContents);
	}

	/**
	 *
	 */
	virtual ~UnorderedMotif()
	{
		//NOTE: Children are responsible for clearing mContents (e.g. through ClearImplementation()).
	}

	/**
	 * Clears the contents of *this. <br />
	 */
	virtual void ClearImplementation()
	{
		this->mContents->Clear();
	}

	/**
	 * Implementation for counting Contents. <br />
	 * @return the number of Contents in *this.
	 */
	virtual Index GetCountImplementation() const
	{
		return this->mContents->GetNumberOfElements();
	}

	/**
	 * Adds content to *this. <br />
	 * @param content
	 * @return the added content or 0.
	 */
	virtual CONTENT_TYPE AddImplementation(const CONTENT_TYPE content)
	{
		CONTENT_TYPE ret = this->mContents->Access(this->mContents->Add(content));
		return ret;
	}

	/**
	 * Removes content from *this and deletes it. <br />
	 * @param content
	 */
	virtual CONTENT_TYPE RemoveImplementation(const CONTENT_TYPE content)
	{
		Index toErase = this->mContents->SeekTo(content);
		CONTENT_TYPE ret = this->mContents->Access(toErase);
		this->mContents->Erase(toErase);
		return ret;
	}

	/**
	 * Check for content. <br />
	 * @param content
	 * @return whether or not the given content exists in *this
	 */
	virtual bool HasImplementation(const CONTENT_TYPE content) const
	{
		return this->mContents->Has(content);
	}

	/**
	 * Copy the contents of another container into *this. <br />
	 * @param other
	 */
	virtual void ImportImplementation(const UnorderedMotif< CONTENT_TYPE >* other)
	{
		BIO_SANITIZE(other, , return);

		this->mContents->Import(other->GetAllImplementation());
	}

	/**
	 * Gives the number of matching contents between *this & other. <br />
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
			!otr.IsBeforeBeginning();
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
	 * Check for all contents <br />
	 * @param contents
	 * @return whether or not the given contents exists in *this
	 */
	virtual bool HasAllImplementation(const Container* contents) const
	{
		BIO_SANITIZE(contents, ,
			return false);
		return this->GetNumMatchingImplementation(contents) == contents->GetNumberOfElements();
	}

	/**
	 * Get the Contents of *this as a String. <br />
	 * @param separator e.g. ", ", the default, or just " ".
	 * @return the Contents of *this as a string.
	 */
	virtual String GetStringFromImplementation(String separator = ", ")
	{
		std::string ret = "";
		SmartIterator cnt = this->mContents->Begin();
		while (true)
		{
			ret += String::From< CONTENT_TYPE >(*cnt).AsStdString();
			++cnt;
			if (cnt.IsAfterEnd())
			{
				break;
			}
			ret += separator.AsStdString();
		}
		return ret;
	}

	virtual void LogImplementation(const LogLevel& level = log_level::Info(), const Filter& filter = FilterPerspective::InvalidId())
	{
		if (filter == FilterPerspective::InvalidId())
		{
			this->AbstractMotif::LogImplementation(level, this->GetFilter());
		}
		else
		{
			this->AbstractMotif::LogImplementation(level, filter);
		}
	}
};

} //chemical namespace
} //bio namespace
