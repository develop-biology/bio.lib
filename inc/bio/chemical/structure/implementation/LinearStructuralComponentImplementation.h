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
#include "StructuralComponentImplementation.h"
#include "bio/physical/Codes.h"
#include "bio/physical/Filters.h"
#include "bio/physical/Identifiable.h"
#include "bio/log/Engine.h"
#include "bio/log/Levels.h"
#include "bio/chemical/Types.h"
#include "bio/chemical/Excitation.h"

namespace bio {
namespace chemical {

/**
 * Basic implementation of methods for a LinearStructuralComponent
 * NOTE: we should support Dimensions other than the StandardDimension. However, the limitations of Atom::Bonds prevent us from indexing more than 1 template variable from the ____Interface.
 * Plus, not supporting other Dimensions makes for cleaner inheritance / downstream code.
 * Support for other Dimensions may be added in a future release.
 * see LinearStructuralComponent.h for more info.
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
	static typename StructuralComponentImplementation< CONTENT_TYPE >::Contents::iterator GetIteratorById(
		StandardDimension id,
		typename StructuralComponentImplementation< CONTENT_TYPE >::Contents* contents
	)
	{
		typename StructuralComponentImplementation< CONTENT_TYPE >::Contents::iterator ret = contents.begin();
		for (
			; ret != contents.end();
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
	static typename StructuralComponentImplementation< CONTENT_TYPE >::Contents::const_iterator GetIteratorById(
		StandardDimension id,
		const typename StructuralComponentImplementation< CONTENT_TYPE >::Contents* contents
	)
	{
		typename StructuralComponentImplementation< CONTENT_TYPE >::Contents::const_iterator ret = contents.begin();
		for (
			; ret != contents.end();
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
	 * @param recurse whether or not to search through contents in searchIn.
	 * @return a pointer to a CONTENT_TYPE of id if one is found within searchIn, NULL if one is not found.
	 */
	static CONTENT_TYPE FindByIdIn(
		typename StructuralComponentImplementation< CONTENT_TYPE >::Contents* searchIn,
		const StandardDimension contentId,
		const bool recurse = false
	)
	{
		typename StructuralComponentImplementation< CONTENT_TYPE >::Contents::iterator itt = GetIteratorFrom(
			searchIn,
			contentId,
			recurse
		);
		if (itt != searchIn->end())
		{
			//BIO_LOG_DEBUG("Found %s in %s", name, ConvertContentsToString(*searchIn).c_str());
			return (*itt);
		}
		//BIO_LOG_DEBUG("Could not find %s in %s", name, ConvertContentsToString(*searchIn).c_str());
		return NULL;
	}

	/**
	 * Searches for a Content of the given id.
	 * @param searchIn contents to search through.
	 * @param contentId id to find.
	 * @param recurse whether or not to search through contents in searchIn.
	 * @return a pointer to a CONTENT_TYPE of id if one is found within searchIn, NULL if one is not found.
	 */
	static const CONTENT_TYPE FindByIdIn(
		const typename StructuralComponentImplementation< CONTENT_TYPE >::Contents* searchIn,
		const StandardDimension contentId,
		const bool recurse = false
	)
	{
		typename StructuralComponentImplementation< CONTENT_TYPE >::Contents::const_iterator itt = GetIteratorFrom(
			searchIn,
			contentId,
			recurse
		);
		if (itt != searchIn->end())
		{
			//BIO_LOG_DEBUG("Found %s in %s", name, ConvertContentsToString(*searchIn).c_str());
			return (*itt);
		}
		//BIO_LOG_DEBUG("Could not find %s in %s", name, ConvertContentsToString(*searchIn).c_str());
		return NULL;
	}

