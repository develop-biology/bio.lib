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

#include "bio/common/macros/Macros.h"
#include "bio/common/String.h"
#include "bio/common/Cast.h"
#include "bio/physical/common/Codes.h"
#include "AbstractStructure.h"
#include <vector>
#include <algorithm>

namespace bio {
namespace chemical {

template < typename CONTENT_TYPE >
class StructuralComponentImplementation :
	public AbstractStructure
{
public:

	typedef std::vector< CONTENT_TYPE > Contents;

	/**
	 *
	 */
	StructuralComponentImplementation()
	{
	}

	/**
	 * @param contents
	 */
	StructuralComponentImplementation(const Contents& contents)
		:
		m_contents(contents)
	{
	}

	/**
	 *
	 */
	virtual ~StructuralComponentImplementation()
	{
		//NOTE: Children are responsible for deleting m_contents (e.g. through ClearImplementation().
	}

	/**
	 * Find some content!
	 * @param content
	 * @param contents
	 * @return an Contents::iterator for the given content
	 */
	static typename Contents::iterator Find(
		CONTENT_TYPE content,
		Contents* contents
	)
	{
		return std::find(
			contents->begin(),
			contents->end(),
			content
		);
	}

	/**
	 * Find some content!
	 * @param content
	 * @param contents
	 * @return an Contents::const_iterator for the given content
	 */
	static typename Contents::const_iterator Find(
		CONTENT_TYPE content,
		const Contents* contents
	)
	{
		return std::find(
			contents->begin(),
			contents->end(),
			content
		);
	}

	/**
	 * Does the given content exist in the contents?
	 * @param content
	 * @param contents
	 * @return whether or not content can be found in the contents.
	 */
	static bool DoesExist(
		CONTENT_TYPE content,
		const Contents* contents
	)
	{
		return Find(
			content,
			contents
		) != contents->end();
	}

	/**
	 * Adds content to the destination iff it does not already exist.
	 * @param content
	 * @param destination
	 * @return a pointer to the added content or 0.
	 */
	static CONTENT_TYPE AddTo(
		CONTENT_TYPE content,
		Contents* destination
	)
	{
		BIO_SANITIZE_AT_SAFETY_LEVEL_2(DoesExist(
			content,
			destination
		), ,
			return 0);
		destination->push_back(content);
		return *(destination->end() - 1);
	}

	/**
	 * Removes toRemove from removeFrom.
	 * @param toRemove what to remove.
	 * @param removeFrom where to remove toRemove from.
	 * @return a pointer to the removed content or 0.
	 */
	static CONTENT_TYPE RemoveFrom(
		CONTENT_TYPE toRemove,
		Contents* removeFrom
	)
	{
		BIO_SANITIZE(removeFrom, ,
			return 0);

		typename Contents::iterator cnt = std::find(
			removeFrom->begin(),
			removeFrom->end(),
			toRemove
		);
		BIO_SANITIZE_AT_SAFETY_LEVEL_2(cnt != removeFrom->end(), ,
			return 0);
		CONTENT_TYPE ret = *cnt;
		removeFrom->erase(cnt);
		return ret;
	}


	/**
	 * Implementation for accessing all Contents.
	 * @return all Contents in *this.
	 */
	virtual Contents* GetAllImplementation()
	{
		return &this->m_contents;
	}

	/**
	 * Const implementation for accessing all Contents.
	 * @return all Contents in *this.
	 */
	virtual const Contents* GetAllImplementation() const
	{
		return &this->m_contents;
	}

	/**
	 * Clears the contents of *this.
	 */
	virtual void ClearImplementation()
	{
		this->m_contents.clear();
	}

	/**
	 * Implementation for counting Contents.
	 * @return the number of Contents in *this.
	 */
	virtual unsigned long GetCountImplementation() const
	{
		return this->m_contents.size();
	}

	/**
	 * Get a pointer to the content in *this
	 * @param content
	 * @return a pointer to the given content matching that within *this; 0 if no match found.
	 */
	virtual CONTENT_TYPE GetImplementation(CONTENT_TYPE content)
	{
		typename Contents::iterator ret = Find(
			content,
			&this->m_contents
		);
		BIO_SANITIZE(ret != this->m_contents.end(), ,
			return 0);
		return *ret;
	}


	/**
	 * Get a pointer to the content in *this
	 * @param content
	 * @return a pointer to the given content matching that within *this; 0 if no match found.
	 */
	virtual const CONTENT_TYPE GetImplementation(CONTENT_TYPE content) const
	{
		typename Contents::const_iterator ret = Find(
			content,
			&this->m_contents
		);
		BIO_SANITIZE(ret != this->m_contents.end(), ,
			return 0);
		return *ret;
	}

	/**
	 * Adds content to *this.
	 * @param content
	 * @return t or 0.
	 */
	virtual CONTENT_TYPE AddImplementation(CONTENT_TYPE content)
	{
		return this->AddTo(
			content,
			&m_contents
		);
	}

	/**
	 * Removes content from *this and deletes it.
	 * @param content
	 */
	virtual CONTENT_TYPE RemoveImplementation(CONTENT_TYPE content)
	{
		return this->RemoveFrom(
			content,
			&m_contents
		);
	}

	/**
	 * Check for content.
	 * @param content
	 * @return whether or not the given content exists in *this
	 */
	virtual bool HasImplementation(CONTENT_TYPE content) const
	{
		return this->DoesExist(
			content,
			&m_contents
		);
	}

	/**
	 * Copy the contents of another container into *this.
	 * @param other
	 */
	virtual void ImportImplementation(const StructuralComponentImplementation< CONTENT_TYPE >* other)
	{
		BIO_SANITIZE_AT_SAFETY_LEVEL_2(other, ,
			return);

		this->m_contents.insert(
			this->m_contents.end(),
			other->m_contents.begin(),
			other->m_contents.end());
	}

	/**
	 * Gives the number of matching contents between *this & other.
	 * @param other
	 * @return quantity overlap with other.
	 */
	virtual unsigned int GetNumMatchingImplementation(const StructuralComponentImplementation< CONTENT_TYPE >& other) const
	{
		unsigned int ret = 0;
		for (
			typename Contents::const_iterator cnt = other.m_contents.begin();
			cnt != other.m_contents.end();
			++cnt
			)
		{
			if (this->HasImplementation(*cnt))
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
	virtual bool HasAllImplementation(const Contents& contents) const
	{
		return this->GetNumMatchingImplementation(contents) == contents.size();
	}

	/**
	 * Get the Contents of *this as a string.
	 * @param separator e.g. ", ", the default, or just " ".
	 * @return the Contents of *this as a string.
	 */
	virtual std::string GetStringFromImplementation(std::string separator = ", ")
	{
		std::string ret = "";
		for (
			typename Contents::const_iterator cnt = m_contents.begin();
			cnt != m_contents.end();
			++cnt
			)
		{
			ret += string::From< CONTENT_TYPE >(*cnt);
			if (cnt != m_contents.end() - 1)
			{
				ret += separator;
			}
		}
		return ret;
	}

protected:
	Contents m_contents;
};

} //chemical namespace
} //bio namespace
