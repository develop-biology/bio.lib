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

#include "bio/chemical/common/Cast.h"
#include "bio/chemical/common/Class.h"
#include "bio/chemical/Element.h"
#include "bio/chemical/structure/StructuralComponent.h"
#include "bio/chemical/structure/implementation/LinearStructureInterface.h"
#include "bio/physical/arrangement/Line.h"

#if BIO_CPP_VERSION >= 11
	#include <type_traits>
#endif

namespace bio {
namespace chemical {

/**
 * LinearStructuralComponent objects contain pointers to chemical::Classes.
 *
 * IMPORTANT: CONTENT_TYPE MUST BE A chemical::Class* (which is in the StandardDimension).
 * YOU CANNOT USE LinearStructuralComponent WITH TYPES THAT ARE NOT POINTERS TO CHILDREN OF chemical::Class (i.e. a physical::Identifiable<StandardDimension>)
 * Other Dimensions may be supported in a future release.
 * physical::Line and physical::Linear for why.
 *
 * NOTE: CONTENT_TYPE cannot be "const".
 * cv qualifiers may be supported in a future release but for now, all CONTENT_TYPEs must have the option of being modified.
 * For more on this, see Linear.h.
 *
 * @tparam CONTENT_TYPE a pointer type to a child of chemical::Class
 */
template < typename CONTENT_TYPE >
class LinearStructuralComponent :
	virtual public LinearStructureInterface,
	public Element< LinearStructuralComponent< CONTENT_TYPE > >,
	public chemical::Class< LinearStructuralComponent< CONTENT_TYPE > >,
	public StructuralComponent< CONTENT_TYPE >
{
public:

	/**
	 * For cleaner code, we redefine Contents.
	 */
	typedef typename physical::Line Contents;

	/**
	 * Ensure virtual methods point to Class implementations.
	 */
	BIO_DISAMBIGUATE_CLASS_METHODS(chemical,
		LinearStructuralComponent< CONTENT_TYPE >)

	/**
	 * Add property::Linear() to what is given by AbstractStructure.
	 * @return {Structural(), Linear()}
	 */
	static Properties GetClassProperties()
	{
		Properties ret = AbstractStructure::GetClassProperties();
		ret.push_back(property::Linear());
		return ret;
	}

	/**
	 * @param perspective
	 */
	explicit LinearStructuralComponent(physical::Perspective< StandardDimension >* perspective = NULL)
		:
		Element< LinearStructuralComponent< CONTENT_TYPE > >(GetClassProperties()),
		chemical::Class< LinearStructuralComponent< CONTENT_TYPE > >(this),
		m_perspective(perspective)
	{
		CtorCommon();
	}

	/**
	 * @param contents
	 * @param name
	 * @param perspective
	 */
	explicit LinearStructuralComponent(
		const Contents* contents,
		physical::Perspective< StandardDimension >* perspective = NULL
	)
		:
		Element< LinearStructuralComponent< CONTENT_TYPE > >(GetClassProperties()),
		chemical::Class< LinearStructuralComponent< CONTENT_TYPE > >(this),
		m_perspective(perspective)
	{
		CtorCommon();
		this->m_contents->Import(contents);
	}

	/**
	 * Copying a LinearStructuralComponent will Clone all contents in toCopy into *this.
	 * Keep in mind that dtor will delete the contents of *this.
	 * @param toCopy
	 */
	LinearStructuralComponent(const LinearStructuralComponent< CONTENT_TYPE >& toCopy)
		:
		Element< LinearStructuralComponent< CONTENT_TYPE > >(toCopy.GetClassProperties()),
		chemical::Class< LinearStructuralComponent< CONTENT_TYPE > >(this),
		m_perspective(toCopy.m_perspective)
	{
		CtorCommon();
		this->m_contents->Import(toCopy.GetAllImplementation());
	}

	/**
	 * Deletes all the Contents in *this.
	 * NOTE: this uses delete, not delete[].
	 * The only way to avoid this is by Clear()ing *this yourself first.
	 */
	virtual ~LinearStructuralComponent()
	{
		this->m_contents->Clear();
	}

	/**
	 * Each LinearStructuralComponent may use a different Perspective for identifying its contents.
	 * This Perspective will be used for Name <-> Id matching, Wave->Clone()ing, etc.
	 * See bio/physical/Perspective.h for more details.
	 */
	physical::Perspective< StandardDimension >* m_perspective;

	/**
	 * @return the m_perspective used by *this.
	 */
	physical::Perspective< StandardDimension >* GetStructuralPerspective()
	{
		return m_perspective;
	}

	/**
	 * @return the m_perspective used by *this.
	 */
	const physical::Perspective< StandardDimension >* GetStructuralPerspective() const
	{
		return m_perspective;
	}

	/**
	 * Adds content to *this.
	 * @param content
	 * @return t or NULL.
	 */
	virtual CONTENT_TYPE AddImplementation(CONTENT_TYPE content)
	{
		return this->m_contents->LinearAccess(this->m_contents->Add(content));
	}

	/**
	 * Adds a Content in *this at the indicated position.
	 * Multiple contents of the same id will cause the previously existing Content to be removed.
	 *
	 * NOTE: THIS DESTROYS INDEX INTEGRITY.
	 * Indices will be rearranged to accommodate the insertion, making any cached Index invalid.
	 *
	 * @param toAdd what to add. IMPORTANT: This must not already be in a LinearStructuralComponent (i.e. create a clone() before adding it to another destination).
	 * @param position determines where in *this the Content is added.
	 * @param optionalPositionArg If a position is specified, the optionalPositionArg is the id of the Content referenced (e.g. BEFORE, MyContentId()).
	 * @param transferSubContents allows all of the Contents within a conflicting Content to be copied into the new Content, before the conflicting Content is deleted (similar to renaming an upper directory while preserving it's contents).
	 * @return Status of addition (e.g. success or failure).
	 */
	virtual Code InsertImplementation(
		CONTENT_TYPE toAdd,
		const Position position = BOTTOM,
		const StandardDimension optionalPositionArg = 0, //i.e. invalid.
		const bool transferSubContents = false
	)
	{
		BIO_SANITIZE(toAdd, ,
			return code::MissingAgument1())

		Code ret = code::Success();

		SmartIterator toReplace(
			this->m_contents,
			InvalidIndex());

		//Remove conflicts
		for (
			SmartIterator cnt = destination->End();
			!cnt->IsAtBeginning();
			--cnt
			)
		{
			if (cnt.template As< CONTENT_TYPE >()->IsId(toAdd->GetId()))
			{
				//Not an error, but potentially worth noting.
				ret = code::SuccessfullyReplaced();

				toReplace = cnt;
				break; //Only find 1 conflict, as no others should exist.
			}
		}

		CONTENT_TYPE addition = CloneAndCast< CONTENT_TYPE >(content);
		BIO_SANITIZE(addition, ,
			return code::GeneralFailure())

		if (this->m_contents->IsAllocated(toReplace.GetIndex())) //i.e. GetIndex() != 0.
		{
			if (transferSubContents)
			{
				//NOTE: THIS REMOVES ANY STRUCTURAL COMPONENTS NOT EXPLICITLY IN addition
				addition->ImportAll(toReplace.template As< CONTENT_TYPE >()->AsWave());
			}
			this->m_contents->Erase(toReplace);
		}

		switch (position)
		{
			case TOP:
			{
				this->m_contents->Insert(
					this->m_contents->GetBeginIndex(),
					addition
				);
				break;
			}
			case BEFORE:
			{
				Index placement = Cast< Line* >(this->m_contents)->SeekToId(optionalPositionArg);
				if (!placement)
				{
					return code::GeneralFailure();
				}
				BIO_SANITIZE(Cast< Line* >(this->m_contents)->LinearAccess(placement)->GetPerspective() == addition->GetPerspective(), ,
					return code::GeneralFailure());

				this->m_contents->Insert(
					addition,
					placement
				);
				break;
			}
			case AFTER:
			{
				Index placement = Cast< Line* >(this->m_contents)->SeekToId(optionalPositionArg);
				if (!placement)
				{
					return code::GeneralFailure();
				}
				BIO_SANITIZE(Cast< Line* >(this->m_contents)->LinearAccess(placement)->GetPerspective() == addition->GetPerspective(), ,
					return code::GeneralFailure());

				this->m_contents->Insert(
					addition,
					++placement
				);
				break;
			}
			case BOTTOM:
			{
				this->m_contents->Insert(
					addition,
					this->m_contents->GetEndIndex());
				break;
			}
			default:
			{
				this->m_contents->Add(addition);
				break;
			}
		} //switch

		return ret;
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
		Index ret = Cast< Line* >(this->m_contents)->SeekToId(id);
		BIO_SANITIZE_AT_SAFETY_LEVEL_2(ret,,return NULL) //level 2 for GetOrCreate.

		return ChemicalCast< CONTENT_TYPE >(Cast< Line* >(this->m_contents)->LinearAccess(ret));
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
		Index ret = Cast< Line* >(this->m_contents)->SeekToId(id);
		BIO_SANITIZE_AT_SAFETY_LEVEL_2(ret,,return NULL) //level 2 for GetOrCreate.

		return ChemicalCast< CONTENT_TYPE >(Cast< Line* >(this->m_contents)->LinearAccess(ret));
	}


	/**
	 * Implementation for getting by name.
	 * @param name
	 * @return a CONTENT_TYPE of the given name or NULL.
	 */
	virtual CONTENT_TYPE GetByNameImplementation(
		Name name
	)
	{
		Index ret = Cast< Line* >(this->m_contents)->SeekToName(name);
		BIO_SANITIZE_AT_SAFETY_LEVEL_2(ret,,return NULL) //level 2 for GetOrCreate.

		return ChemicalCast< CONTENT_TYPE >(Cast< Line* >(this->m_contents)->LinearAccess(ret));
	}

	/**
	 * Implementation for getting by name.
	 * @param name
	 * @return a CONTENT_TYPE of the given name or NULL.
	 */
	virtual const CONTENT_TYPE GetByNameImplementation(
		Name name
	) const
	{
		Index ret = Cast< Line* >(this->m_contents)->SeekToName(name);
		BIO_SANITIZE_AT_SAFETY_LEVEL_2(ret,,return NULL) //level 2 for GetOrCreate.

		return ChemicalCast< CONTENT_TYPE >(Cast< Line* >(this->m_contents)->LinearAccess(ret));
	}

	/**
	 * Create a CONTENT_TYPE from a given Id and adds it to *this.
	 * Clones the Wave associated with the given Id.
	 * This requires a valid Perspective in *this and for that Perspective to have an Wave registered with the given Id.
	 * @param id
	 * @return a newly created CONTENT_TYPE else NULL.
	 */
	virtual CONTENT_TYPE CreateImplementation(
		StandardDimension id
	)
	{
		BIO_SANITIZE(this->GetStructuralPerspective(), ,
			return NULL);
		return this->AddImplementation((this->GetStructuralPerspective()->template GetTypeFromIdAs< CONTENT_TYPE >(id)));
	}

	/**
	 * Tries to find a Content of the given id in *this and, optionally, the Contents beneath.
	 * If such an object doesn't exist, one is created from its Wave.
	 * @param id
	 * @return A CONTENT_TYPE of the given id.
	 */
	virtual CONTENT_TYPE GetOrCreateByIdImplementation(
		StandardDimension id
	)
	{
		CONTENT_TYPE ret = this->GetByIdImplementation(
			id
		);
		if (ret)
		{
			return ret;
		}
		return this->CreateImplementation(id);
	}

	/**
	 * Tries to find a Content of the given id in *this and, optionally, the Contents beneath.
	 * If such an object doesn't exist, one is created from its Wave.
	 * @param name
	 * @return A CONTENT_TYPE of the given id.
	 */
	virtual CONTENT_TYPE GetOrCreateByNameImplementation(
		Name name
	)
	{
		BIO_SANITIZE(this->GetStructuralPerspective(), ,
			return NULL);
		//We convert to Id in case the Name is not already registered in the desired Perspective.
		StandardDimension id = this->GetStructuralPerspective()->GetIdFromName(name);
		CONTENT_TYPE ret = this->GetByIdImplementation(id);
		if (ret)
		{
			return ret;
		}
		return this->CreateImplementation(id);
	}

	/**
	 * Check for content.
	 * Dereferences content (i.e. prevents pointer comparison (unless**)).
	 * @param content
	 * @return whether or not the given content exists in *this
	 */
	virtual bool HasImplementation(const CONTENT_TYPE& content) const
	{
		return this->m_contents->Has(content);
	}

	/**
	 * Copy the contents of another container into *this.
	 * Clone()s each element.
	 * @param other
	 */
	virtual void ImportImplementation(const LinearStructuralComponent< CONTENT_TYPE >* other)
	{
		BIO_SANITIZE(other, ,
			return);

		this->m_contents->Import(other->m_contens);
	}

	/**
	 * Override of Wave method. See that class for details.
	 * If other is an Excitation, call ForEach instead.
	 * @param other
	 * @return the result of all Attenuations.
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
			SmartIterator cnt = this->m_contents;
			!cnt.IsAtBeginning();
			--cnt
			)
		{
			if (cnt.template As< physical::Identifiable< StandardDimension >* >()->Attenuate(other) != code::Success())
			{
				ret = code::UnknownError();
			}
		}
		return ret;
	}

	/**
	 * Override of Wave method. See that class for details.
	 * @param other
	 * @return the result of all Disattenuations.
	 */
	virtual Code Disattenuate(const physical::Wave* other)
	{
		Code ret = code::Success();
		for (
			SmartIterator cnt = this->m_contents;
			!cnt.IsAtBeginning();
			--cnt
			)
		{
			if (cnt.template As< physical::Identifiable< StandardDimension >* >()->Disattenuate(other) != code::Success())
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
			SmartIterator cnt = this->m_contents;
			!cnt.IsAtBeginning();
			--cnt
			)
		{
			ByteStream result;
			excitation->CallDown(
				cnt.template As< physical::Identifiable< StandardDimension >* >()->AsWave(),
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
			SmartIterator cnt(this->m_contents, this->m_contents->GetBeginIndex());
			!cnt.IsAtEnd();
			++cnt
			)
		{
			ret += cnt.template As< physical::Identifiable< StandardDimension >* >()->GetName();
			if (cnt.GetIndex() != this->m_contents.GetEndIndex() - 1)
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
		this->m_contents->Clear();
	}

private:

	/**
	 * Common constructor code.
	 */
	void CtorCommon()
	{
		#if BIO_CPP_VERSION >= 11
		BIO_ASSERT(std::is_base_of<Substance, CONTENT_TYPE>::value);
		#else
		CONTENT_TYPE ct;
		BIO_ASSERT(Cast< Substance* >(&ct) != NULL);
		#endif

		if (m_contents)
		{
			delete m_contents;
		}
		m_contents = new physical::Line(4);
	}

};
} //chemical namespace
} //bio namespace