	/**
	 * Searches for a Content of the given name.
	 * @param perspective where to get the id for the given name.
	 * @param searchIn contents to search through.
	 * @param contentId id to find.
	 * @param recurse whether or not to search through contents in searchIn.
	 * @return a pointer to a CONTENT_TYPE of id if one is found within searchIn, NULL if one is not found.
	 */
	static CONTENT_TYPE FindByNameIn(
		const physical::Perspective< StandardDimension >* perspective,
		typename StructuralComponentImplementation< CONTENT_TYPE >::Contents* searchIn,
		Name contentName,
		const bool recurse = false
	)
	{
		BIO_SANITIZE(perspective && searchIn, ,
			return NULL);
		return FindByIdIn(
			searchIn,
			perspective->GetIdWithoutCreation(contentName),
			recurse
		);
	}

	/**
	 * Searches for a Content of the given name.
	 * @param perspective where to get the id for the given name.
	 * @param searchIn contents to search through.
	 * @param contentId id to find.
	 * @param recurse whether or not to search through contents in searchIn.
	 * @return a pointer to a CONTENT_TYPE of id if one is found within searchIn, NULL if one is not found.
	 */
	static const CONTENT_TYPE FindByNameIn(
		const physical::Perspective< StandardDimension >* perspective,
		const typename StructuralComponentImplementation< CONTENT_TYPE >::Contents* searchIn,
		Name contentName,
		const bool recurse = false
	)
	{
		BIO_SANITIZE(perspective && searchIn, ,
			return NULL);
		return FindByIdIn(
			searchIn,
			perspective->GetIdWithoutCreation(contentName),
			recurse
		);
	}

	/**
	 * Same as FindContent, but returns an iterator of searchIn, rather than the CONTENT_TYPE
	 * Depth-first search.
	 * @param searchIn contents to search through.
	 * @param contentId id to find.
	 * @param recurse whether or not to search through contents in searchIn.
	 * @return iterator of searchIn matching contentId.
	 */
	static typename StructuralComponentImplementation< CONTENT_TYPE >::Contents::iterator GetIteratorFrom(
		typename StructuralComponentImplementation< CONTENT_TYPE >::Contents* searchIn,
		const StandardDimension contentId,
		const bool recurse = false
	)
	{
		BIO_SANITIZE(searchIn, ,
			return searchIn->end());

		typename StructuralComponentImplementation< CONTENT_TYPE >::Contents::iterator ret = searchIn->end();
		typename StructuralComponentImplementation< CONTENT_TYPE >::Contents::iterator recur;
		for (
			typename StructuralComponentImplementation< CONTENT_TYPE >::Contents::iterator cnt = searchIn->begin();
			cnt != searchIn->end();
			++cnt
			)
		{
			if (*(*cnt) == contentId)
			{
				#if 0
				if (ret != searchIn->end())
				{
					BIO_LOG_DEBUG("Multiple matches found for content %s", name);
				}
				#endif
				ret = cnt;
			}
			else if (recurse)
			{
				recur = (*cnt)->GetContentIteratorFrom((*cnt)->GetAll< CONTENT_TYPE >(),
					contentId,
					recurse
				);
				if (recur != (*cnt)->GetAll< CONTENT_TYPE >()->end())
				{
					#if 0
					if (ret != searchIn->end())
					{
						BIO_LOG_DEBUG("Multiple matches found for content %s", name);
					}
					#endif
					ret = recur;
				}
			}
		}
		return ret;
	}

