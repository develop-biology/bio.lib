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

#include "UnorderedMotif.h"
#include "bio/chemical/common/Cast.h"
#include "bio/chemical/common/Class.h"
#include "bio/chemical/common/Properties.h"
#include "bio/chemical/reaction/Excitation.h"
#include "bio/physical/shape/Line.h"

#if BIO_CPP_VERSION >= 11

	#include <type_traits>

#endif

namespace bio {
namespace chemical {

/**
 * LinearMotif objects contain pointers to chemical::Classes. <br />
 *
 * IMPORTANT: CONTENT_TYPE MUST BE A chemical::Class* (which is in the Id).
 * YOU CANNOT USE LinearMotif WITH TYPES THAT ARE NOT POINTERS TO CHILDREN OF chemical::Class (i.e. a physical::Identifiable<Id>) <br />
 * Other Dimensions may be supported in a future release. <br />
 * physical::Line and physical::Linear for why. <br />
 *
 * NOTE: CONTENT_TYPE cannot be "const". <br />
 * cv qualifiers may be supported in a future release but for now, all CONTENT_TYPEs must have the option of being modified. <br />
 * For more on this, see Linear.h. <br />
 *
 * @tparam CONTENT_TYPE a pointer type to a child of chemical::Class
 */
template < typename CONTENT_TYPE >
class LinearMotif :
	public chemical::Class< LinearMotif< CONTENT_TYPE > >,
	public UnorderedMotif< CONTENT_TYPE >
{
private:

	/**
	 * Common constructor code. <br />
	 */
	void CommonConstructor()
	{
		//TODO: check if T is a child of Substance.
		//This will do for now.
		BIO_STATIC_ASSERT(type::IsPointer< CONTENT_TYPE >())

		RegisterProperties(GetClassProperties());

		mContentId = SafelyAccess< PeriodicTable >()->template GetIdFromType< CONTENT_TYPE >();

		if (this->mContents)
		{
			delete this->mContents;
		}
		this->mContents = new physical::Line(4);
	}


public:

	/**
	 * For cleaner code, we redefine Contents. <br />
	 */
	typedef typename physical::Line Contents;

	/**
	 * Ensure virtual methods point to Class implementations. <br />
	 * We want to define our own Attenuate & Disattenuate, so we have to ignore the optional class methods for the chemical class. <br />
	 */
	BIO_DISAMBIGUATE_REQUIRED_CLASS_METHODS(chemical, LinearMotif< CONTENT_TYPE >)
	BIO_DISAMBIGUATE_OPTIONAL_CLASS_METHODS(physical, LinearMotif< CONTENT_TYPE >)

	/**
	 * Add property::Linear() to what is given by AbstractMotif. <br />
	 * @return {Structural(), Linear()}
	 */
	static Properties GetClassProperties()
	{
		Properties ret = AbstractMotif::GetClassProperties();
		ret.Add(property::Linear());
		return ret;
	}

	/**
	 * @param perspective
	 */
	explicit LinearMotif() :
		chemical::Class< LinearMotif< CONTENT_TYPE > >(this)
	{
		CommonConstructor();
	}

	/**
	 * @param contents
	 * @param perspective
	 */
	explicit LinearMotif(const Contents* contents) :
		chemical::Class< LinearMotif< CONTENT_TYPE > >(this)
	{
		CommonConstructor();
		this->mContents->Import(contents);
	}

	/**
	 * Copying a LinearMotif will Clone all contents in toCopy into *this. <br />
	 * Keep in mind that dtor will delete the contents of *this. <br />
	 * @param toCopy
	 */
	LinearMotif(const LinearMotif< CONTENT_TYPE >& toCopy)
		:
		chemical::Class< LinearMotif< CONTENT_TYPE > >(this)
	{
		CommonConstructor();
		this->mContents->Import(toCopy.GetAllImplementation());
	}

	/**
	 * Deletes all the Contents in *this. <br />
	 * NOTE: this uses delete, not delete[]. <br />
	 * The only way to avoid this is by Clear()ing *this yourself first. <br />
	 */
	virtual ~LinearMotif()
	{
		this->mContents->Clear();
	}

	/**
	 * Each LinearMotif may use a different Perspective for identifying its contents. <br />
	 * This Perspective will be used for Name <-> Id matching, Wave->Clone()ing, etc. <br />
	 * See bio/physical/Perspective.h for more details. <br />
	 */
	mutable AtomicNumber mContentId;

	/**
	 * Adds content to *this. <br />
	 * Added objects are "owned" (not physical::Linear::IsShared()) and will be deleted with *this. <br />
	 * @param content
	 * @return t or NULL.
	 */
	virtual CONTENT_TYPE AddImplementation(CONTENT_TYPE content)
	{
		Index addedPosition = this->mContents->Add(physical::Linear(content)); //stored as shared...
		BIO_SANITIZE(addedPosition, , return NULL)
		physical::Linear& added = Cast< physical::Line* >(this->mContents)->OptimizedAccess(addedPosition);
		added.SetShared(false); //...but added contents are not shared.
		CONTENT_TYPE ret = ChemicalCast< CONTENT_TYPE >(added.operator physical::Identifiable< Id >*());
		BIO_SANITIZE(ret == content, , return NULL)
		return ret;
	}

	/**
	 * Adds a Content in *this at the indicated position. <br />
	 * Multiple contents of the same id will cause the previously existing Content to be removed. <br />
	 *
	 * NOTE: THIS DESTROYS INDEX INTEGRITY. <br />
	 * Indices will be rearranged to accommodate the insertion, making any cached Index invalid. <br />
	 *
	 * @param toAdd what to add. IMPORTANT: This must not already be in a LinearMotif (i.e. create a clone() before adding it to another destination).
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

		Code ret = code::Success();

		SmartIterator toReplace(
			this->mContents,
			InvalidIndex());

		//Remove conflicts
		for (
			SmartIterator cnt = this->mContents->End();
			!cnt.IsBeforeBeginning();
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

		CONTENT_TYPE additionContent = CloneAndCast< CONTENT_TYPE >(toAdd);
		BIO_SANITIZE(additionContent, , return code::GeneralFailure())
		physical::Linear addition(additionContent);

		if (this->mContents->IsAllocated(toReplace.GetIndex())) //i.e. GetIndex() != 0.
		{
			if (transferSubContents)
			{
				//NOTE: THIS REMOVES ALL STRUCTURAL COMPONENTS IN toReplace WHICH ARE NOT EXPLICITLY IN addition
				//This makes sense but is bound to be a bug at some point...

				CONTENT_TYPE toReplaceCasted = toReplace.template As< CONTENT_TYPE >();
				//addition->ImportAll(toReplaceCasted); //<- inaccessible, so we replicate the function here.

				Bond* bond;
				for (
					SmartIterator bnd = addition->AsAtom()->GetAllBonds()->End();
					!bnd.IsBeforeBeginning();
					--bnd
					)
				{
					bond = bnd;
					if (bond->IsEmpty())
					{
						continue;
					}
					if (physical::Wave::GetResonanceBetween(
						bond->GetBonded(),
						AbstractMotif::GetClassProperties()).Size() == 0)
					{
						continue;
					}
					const physical::Wave* otherBond = toReplaceCasted->AsAtom()->GetBonded(toReplaceCasted->AsAtom()->GetBondPosition(bond->GetId()));
					Cast< AbstractMotif* >(bond->GetBonded())->ImportImplementation(otherBond); //actual work
				}
			}
			this->mContents->Erase(toReplace);
		}

		switch (position)
		{
			case TOP:
			{
				this->mContents->Insert(
					addition,
					this->mContents->GetBeginIndex());
				break;
			}
			case BEFORE:
			{
				Index placement = Cast< physical::Line* >(this->mContents)->SeekToId(optionalPositionArg);
				if (!placement)
				{
					return code::GeneralFailure();
				}
				BIO_SANITIZE(Cast< physical::Line* >(this->mContents)->LinearAccess(placement)->GetPerspective() == addition->GetPerspective(), ,
					return code::GeneralFailure());

				this->mContents->Insert(
					addition,
					placement
				);
				break;
			}
			case AFTER:
			{
				Index placement = Cast< physical::Line* >(this->mContents)->SeekToId(optionalPositionArg);
				if (!placement)
				{
					return code::GeneralFailure();
				}
				BIO_SANITIZE(Cast< physical::Line* >(this->mContents)->LinearAccess(placement)->GetPerspective() == addition->GetPerspective(), ,
					return code::GeneralFailure());

				this->mContents->Insert(
					addition,
					++placement
				);
				break;
			}
			case BOTTOM:
			{
				this->mContents->Insert(
					addition,
					this->mContents->GetEndIndex());
				break;
			}
			default:
			{
				this->mContents->Add(addition);
				break;
			}
		} //switch

		return ret;
	}

	/**
	 * Implementation for getting by id. <br />
	 * @param id
	 * @return a Content of the given id or NULL.
	 */
	virtual CONTENT_TYPE GetByIdImplementation(const Id& id)
	{
		Index found = Cast< physical::Line* >(this->mContents)->SeekToId(id);
		BIO_SANITIZE_AT_SAFETY_LEVEL_1(found, , return NULL) //level 2 for GetOrCreate.

		physical::Identifiable< Id >* got = Cast< physical::Line* >(this->mContents)->LinearAccess(found);
		CONTENT_TYPE ret = ChemicalCast< CONTENT_TYPE >(got);
		return ret;
	}

	/**
	 * const interface for getting by id. <br />
	 * @param id
	 * @return a Content of the given id or NULL.
	 */
	 virtual const CONTENT_TYPE GetByIdImplementation(const Id& id) const
	{
		Index found = Cast< physical::Line* >(this->mContents)->SeekToId(id);
		BIO_SANITIZE_AT_SAFETY_LEVEL_1(found, , return NULL) //level 2 for GetOrCreate.

		physical::Identifiable< Id >* got = Cast< physical::Line* >(this->mContents)->LinearAccess(found);
		CONTENT_TYPE ret = ChemicalCast< CONTENT_TYPE >(got);
		return ret;
	}


	/**
	 * Implementation for getting by name. <br />
	 * @param name
	 * @return a CONTENT_TYPE of the given name or NULL.
	 */
	virtual CONTENT_TYPE GetByNameImplementation(const Name& name)
	{
		Index found = Cast< physical::Line* >(this->mContents)->SeekToName(name);
		BIO_SANITIZE_AT_SAFETY_LEVEL_1(found, , return NULL) //level 2 for GetOrCreate.

		physical::Identifiable< Id >* got = Cast< physical::Line* >(this->mContents)->LinearAccess(found);
		CONTENT_TYPE ret = ChemicalCast< CONTENT_TYPE >(got);
		return ret;
	}

	/**
	 * Implementation for getting by name. <br />
	 * @param name
	 * @return a CONTENT_TYPE of the given name or NULL.
	 */
	virtual const CONTENT_TYPE GetByNameImplementation(const Name& name) const
	{
		Index found = Cast< physical::Line* >(this->mContents)->SeekToName(name);
		BIO_SANITIZE_AT_SAFETY_LEVEL_1(found, , return NULL) //level 2 for GetOrCreate.

		physical::Identifiable< Id >* got = Cast< physical::Line* >(this->mContents)->LinearAccess(found);
		CONTENT_TYPE ret = ChemicalCast< CONTENT_TYPE >(got);
		return ret;
	}

	/**
	 * Create a CONTENT_TYPE from a given Id and adds it to *this. <br />
	 * Clones the Wave associated with the given Id. <br />
	 * This requires a valid Perspective in *this and for that Perspective to have an Wave registered with the given Id. <br />
	 * @param id
	 * @return a newly created CONTENT_TYPE else NULL.
	 */
	virtual CONTENT_TYPE CreateImplementation(const Id& id)
	{
		BIO_SANITIZE(this->GetStructuralPerspective(), , return NULL)
		CONTENT_TYPE* created = SafelyAccess< PeriodicTable >()->template GetInstance< CONTENT_TYPE >();
		BIO_SANITIZE(created, , return NULL)
		created->SetPerspective(IdPerspective::Instance());
		created->SetId(id);
		return this->AddImplementation(created);
	}

	/**
	 * Tries to find a Content of the given id in *this and, optionally, the Contents beneath. <br />
	 * If such an object doesn't exist, one is created from its Wave. <br />
	 * @param id
	 * @return A CONTENT_TYPE of the given id.
	 */
	virtual CONTENT_TYPE GetOrCreateByIdImplementation(const Id& id)
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
	 * Tries to find a Content of the given id in *this and, optionally, the Contents beneath. <br />
	 * If such an object doesn't exist, one is created from its Wave. <br />
	 * @param name
	 * @return A CONTENT_TYPE of the given id.
	 */
	virtual CONTENT_TYPE GetOrCreateByNameImplementation(const Name& name)
	{
		BIO_SANITIZE(this->GetStructuralPerspective(), , return NULL);
		//We convert to Id in case the Name is not already registered in the desired Perspective.
		Id id = this->GetStructuralPerspective()->GetIdFromName(name);
		CONTENT_TYPE ret = this->GetByIdImplementation(id);
		if (ret)
		{
			return ret;
		}
		return this->CreateImplementation(id);
	}

	/**
	 * const interface for removal by id. <br />
	 * @param id
	 * @return the CONTENT_TYPE removed or NULL.
	 */
	virtual CONTENT_TYPE RemoveByIdImplementation(const Id& id)
	{
		Index found = Cast< physical::Line* >(this->mContents)->SeekToId(id);
		BIO_SANITIZE(found, , return NULL)

		physical::Identifiable< Id >* got = Cast< physical::Line* >(this->mContents)->LinearAccess(found);
		CONTENT_TYPE ret = ChemicalCast< CONTENT_TYPE >(got);
		this->mContents->Erase(found); //Erases the pointer but the pointed-to object should still exist.
		return ret;
	}


	/**
	 * Implementation for removal by name. <br />
	 * @param name
	 * @return the CONTENT_TYPE removed or NULL.
	 */
	virtual CONTENT_TYPE RemoveByNameImplementation(const Name& name)
	{
		Index found = Cast< physical::Line* >(this->mContents)->SeekToName(name);
		BIO_SANITIZE(found, , return NULL)

		physical::Identifiable< Id >* got = Cast< physical::Line* >(this->mContents)->LinearAccess(found);
		CONTENT_TYPE ret = ChemicalCast< CONTENT_TYPE >(got);
		this->mContents->Erase(found); //Erases the pointer but the pointed-to object should still exist.
		return ret;
	}

	/**
	 * Check for content. <br />
	 * Dereferences content (i.e. prevents pointer comparison (unless**)). <br />
	 * @param content
	 * @return whether or not the given content exists in *this
	 */
	virtual bool HasImplementation(const CONTENT_TYPE& content) const
	{
		return this->mContents->Has(physical::Linear(content));
	}

	/**
	 * Copy the contents of another container into *this. <br />
	 * Clone()s each element. <br />
	 * @param other
	 */
	virtual void ImportImplementation(const LinearMotif< CONTENT_TYPE >* other)
	{
		BIO_SANITIZE(other, , return);

		this->mContents->Import(other->mContents);
	}

	/**
	 * Override of Wave method. See that class for details. <br />
	 * If other is an Excitation, call ForEach instead. <br />
	 * @param other
	 * @return the result of all Attenuations.
	 */
	virtual Code Attenuate(const physical::Wave* other)
	{
		//if other is an Excitation...
		if (physical::Wave::GetResonanceBetween(
			other,
			ExcitationBase::GetClassProperties()).Size())
		{
			ForEachImplementation(ChemicalCast< ExcitationBase* >(other));
			return code::Success();
		}

		Code ret = code::Success();
		for (
			SmartIterator cnt = this->mContents;
			!cnt.IsBeforeBeginning();
			--cnt
			)
		{
			if (cnt.template As< physical::Linear >()->Attenuate(other) != code::Success())
			{
				ret = code::UnknownError();
			}
		}
		return ret;
	}

	/**
	 * Override of Wave method. See that class for details. <br />
	 * @param other
	 * @return the result of all Disattenuations.
	 */
	virtual Code Disattenuate(const physical::Wave* other)
	{
		Code ret = code::Success();
		for (
			SmartIterator cnt = this->mContents;
			!cnt.IsBeforeBeginning();
			--cnt
			)
		{
			if (cnt.template As< physical::Linear >()->Disattenuate(other) != code::Success())
			{
				ret = code::UnknownError();
			}
		}
		return ret;
	}


	/**
	 * Performs the given Excitation on all contents. <br />
	 * @param excitation
	 */
	virtual Emission ForEachImplementation(const ExcitationBase* excitation)
	{
		Emission ret;
		for (
			SmartIterator cnt = this->mContents;
			!cnt.IsBeforeBeginning();
			--cnt
			)
		{
			ByteStream result;
			excitation->CallDown(
				cnt.template As< physical::Linear >()->AsWave(),
				&result
			);
			ret.Add(result);
		}
		return ret;
	}

	/**
	 * Gets the Names of all Contents and puts them into a string. <br />
	 * @param separator e.g. ", ", the default, or just " ".
	 * @return the Contents of *this as a string.
	 */
	virtual ::std::string GetStringFromImplementation(std::string separator = ", ")
	{
		std::string ret = "";

		for (
			SmartIterator cnt(
				this->mContents,
				this->mContents->GetBeginIndex());
			!cnt.IsAfterEnd();
			++cnt
			)
		{
			ret += cnt.template As< physical::Linear >()->GetName().AsStdString();
			if (cnt.GetIndex() != this->mContents->GetEndIndex() - 1)
			{
				ret += separator;
			}
		}
		return ret;
	}

	/**
	 * Deletes & clears the contents of *this. <br />
	 * NOTE: this uses delete, not delete[]. <br />
	 */
	virtual void ClearImplementation()
	{
		//No need to delete anything, since our Linear wrapper handles that for us.
		this->mContents->Clear();
	}
};
} //chemical namespace
} //bio namespace
