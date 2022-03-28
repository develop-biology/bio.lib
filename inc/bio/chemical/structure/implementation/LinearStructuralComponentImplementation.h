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

#include <vector>
#include "bio/chemical/arrangement/Linear.h"
#include "StructuralComponentImplementation.h"
#include "StructureInterface.h"
#include "bio/physical/common/Codes.h"
#include "bio/physical/common/Filters.h"
#include "bio/physical/Identifiable.h"
#include "bio/log/Engine.h"
#include "bio/log/common/Levels.h"
#include "bio/chemical/common/Codes.h"
#include "bio/chemical/common/Types.h"
#include "bio/chemical/reaction/Excitation.h"

namespace bio {
namespace chemical {

/**
 * Basic implementation of methods for a LinearStructuralComponent
 *
 * Using LinearStructuralComponents requires the storage of chemical::Class*, so CONTENT_TYPE must derive from chemical::Class (or at minimum physical::Identifiable<StandardDimension>)
 * For an example, see chemical::Symmetry.
 * For more information, see Linear.h.
 *
 * NOTE: we must inherit from StructuralComponent and NOT its implementation so that we only have 1 m_contents per Component.
 * 
 * @tparam CONTENT_TYPE
 */
template < typename CONTENT_TYPE >
class LinearStructuralComponentImplementation :
	public StructuralComponent< CONTENT_TYPE >
{
public:
	typedef StandardDimension Dimension;
	typedef std::vector< Dimension > Dimensions;

	/**
	 *
	 */
	LinearStructuralComponentImplementation()
	{
	}

	/**
	 *
	 */
	virtual ~LinearStructuralComponentImplementation()
	{
	}

	/**
	 * Get an iterator for a Content of the given id from contents.
	 * @param id
	 * @param contents
	 * @return Contents::iterator
	 */
	static typename StructuralComponentImplementation< Linear >::Contents::iterator GetIteratorById(
		StandardDimension id,
		typename StructuralComponentImplementation< Linear >::Contents* contents
	)
	{
		typename StructuralComponentImplementation< Linear >::Contents::iterator ret = contents->begin();
		for (
			; ret != contents->end();
			++ret
			)
		{
			if (*(*ret) == id)
			{
				return ret;
			}
		}
		return ret;
	}

	/**
	 * Get a const iterator for a Content of the given id from contents.
	 * @param id
	 * @param contents
	 * @return Contents::const_iterator
	 */
	static typename StructuralComponentImplementation< Linear >::Contents::const_iterator GetIteratorById(
		StandardDimension id,
		const typename StructuralComponentImplementation< Linear >::Contents* contents
	)
	{
		typename StructuralComponentImplementation< Linear >::Contents::const_iterator ret = contents->begin();
		for (
			; ret != contents->end();
			++ret
			)
		{
			if (*(*ret) == id)
			{
				return ret;
			}
		}
		return ret;
	}


	/**
	 * Searches for a Content of the given id.
	 * @param searchIn contents to search through.
	 * @param contentId id to find.
	 * @return a pointer to a CONTENT_TYPE of id if one is found within searchIn, NULL if one is not found.
	 */
	static CONTENT_TYPE FindByIdIn(
		typename StructuralComponentImplementation< Linear >::Contents* searchIn,
		const StandardDimension contentId
	)
	{
		typename StructuralComponentImplementation< Linear >::Contents::iterator itt = GetIteratorFrom(
			searchIn,
			contentId
		);
		if (itt != searchIn->end())
		{
			//BIO_LOG_DEBUG("Found %s", name);
			return *itt;
		}
		//BIO_LOG_DEBUG("Could not find %s", name);
		return NULL;
	}

	/**
	 * Searches for a Content of the given id.
	 * @param searchIn contents to search through.
	 * @param contentId id to find.
	 * @return a pointer to a CONTENT_TYPE of id if one is found within searchIn, NULL if one is not found.
	 */
	static const CONTENT_TYPE FindByIdIn(
		const typename StructuralComponentImplementation< Linear >::Contents* searchIn,
		const StandardDimension contentId
	)
	{
		typename StructuralComponentImplementation< Linear >::Contents::const_iterator itt = GetIteratorFrom(
			searchIn,
			contentId
		);
		if (itt != searchIn->end())
		{
			//BIO_LOG_DEBUG("Found %s", name);
			return *itt;
		}
		//BIO_LOG_DEBUG("Could not find %s", name);
		return NULL;
	}

	/**
	 * Searches for a Content of the given name.
	 * @param perspective where to get the id for the given name.
	 * @param searchIn contents to search through.
	 * @param contentId id to find.
	 * @return a pointer to a CONTENT_TYPE of id if one is found within searchIn, NULL if one is not found.
	 */
	static CONTENT_TYPE FindByNameIn(
		const physical::Perspective< StandardDimension >* perspective,
		typename StructuralComponentImplementation< Linear >::Contents* searchIn,
		Name contentName
	)
	{
		BIO_SANITIZE(perspective && searchIn, ,
			return NULL);
		return FindByIdIn(
			searchIn,
			perspective->GetIdWithoutCreation(contentName)
		);
	}

	/**
	 * Searches for a Content of the given name.
	 * @param perspective where to get the id for the given name.
	 * @param searchIn contents to search through.
	 * @param contentId id to find.
	 * @return a pointer to a CONTENT_TYPE of id if one is found within searchIn, NULL if one is not found.
	 */
	static const CONTENT_TYPE FindByNameIn(
		const physical::Perspective< StandardDimension >* perspective,
		const typename StructuralComponentImplementation< Linear >::Contents* searchIn,
		Name contentName
	)
	{
		BIO_SANITIZE(perspective && searchIn, ,
			return NULL);
		return FindByIdIn(
			searchIn,
			perspective->GetIdWithoutCreation(contentName)
		);
	}

	/**
	 * Same as FindContent, but returns an iterator of searchIn, rather than the CONTENT_TYPE
	 * Depth-first search.
	 * @param searchIn contents to search through.
	 * @param contentId id to find.
	 * @return iterator of searchIn matching contentId.
	 */
	static typename StructuralComponentImplementation< Linear >::Contents::iterator GetIteratorFrom(
		typename StructuralComponentImplementation< Linear >::Contents* searchIn,
		const StandardDimension contentId
	)
	{
		BIO_SANITIZE(searchIn, ,
			return searchIn->end());

		typename StructuralComponentImplementation< Linear >::Contents::iterator ret = searchIn->end();
		for (
			typename StructuralComponentImplementation< Linear >::Contents::iterator cnt = searchIn->begin();
			cnt != searchIn->end();
			++cnt
			)
		{
			if (*(*cnt) == contentId)
			{
				ret = cnt;
				break;
			}
		}
		return ret;
	}

	/**
	 * Same as FindContent, but returns an iterator of searchIn, rather than the CONTENT_TYPE
	 * Depth-first search.
	 * @param searchIn contents to search through.
	 * @param contentId id to find.
	 * @return iterator of searchIn matching contentId.
	 */
	static typename StructuralComponentImplementation< Linear >::Contents::const_iterator GetIteratorFrom(
		const typename StructuralComponentImplementation< Linear >::Contents* searchIn,
		const StandardDimension contentId
	)
	{
		BIO_SANITIZE(searchIn, ,
			return searchIn->end());

		typename StructuralComponentImplementation< Linear >::Contents::const_iterator ret = searchIn->end();
		for (
			typename StructuralComponentImplementation< Linear >::Contents::const_iterator cnt = searchIn->begin();
			cnt != searchIn->end();
			++cnt
			)
		{
			if (*(*cnt) == contentId)
			{
				ret = cnt;
				break;
			}
		}
		return ret;
	}

	/**
	 * Turns a vector into a string.
	 * @param contents
	 * @return a string of content names, each separated by a comma
	 */
	static std::string ConvertToString(const typename StructuralComponentImplementation< Linear >::Contents& contents)
	{
		std::string ret = "";
		for (
			typename StructuralComponentImplementation< Linear >::Contents::const_iterator cnt = contents.begin();
			cnt != contents.end();
			++cnt
			)
		{
			ret += (*cnt)->GetName();
			if (cnt != contents.end() - 1)
			{
				ret += ", ";
			}
		}

		if (!ret.size())
		{
			ret += "NO_CHILDREN";
		}

		return ret;
	}

	/**
	 * Adds a Content in the destination at the indicated position.
	 * Multiple contents of the same id will cause the previously existing Content to be removed.
	 * @param perspective used for name to id conversions (e.g. with the optionalPositionArg).
	 * @param contentPtr what to add. IMPORTANT: This must not already be in a LinearStructuralComponent (i.e. create a clone() before adding it to another destination).
	 * @param destination what to add to.
	 * @param position determines where in destination the Content is added.
	 * @param optionalPositionArg If a position is specified, the optionalPositionArg is the id of the Content referenced (e.g. BEFORE, MyContentId()).
	 * @param transferSubContents allows all of the Contents within a conflicting Content to be copied into the new Content, before the conflicting Content is deleted (similar to renaming an upper directory while preserving it's contents).
	 * @param logger what to use to print messages (since this is static).
	 * @return Status of addition (e.g. success or failure).
	 */
	static Code Insert(
		physical::Perspective< StandardDimension >* perspective,
		CONTENT_TYPE content,
		typename StructuralComponentImplementation< Linear >::Contents* destination,
		const Position position = BOTTOM,
		const StandardDimension optionalPositionArg = CONTENT_TYPE::Perspective::InvalidId(),
		const bool transferSubContents = false,
		log::Engine* logger = NULL
	)
	{
		if (!content)
		{
			#ifndef BIO_LOG_DISABLE_WARN
			if (logger)
			{
				logger->Log(
					filter::Default(),
					log::level::Warn(),
					"Attempt to add null content to parent"
				);
			}
			#endif
			return code::MissingArgument1();
		}

		if (!destination)
		{
			//We know that content is valid at this point (or at least we hope)
			#ifndef BIO_LOG_DISABLE_WARN
			if (logger)
			{
				logger->Log(
					filter::Default(),
					log::level::Warn(),
					"No destination given to add content %s to",
					content->GetName());
			}
			#endif
			return code::MissingArgument2();
		}

		Code ret = code::Success();

		StructuralComponentImplementation< Linear >::Contents::iterator toReplace = destination->end();

		//Remove conflicts
		for (
			typename StructuralComponentImplementation< Linear >::Contents::iterator cnt = destination->begin();
			cnt != destination->end();
			++cnt
			)
		{
			if (**cnt == content->GetId())
			{
				#if 0
				if (logger)
				{
					logger->Log(filter::Default(), log::level::Debug(), "%s (%p) has %u contents", (*cnt)->GetName(), *cnt, (*cnt)->template GetAll<CONTENT_TYPE>()->size());
				}
				#endif

				//Not an error, but potentially worth noting.
				ret = code::SuccessfullyReplaced();

				toReplace = cnt;
				break; //Only find 1 conflict, as no others should exist.
			}
		}

		Linear addition = Linear(CloneAndCast< CONTENT_TYPE >(content));
//		BIO_SANITIZE(addition, ,return code::GeneralFailure());

		if (toReplace != destination->end())
		{
			if (transferSubContents)
			{
				//NOTE: THIS REMOVES ANY STRUCTURAL COMPONENTS NOT EXPLICITLY IN addition
				CONTENT_TYPE casted = ChemicalCast< CONTENT_TYPE, physical::Identifiable< StandardDimension >* >(addition);
				BIO_SANITIZE(casted,,return code::GeneralFailure())
				casted->ImportAll((*toReplace)->AsWave());

				#if 0
				if (logger)
				{
					 logger->Log(filter::Default(), log::level::Debug(), "%s (%p) will replace %s", content->GetName(), content, toReplace->GetName());
				}
				#endif
			}

			destination->erase(toReplace);
		}

		#if 0
		if (logger)
		{
			logger->Log(filter::Default(), log::level::Debug(), "Adding new content, %s (%p)", addition->GetName(), (void*)addition);
		}
		#endif

		switch (position)
		{
			case TOP:
			{
				destination->insert(
					destination->begin(),
					addition
				);
				#if 0
				if (logger)
				{
					logger->Log(filter::Default(), log::level::Debug(), "Confirming: %s (%p)", addition->GetName(), (void*)addition);
				}
				#endif
				break;
			}
			case BEFORE:
			{
				for (
					typename StructuralComponentImplementation< Linear >::Contents::iterator cnt = destination->begin();
					cnt != destination->end();
					++cnt
					)
				{
					if ((*cnt)->IsId(optionalPositionArg))
					{
						destination->insert(
							cnt,
							addition
						);
						break;
					}
					if (cnt == (destination->end() - 1))
					{
						#ifndef BIO_LOG_DISABLE_WARN
						if (logger)
						{
							logger->Log(
								filter::Default(),
								log::level::Warn(),
								"Could not find content %s",
								perspective->GetNameFromId(optionalPositionArg));
						}
						#endif
					}
				}
				break;
			}
			case AFTER:
			{
				for (
					typename StructuralComponentImplementation< Linear >::Contents::iterator cnt = destination->begin();
					cnt != destination->end();
					++cnt
					)
				{
					if ((*cnt)->IsId(optionalPositionArg))
					{
						destination->insert(
							++cnt,
							addition
						);
						break;
					}
					if (cnt == (destination->end() - 1))
					{
						#ifndef BIO_LOG_DISABLE_WARN
						if (logger)
						{
							logger->Log(
								filter::Default(),
								log::level::Warn(),
								"Could not find content %s",
								perspective->GetNameFromId(optionalPositionArg));
						}
						#endif
					}
				}
				break;
			}
			case BOTTOM:
			{
				destination->push_back(addition);
				break;
			}
			default:
			{
				destination->push_back(addition);
				break;
			}
		} //switch

		return ret;
	}

	/**
	 * Implementation for inserting a Content to *this.
	 * @param toAdd
	 * @param position
	 * @param optionalPositionArg
	 * @param transferSubContents
	 * @return status of insertion
	 */
	virtual Code InsertImplementation(
		CONTENT_TYPE toAdd,
		const Position position = BOTTOM,
		const StandardDimension optionalPositionArg = CONTENT_TYPE::Perspective::InvalidId(),
		const bool transferSubContents = false
	)
	{
		return code::NotImplemented();
	}

	/**
	 * Implementation for getting by id.
	 * @param id
	 * @return a Content of the given id or NULL.
	 */
	virtual CONTENT_TYPE GetByIdImplementation(
		StandardDimension id
	)
	{
		return LinearStructuralComponentImplementation< CONTENT_TYPE >::FindByIdIn(
			&this->m_contents,
			id
		);
	}

	/**
	* const implementation for getting by id.
	* @param id
	* @return a Content of the given id or NULL.
	*/
	virtual const CONTENT_TYPE GetByIdImplementation(
		StandardDimension id
	) const
	{
		return LinearStructuralComponentImplementation< CONTENT_TYPE >::FindByIdIn(
			&this->m_contents,
			id
		);
	}

	/**
	 * Implementation for getting by name.
	 * @param name
	 * @return a Content of the given name or NULL.
	 */
	virtual CONTENT_TYPE GetByNameImplementation(
		Name name
	)
	{
		return NULL;
	}

	/**
	* const implementation for getting by name.
	* @param name
	* @return a Content of the given name or NULL.
	*/
	virtual const CONTENT_TYPE GetByNameImplementation(
		Name name
	) const
	{
		return NULL;
	}

	/**
	 * Create a CONTENT_TYPE from a given id.
	 * This will likely require a Taxonomy or custom Id <-> type mapping.
	 * @param id
	 * @return
	 */
	virtual CONTENT_TYPE CreateImplementation(StandardDimension id)
	{
		return NULL;
	}

	/**
	 * Tries to find a Content of the given id in *this and, optionally, the Contents beneath.
	 * If such an object doesn't exist, one is created from its Wave.
	 * @param id
	 * @return A Content of the given id.
	 */
	virtual CONTENT_TYPE GetOrCreateByIdImplementation(
		StandardDimension id
	)
	{
		return NULL;
	}

	/**
	* Tries to find a Content of the given name in *this and, optionally, the Contents beneath.
	* If such an object doesn't exist, one is created from its Wave.
	* @param name
	* @return A Content of the given id.
	*/
	virtual CONTENT_TYPE GetOrCreateByNameImplementation(
		Name name
	)
	{
		return NULL;
	}

	/**
	 * Check for content.
	 * Dereferences content (i.e. prevents pointer comparison (unless**)).
	 * @param content
	 * @return whether or not the given content exists in *this
	 */
	virtual bool HasImplementation(const CONTENT_TYPE& content) const
	{
		for (
			typename StructuralComponentImplementation< Linear >::Contents::const_iterator cnt = this->m_contents.begin();
			cnt != this->m_contents.end();
			++cnt
			)
		{
			if ((**cnt) == *content)
			{
				return true;
			}
		}
		return false;
	}

	/**
	 * Copy the contents of another container into *this.
	 * Clone()s each element.
	 * @param other
	 */
	virtual void ImportImplementation(const LinearStructuralComponentImplementation< CONTENT_TYPE >* other)
	{
		BIO_SANITIZE_AT_SAFETY_LEVEL_2(other, ,
			return);

		for (
			typename StructuralComponentImplementation< Linear >::Contents::const_iterator cnt = other->m_contents.begin();
			cnt != other->m_contents.end();
			++cnt
			)
		{
			BIO_SANITIZE_WITH_CACHE(ChemicalCast< CONTENT_TYPE >((*cnt)->Clone()),
				this->AddImplementation(Cast< CONTENT_TYPE >(RESULT)),
				continue);
		}
	}

	/**
	 * Override of Wave method. See that class for details.
	 * If other is an Excitation, call ForEach instead.
	 * @param other
	 */
	virtual Code Attenuate(const physical::Wave* other)
	{
		if (physical::Wave::GetResonanceBetween(
			other,
			ExcitationBase::GetClassProperties()).size())
		{
			ForEachImplementation(ChemicalCast< ExcitationBase* >(other));
			return code::Success();
		}

		Code ret = code::Success();
		for (
			typename StructuralComponentImplementation< Linear >::Contents::iterator cnt = this->m_contents.begin();
			cnt != this->m_contents.end();
			++cnt
			)
		{
			if ((*cnt)->Attenuate(other) != code::Success())
			{
				ret = code::UnknownError();
			}
		}
		return ret;
	}

	/**
	 * Override of Wave method. See that class for details.
	 */
	virtual Code Disattenuate(const physical::Wave* other)
	{
		Code ret = code::Success();
		for (
			typename StructuralComponentImplementation< Linear >::Contents::iterator cnt = this->m_contents.begin();
			cnt != this->m_contents.end();
			++cnt
			)
		{
			if ((*cnt)->Disattenuate(other) != code::Success())
			{
				ret = code::UnknownError();
			}
		}
		return ret;
	}


	/**
	 * Performs the given Excitation on all contents.
	 * @param excitation
	 * @param self a pointer to *this, if *this is a chemical::Substance.
	 */
	virtual Emission ForEachImplementation(
		ExcitationBase* excitation
	)
	{
		Emission ret;
		for (
			typename StructuralComponentImplementation< Linear >::Contents::iterator cnt = this->m_contents.begin();
			cnt != this->m_contents.end();
			++cnt
			)
		{
			ByteStream result;
			excitation->CallDown((*cnt)->AsWave(),
				&result
			);
			ret.push_back(result);
		}
		return ret;
	}

	/**
	 * Gets the Names of all Contents and puts them into a string.
	 * @param separator e.g. ", ", the default, or just " ".
	 * @return the Contents of *this as a string.
	 */
	virtual std::string GetStringFromImplementation(std::string separator = ", ")
	{
		std::string ret = "";
		for (
			typename StructuralComponentImplementation< Linear >::Contents::const_iterator cnt = this->m_contents.begin();
			cnt != this->m_contents.end();
			++cnt
			)
		{
			ret += (*cnt)->GetName();
			if (cnt != this->m_contents.end() - 1)
			{
				ret += separator;
			}
		}
		return ret;
	}

	/**
	 * Deletes & clears the contents of *this.
	 * NOTE: this uses delete, not delete[].
	 */
	virtual void ClearImplementation()
	{
		//No need to delete anything, since our Linear wrapper handles that for us.
		this->m_contents.clear();
	}

};

} //physical namespace
} //bio namespace