	/**
	 * Same as FindContent, but returns an iterator of searchIn, rather than the CONTENT_TYPE
	 * Depth-first search.
	 * @param searchIn contents to search through.
	 * @param contentId id to find.
	 * @param recurse whether or not to search through contents in searchIn.
	 * @return iterator of searchIn matching contentId.
	 */
	static typename StructuralComponentImplementation< CONTENT_TYPE >::Contents::const_iterator GetIteratorFrom(
		const typename StructuralComponentImplementation< CONTENT_TYPE >::Contents* searchIn,
		const StandardDimension contentId,
		const bool recurse = false
	)
	{
		BIO_SANITIZE(searchIn, ,
			return searchIn->end());

		typename StructuralComponentImplementation< CONTENT_TYPE >::Contents::const_iterator ret = searchIn->end();
		typename StructuralComponentImplementation< CONTENT_TYPE >::Contents::const_iterator recur;
		for (
			typename StructuralComponentImplementation< CONTENT_TYPE >::Contents::const_iterator cnt = searchIn->begin();
			cnt != searchIn->end();
			++cnt
			)
		{
			if (*(*cnt) == contentId)
			{
				#if 0
				if (ret != searchIn->end())
				{
					BIO_LOG_DEBUG("Multiple matches found for content %s", name);
				}
				#endif
				ret = cnt;
			}
			else if (recurse)
			{
				recur = (*cnt)->GetContentIteratorFrom((*cnt)->GetAll< CONTENT_TYPE >(),
					contentId,
					recurse
				);
				if (recur != (*cnt)->GetAll< CONTENT_TYPE >()->end())
				{
					#if 0
					if (ret != searchIn->end())
					{
						BIO_LOG_DEBUG("Multiple matches found for content %s", name);
					}
					#endif
					ret = recur;
				}
			}
		}
		return ret;
	}

	/**
	 * 	Essentially a find method for a recursive vector of vectors.
	 * 	All InvalidId()s in searchPath are skipped.
	 * @param startingPoint what to search; similar to a top level directory (/) or working directory (./).
	 * @param searchPath where to search; similar to a file path if contents are directories; meaning that the first entry is a CONTENT_TYPE within startingPoint, the second entry is a CONTENT_TYPE within that Content's Contents, and so on.
	 * @return the Contents of a specified point within startingPoint.
	 */
	static typename StructuralComponentImplementation< CONTENT_TYPE >::Contents* FollowTreeOf(
		typename StructuralComponentImplementation< CONTENT_TYPE >::Contents* startingPoint,
		const Dimensions& searchPath
	)
	{
		BIO_SANITIZE(startingPoint, ,
			return NULL);
		BIO_SANITIZE(searchPath.size(), ,
			return startingPoint);

		typename StructuralComponentImplementation< CONTENT_TYPE >::Contents* searchTree = startingPoint;

		for (
			typename Dimensions::const_iterator insert = searchPath.begin();
			insert != searchPath.end();
			++insert
			)
		{
			//Skip over invalid values.
			//It is up to Cell or other, non-generic, methods of CONTENT_TYPE to respect these.
			//TODO make an array of these items and document each one.
			if (*insert == CONTENT_TYPE::Perspective::InvalidId())
			{
				if (insert == searchPath.end() - 1)
				{
					return searchTree;
				}
				else
				{
					continue;
				}
			}

			for (
				typename StructuralComponentImplementation< CONTENT_TYPE >::Contents::iterator cnt = searchTree->begin();
				cnt != searchTree->end();
				++cnt
				)
			{
				if (*(*cnt) == (*insert))
				{
					if (insert == searchPath.end() - 1)
					{
						return (*cnt)->GetAll< CONTENT_TYPE >();
					}
					else if ((*cnt)->GetCount< CONTENT_TYPE >())
					{
						searchTree = (*cnt)->GetAll< CONTENT_TYPE >();
						cnt = searchTree->begin();
					}
					else
					{
						//There is no more tree to follow, give up.
						return NULL;
					}
				}
				else if (cnt == searchTree->end() - 1)
				{
					//No match was found, do not continue.
					return NULL;
				}
			}
		}

		return NULL;
	}

