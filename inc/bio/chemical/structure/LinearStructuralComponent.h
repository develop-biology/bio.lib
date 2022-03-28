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
#include "bio/chemical/structure/implementation/LinearStructuralComponentImplementation.h"
#include "bio/chemical/structure/implementation/LinearStructureInterface.h"
#include "bio/chemical/arrangement/Linear.h"

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
 * Linear.h for why.
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
	typedef typename StructuralComponent< CONTENT_TYPE >::Contents Contents;

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
		const Contents& contents,
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
	LinearStructuralComponent(const LinearStructuralComponent< CONTENT_TYPE >* toCopy)
		:
		Element< LinearStructuralComponent< CONTENT_TYPE > >(toCopy->GetClassProperties()),
		chemical::Class< LinearStructuralComponent< CONTENT_TYPE > >(this),
		m_perspective(toCopy->m_perspective)
	{
		CtorCommon();
		this->m_contents->Import(toCopy->GetAllImplementation());
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
		return this->AddTo(
			Linear(CloneAndCast< CONTENT_TYPE >(content)),
			&this->m_contents
		);
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
		return LinearStructuralComponentImplementation< CONTENT_TYPE >::Insert(
			this->GetStructuralPerspective(),
			toAdd,
			this->GetAllImplementation(),
			position,
			optionalPositionArg,
			transferSubContents,
			this->GetLogEngine());
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
		return LinearStructuralComponentImplementation< CONTENT_TYPE >::FindByNameIn(
			this->GetStructuralPerspective(),
			this->GetAllImplementation(),
			name
		);
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
		return LinearStructuralComponentImplementation< CONTENT_TYPE >::FindByNameIn(
			this->GetStructuralPerspective(),
			this->GetAllImplementation(),
			name
		);
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
		return this->AddImplementation(
			(this->GetStructuralPerspective()->template GetTypeFromIdAs< CONTENT_TYPE >(id)));
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
		StandardDimension id = this->GetStructuralPerspective()->GetIdFromName(name);
		CONTENT_TYPE ret = this->GetByNameImplementation(
			name
		);
		if (ret)
		{
			return ret;
		}
		return this->CreateImplementation(id);
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
		BIO_ASSERT(Cast<Substance*>(&ct) != NULL);
		#endif
	}

};
} //chemical namespace
} //bio namespace
