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

#include "bio/chemical/macros/Macros.h"
#include "bio/chemical/common/Class.h"
#include "bio/physical/common/Filters.h"
#include "bio/physical/arrangement/Arrangement.h"
#include "StructureInterface.h"
#include "AbstractStructure.h"
#include <vector>
#include <algorithm>

namespace bio {
namespace chemical {

/**
 * StructuralComponent classes have Content classes stored within them.
 * They are simple containers.
 */
template < typename CONTENT_TYPE >
class StructuralComponent :
	virtual public StructureInterface,
	public chemical::Class< StructuralComponent< CONTENT_TYPE > >,
	public AbstractStructure
{
public:

	typedef physical::Arrangement Contents;

	/**
	 * Ensure virtual methods point to Class implementations.
	 */
	BIO_DISAMBIGUATE_CLASS_METHODS(chemical, StructuralComponent< CONTENT_TYPE >)

	/**
	 *
	 */
	StructuralComponent()
		:
		chemical::Class< StructuralComponent< CONTENT_TYPE > >(this), //TODO: Define Symmetry.
		m_contents(new Contents(4))
	{
	}

	/**
	 * @param contents
	 */
	StructuralComponent(Contents* contents)
		:
		chemical::Class< StructuralComponent< CONTENT_TYPE > >(this), //TODO: Define Symmetry.
		m_contents(new Contents(*contents))
	{
	}

	/**
	 * @param toCopy
	 */
	StructuralComponent(const StructuralComponent< CONTENT_TYPE >* toCopy) :
		chemical::Class< StructuralComponent< CONTENT_TYPE > >(this), //TODO: Define Symmetry.
		m_contents(new Contents(*toCopy->m_contents))
	{
	}

	/**
	 *
	 */
	virtual ~StructuralComponent()
	{
		//NOTE: Children are responsible for clearing m_contents (e.g. through ClearImplementation().
	}

	/**
	 * Implementation for accessing all Contents.
	 * @return all Contents in *this.
	 */
	virtual Contents* GetAllImplementation()
	{
		return this->m_contents;
	}

	/**
	 * Const implementation for accessing all Contents.
	 * @return all Contents in *this.
	 */
	virtual const Contents* GetAllImplementation() const
	{
		return this->m_contents;
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
		CONTENT_TYPE retPtr = this->m_contents->Access(this->m_contents->Add(content));
		BIO_SANITIZE(retPtr, ,
			return 0);
		return *retPtr;
	}

	/**
	 * Removes content from *this and deletes it.
	 * @param content
	 */
	virtual CONTENT_TYPE RemoveImplementation(const CONTENT_TYPE content)
	{
		typename Index toErase = this->m_contents->SeekTo(content);
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
	virtual void ImportImplementation(const StructuralComponentImplementation< CONTENT_TYPE >* other)
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
	virtual unsigned int GetNumMatchingImplementation(const StructuralComponentImplementation< CONTENT_TYPE >* other) const
	{
		BIO_SANITIZE(other, ,
			return 0);

		unsigned int ret = 0;
		for (
			SmartIterator otr = other->End();
			; !otr->IsAtBeginning();
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
	virtual bool HasAllImplementation(const Contents* contents) const
	{
		BIO_SANITIZE(contents, ,
			return false);
		return this->GetNumMatchingImplementation(contents) == contents->GetUsableSize();
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
			if (cnt->IsAtEnd())
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