	/**
	 * Turns a vector into a string.
	 * @param contents
	 * @return a string of content names, each separated by a comma
	 */
	static std::string ConvertToString(const typename StructuralComponentImplementation< CONTENT_TYPE >::Contents& contents)
	{
		std::string ret = "";
		for (
			typename StructuralComponentImplementation< CONTENT_TYPE >::Contents::const_iterator cnt = contents.begin();
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
	static Code AddAt(
		physical::Perspective< StandardDimension >* perspective,
		CONTENT_TYPE content,
		typename StructuralComponentImplementation< CONTENT_TYPE >::Contents* destination,
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

		typename StructuralComponentImplementation< CONTENT_TYPE >* toReplace = NULL;

		#if 0
		if (logger)
		{
			logger->Log(filter::Default(), log::level::Debug(), "Adding %s (%p) to %s (%p)", content->GetName(), content, ConvertContentsToString(*destination).c_str(), destination);
		}
		#endif

		//Remove conflicts
		for (
			typename StructuralComponentImplementation< CONTENT_TYPE >::Contents::iterator cnt = destination->begin();
			cnt != destination->end();
			++cnt
			)
		{
			if (*(*cnt) == content->GetId())
			{
				#if 0
				if (logger)
				{
					logger->Log(filter::Default(), log::level::Debug(), "%s (%p) has %u contents", (*cnt)->GetName(), *cnt, (*cnt)->GetAll<CONTENT_TYPE>()->size());
				}
				#endif

				//Not an error, but potentially worth noting.
				ret = code::SuccessfullyReplaced();

				toReplace = *cnt;
				destination->erase(cnt);
				break; //Only find 1 conflict, as no others should exist.
			}
		}

		CONTENT_TYPE addition = content->Clone();
		BIO_SANITIZE(addition, ,
			return code::GeneralFailure());

		if (transferSubContents)
		{
			addition->ImportAll(
				toReplace,
				true
			);

			#if 0
			if (logger)
			{
				 logger->Log(filter::Default(), log::level::Debug(), "%s (%p) will replace %s", content->GetName(), content, toReplace->GetName());
			 }
			#endif
		}

		if (toReplace)
		{
			delete toReplace;
			toReplace = NULL;
		}

		#if 0
		if (logger)
		{
			logger->Log(filter::Default(), log::level::Debug(), "Adding new content, %s (%p), to %s", addition->GetName(), (void*)addition, ConvertContentsToString(*destination).c_str());
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
					logger->Log(filter::Default(), log::level::Debug(), "Confirming: %s (%p) in %s", addition->GetName(), (void*)addition, ConvertContentsToString(*destination).c_str());
				}
				#endif
				break;
			}
			case BEFORE:
			{
				for (
					typename StructuralComponentImplementation< CONTENT_TYPE >::Contents::iterator cnt = destination->begin();
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
								"Could not find content %s in %s",
								perspective->GetNameFromId(optionalPositionArg),
								ConvertContentsToString(*destination).c_str());
						}
						#endif
					}
				}
				break;
			}
			case AFTER:
			{
				for (
					typename StructuralComponentImplementation< CONTENT_TYPE >::Contents::iterator cnt = destination->begin();
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
								"Could not find content %s in %s",
								perspective->GetNameFromId(optionalPositionArg),
								ConvertContentsToString(*destination).c_str());
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

		//TODO: support setting the log engine of addition?

		return ret;
	}


	/**
	 * Inserts contentPtr into the destination at the specified insertionPoint and position.
	 * Multiple contents of the same id will cause the previously existing Content to be removed.
	 * @param perspective used for name to id conversions (e.g. with the optionalPositionArg).
	 * @param contentPtr what to insert. IMPORTANT: This must not already be in a LinearStructuralComponent (i.e. create a clone() before adding it to another destination).
	 * @param destination what to insert into.
	 * @param insertionPoint path of Ids that describes where to add contentPtr.
	 * @param position determines where in destination the content is inserted.
	 * @param optionalPositionArg If a position is specified, the optionalPositionArg is the id of the content referenced (e.g. BEFORE, MyContentId()).
	 * @param transferSubContents allows all of the Contents within a conflicting Content to be copied into the new Content, before the conflicting Content is deleted (similar to renaming an upper directory while preserving it's contents).
	 * @param logger what to use to print messages (since this is static).
	 * @return Status of insertion (e.g. success or failure).
	 */
	static Code

