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

#include "LinearMotif.h"
#include "bio/chemical/EnvironmentDependent.h"

namespace bio {
namespace chemical {

/**
 * DependentMotifs extend LinearMotifs by adding support for EnvironmentDependent objects. <br />
 * When an object is added to a DependentMotif, the object's Environment is Set as that stored in *this. <br />
 */
template < typename CONTENT_TYPE, typename ENVIRONMENT >
class DependentMotif :
	public chemical::Class< DependentMotif< CONTENT_TYPE, ENVIRONMENT > >,
	public LinearMotif< CONTENT_TYPE >,
	public EnvironmentDependent< ENVIRONMENT >
{
public:
	/**
	 * Ensure virtual methods point to Class implementations. <br />
	 * We want to define our own Attenuate & Disattenuate, so we have to ignore the optional class methods for the chemical class. <br />
	 */
	BIO_DISAMBIGUATE_REQUIRED_CLASS_METHODS(chemical, BIO_SINGLE_ARG(DependentMotif< CONTENT_TYPE, ENVIRONMENT >))
	BIO_DISAMBIGUATE_OPTIONAL_CLASS_METHODS(physical, BIO_SINGLE_ARG(DependentMotif< CONTENT_TYPE, ENVIRONMENT >))

	/**
	 * @param environment
	 * @param perspective
	 */
	explicit DependentMotif(
		ENVIRONMENT* environment = NULL,
		physical::Perspective< Id >* perspective = NULL
	)
		:
		chemical::Class< DependentMotif< CONTENT_TYPE, ENVIRONMENT > >(this),
		EnvironmentDependent< ENVIRONMENT >(environment),
		LinearMotif< CONTENT_TYPE >(perspective)
	{

	}

	/**
	 * @param contents
	 * @param environment
	 * @param perspective
	 */
	explicit DependentMotif(
		const LinearMotif< CONTENT_TYPE >::Contents* contents,
		ENVIRONMENT* environment = NULL,
		physical::Perspective< Id >* perspective = NULL
	)
		:
		chemical::Class< DependentMotif< CONTENT_TYPE, ENVIRONMENT > >(this),
		EnvironmentDependent< ENVIRONMENT >(environment),
		LinearMotif< CONTENT_TYPE >(contents, perspective)
	{

	}

	/**
	 * Copying a DependentMotif will Clone all contents in toCopy into *this. <br />
	 * Keep in mind that dtor will delete the contents of *this. <br />
	 * @param toCopy
	 */
	DependentMotif(const DependentMotif< CONTENT_TYPE, ENVIRONMENT >& toCopy)
		:
		chemical::Class< DependentMotif< CONTENT_TYPE, ENVIRONMENT > >(this),
		EnvironmentDependent< ENVIRONMENT >(toCopy),
		LinearMotif< CONTENT_TYPE >(toCopy)
	{

	}

	/**
	 *
	 */
	virtual ~DependentMotif()
	{

	}

	/**
	 * Sets the Environment of *this and all mContents. <br />
	 * Don't let the environment go out of scope or be deleted before *this! <br />
	 * @param environment
	 */
	virtual void SetEnvironment(ENVIRONMENT* environment)
	{
		this->EnvironmentDependent< ENVIRONMENT >::SetEnvironment(environment);
		for (
			SmartIterator cnt = this->mContents->End();
			!cnt.IsBeforeBeginning();
			--cnt
			)
		{
			cnt.template As< CONTENT_TYPE >()->SetEnvironment(environment);
		}
	}


	/**
	 * Adds content to *this. <br />
	 * Added objects are "owned" (not physical::Linear::IsShared()) and will be deleted with *this. <br />
	 * @param content
	 * @return t or NULL.
	 */
	virtual CONTENT_TYPE AddImplementation(CONTENT_TYPE content)
	{
		BIO_SANITIZE(content,,return NULL)
		content->SetEnvironment(this);
		return LinearMotif< CONTENT_TYPE >::AddImplementation(content);
	}

	/**
	 * Adds a Content in *this at the indicated position. <br />
	 * Multiple contents of the same id will cause the previously existing Content to be removed. <br />
	 *
	 * NOTE: THIS DESTROYS INDEX INTEGRITY. <br />
	 * Indices will be rearranged to accommodate the insertion, making any cached Index invalid. <br />
	 *
	 * @param toAdd what to add. IMPORTANT: This must not already be in a DependentMotif (i.e. create a clone() before adding it to another destination).
	 * @param position determines where in *this the Content is added.
	 * @param optionalPositionArg If a position is specified, the optionalPositionArg is the id of the Content referenced (e.g. BEFORE, MyContentId()).
	 * @param transferSubContents allows all of the Contents within a conflicting Content to be copied into the new Content, before the conflicting Content is deleted (similar to renaming an upper directory while preserving it's contents).
	 * @return Status of addition (e.g. success or failure).
	 */
	virtual Code InsertImplementation(
		CONTENT_TYPE toAdd,
		const Position position = BOTTOM,
		const Id optionalPositionArg = 0, //i.e. invalid.
		const bool transferSubContents = false
	)
	{
		BIO_SANITIZE(toAdd, , return code::MissingArgument1())
		toAdd->SetEnvironment(this->GetEnvironment());
		return chemical::LinearMotif< CONTENT_TYPE >::InsertImplementation(
			toAdd,
			position,
			optionalPositionArg,
			transferSubContents);
	}
};
} //chemical namespace
} //bio namespace