	Insert(
		physical::Perspective< StandardDimension >* perspective,
		void* contentPtr,
		typename StructuralComponentImplementation< CONTENT_TYPE >::Contents* destination,
		const Dimensions& insertionPoint,
		const Position position = BOTTOM,
		const StandardDimension optionalPositionArg = CONTENT_TYPE::Perspective::InvalidId(),
		const bool transferSubContents = false,
		log::Engine* logger = NULL
	)
	{
		BIO_SANITIZE(contentPtr, ,
			return code::MissingArgument2());

		if (!insertionPoint.

			size()

			)
		{
			return AddAt(
				perspective,
				contentPtr,
				destination,
				position,
				optionalPositionArg,
				transferSubContents,
				logger
			);
			//TODO add a check here for improper configuration
		}

		CONTENT_TYPE content = Cast< CONTENT_TYPE >(contentPtr);
		BIO_SANITIZE(content, ,
			code::BadArgument2());

		if (std::find(
			insertionPoint.

				begin(),
			insertionPoint

				.

					end(),
			CONTENT_TYPE::Perspective::InvalidId()

		) != insertionPoint.

			end()

			)
		{
			return

				code::NoErrorNoSuccess();

		}

		std::string insertionPointStr = "";
		typename StructuralComponentImplementation< CONTENT_TYPE >::Contents* searchTree = FollowTreeOf(
			destination,
			insertionPoint
		);
		if (!searchTree)
		{
			for (
				typename Dimensions::const_iterator cid = insertionPoint.begin();
				cid != insertionPoint.

					end();

				++cid
				)
			{
				insertionPointStr += perspective->GetNameFromId(*cid);
				if (cid != insertionPoint.

					end()

					- 1)
				{
					insertionPointStr += ", ";
				}
			}
			#if 0
			if (logger)
			 {
				 logger->Log(filter::Default(), log::level::Debug(),"Insertion point %s not found for content %s", content->GetName(), insertionPointStr.c_str());
			 }
			#endif
			return

				code::InsertionPointMissing();

		}

		#if 0
		if (logger)
		{
			 logger->Log(filter::Default(), log::level::Debug(),"%s adding content %s to insertion point %s", GetEpitopesString().c_str(), content->GetName(), insertionPointStr.c_str());
		}
		#endif

		return AddAt(
			perspective,
			content,
			searchTree,
			position,
			optionalPositionArg,
			transferSubContents,
			logger
		);
	}

	/**
	 * Implementation for inserting a Content to *this.
	 * @param toAdd
	 * @param insertionPoint
	 * @param position
	 * @param optionalPositionArg
	 * @param transferSubContents
	 * @return status of insertion
	 */
	virtual Code InsertImplementation(
		CONTENT_TYPE toAdd,
		const Dimensions& insertionPoint,
		const Position position = BOTTOM,
		const StandardDimension optionalPositionArg = CONTENT_TYPE::Perspective::InvalidId(),
		const bool transferSubContents = false
	) = 0;

	/**
	 * Implementation for getting by id.
	 * @param id
	 * @param recurse
	 * @return a Content of the given id or NULL.
	 */
	virtual CONTENT_TYPE GetByIdImplementation(
		StandardDimension id,
		const bool recurse = false
	)
	{
		return LinearStructuralComponentImplementation< CONTENT_TYPE >::FindByIdIn(
			&m_contents,
			id,
			recurse
		);
	}

	/**
	* const implementation for getting by id.
	* @param id
	* @param recurse
	* @return a Content of the given id or NULL.
	*/
	virtual const CONTENT_TYPE GetByIdImplementation(
		StandardDimension id,
		const bool recurse = false
	) const
	{
		return LinearStructuralComponentImplementation< CONTENT_TYPE >::FindByIdIn(
			&m_contents,
			id,
			recurse
		);
	}

	/**
	 * Implementation for getting by name.
	 * @param name
	 * @param recurse
	 * @return a Content of the given name or NULL.
	 */
	virtual CONTENT_TYPE GetByNameImplementation(
		Name name,
		const bool recurse = false
	)
	{
		return NULL;
	}

	/**
	* const implementation for getting by name.
	* @param name
	* @param recurse
	* @return a Content of the given name or NULL.
	*/
	virtual const CONTENT_TYPE GetByNameImplementation(
		Name name,
		const bool recurse = false
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
	 * @param recurse
	 * @return A Content of the given id.
	 */
	virtual CONTENT_TYPE GetOrCreateByIdImplementation(
		StandardDimension id,
		const bool recurse = false
	)
	{
		return NULL;
	}

	/**
	* Tries to find a Content of the given name in *this and, optionally, the Contents beneath.
	* If such an object doesn't exist, one is created from its Wave.
	* @param name
	* @param recurse
	* @return A Content of the given id.
	*/
	virtual CONTENT_TYPE GetOrCreateByNameImplementation(
		Name name,
		const bool recurse = false
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
	virtual bool HasImplementation(CONTENT_TYPE content) const
	{
		for (
			typename StructuralComponentImplementation< CONTENT_TYPE >::Contents::iterator cnt = this->m_contents.begin();
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
			typename StructuralComponentImplementation< CONTENT_TYPE >::Contents::const_iterator cnt = other.m_contents.begin();
			cnt != other.m_contents.end();
			++cnt
			)
		{
			AddImplementation((*cnt)->Clone());
		}
	}

	/**
	 * Override of Wave method. See that class for details.
	 * If other is an Excitation, call ForEach instead.
	 * @param other
	 */
	virtual void Attenuate(const physical::Wave* other)
	{
		if (physical::Wave::GetResonnanceBetween(
			other,
			ExcitationBase::GetClassProperties()).size())
		{
			ForEachImplementation(Cast<ExcitationBase*>(other));
			return;
		}

		for (
			typename StructuralComponentImplementation< CONTENT_TYPE >::Contents::iterator cnt = this->m_contents.begin();
			cnt != this->m_contents.end();
			++cnt
			)
		{
			(*cnt)->Attenuate(other);
		}
	}

	/**
	 * Override of Wave method. See that class for details.
	 */
	virtual void Disattenuate(const physical::Wave* other)
	{
		for (
			typename StructuralComponentImplementation< CONTENT_TYPE >::Contents::iterator cnt = this->m_contents.begin();
			cnt != this->m_contents.end();
			++cnt
			)
		{
			(*cnt)->Disattenuate(other);
		}
	}


	/**
	 * Performs the given Excitation on all contents.
	 * @param excitation
	 * @param self a pointer to *this, if *this is a chemical::Substance.
	 */
	virtual ExcitationReturns ForEachImplementation(
		ExcitationBase* excitation
	)
	{
		ExcitationReturns ret;
		for (
			typename StructuralComponentImplementation< CONTENT_TYPE >::Contents::const_iterator cnt = this->m_contents.begin();
			cnt != this->m_contents.end();
			++cnt
			)
		{
			ret.push_back(excitation->CallDown(*cnt));
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
		for (typename StructuralComponentImplementation< CONTENT_TYPE >::Contents::const_iterator cnt = m_contents.begin(), cnt != m_contents.end();
		++cnt)
		{
			ret += (*cnt)->GetName();
			if (cnt != m_contents.end() - 1)
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
		for (
			typename StructuralComponentImplementation< CONTENT_TYPE >::Contents::iterator cnt = this->m_contents.begin();
			cnt != this->m_contents.end();
			++cnt
			)
		{
			BIO_SANITIZE(*cnt,
				delete (*cnt),);
			(*cnt) = NULL;
		}
		m_contents.clear();
	}

};

} //physical namespace
} //bio namespace
