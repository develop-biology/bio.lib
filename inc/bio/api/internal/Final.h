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

#include "bio/chemical/Substance.h"

namespace bio {

/**
 * Final classes strip away inheritance, removing the need to disambiguate virtual methods in every derivation. <br />
 * Derive from a Final<> class if you don't want you downstream clients to necessarily override Biology functions. <br /> BIO_FINAL
 * In other words, this is where methods come to an end. From here on out (i.e. where you chose to use this), you will no longer be able to combine Biology classes. <br /> BIO_FINAL
 * This inhibition of extensibility is done to remove unnecessary impositions on downstream consumers of your code (i.e. it makes your code cleaner). <br />
 * Thus, only use Final<> if and only if you are USING rather than EXTENDING this library. <br />
 * <br />
 * NOTE: CLASS MUST be a child of chemical::Substance. This will be true for all classes users (and not developers) are likely to want.
 */
template < class CLASS>
class Final :
	public TransparentWrapper< CLASS >
{
public:

	/**
	 *
	 */
	Final() :
		TransparentWrapper< CLASS >(CLASS())
	{
	}

	/**
	 * @param name
	 */
	explicit Final(const Name& name):
		TransparentWrapper< CLASS >(CLASS(name))
	{
	}

	/**
	 * @param id
	 */
	explicit Final(const Id& id):
		TransparentWrapper< CLASS >(CLASS(id))
	{
	}

	/**
	 *
	 */
	~Final() 
	{

	}
	
	//START: operators

	operator Id() const
	{
		return this->mT.operator Id();
	}

	operator physical::Wave*() 
	{
		return this->mT.operator physical::Wave*();
	}
	
	template < typename AS >
	operator AS()
	{
		return this->mT.template As< AS >();
	}
	
	void operator|(physical::Symmetry* symmetry) 
	{
		return this->mT.operator|(symmetry);
	}
	
	physical::Wave* operator*(physical::Wave* signal) 
	{
		return this->mT.operator*(signal);
	}
	
	physical::Wave* operator*() 
	{
		return this->mT.operator*();
	}
	
	const physical::Wave* operator*() const 
	{
		return this->mT.operator*();
	}

	void operator+(const physical::Wave* other) 
	{
		this->mT.operator+(other);
	}

	void operator-(const physical::Wave* other) 
	{
		return this->mT.operator-(other);
	}

	bool operator==(const Id id) const
	{
		return this->mT.operator==(id);
	}

	bool operator==(const Name& name) const
	{
		return this->mT.operator==(name);
	}

	bool operator==(const physical::Identifiable< Id >& other) const
	{
		return this->mT.operator==(other);
	}
	
	//END: operators

	//START: ...::Class<> methods

	physical::Wave* Clone() const 
	{
		return this->mT.Clone();
	}
	physical::Wave* AsWave() 
	{
		return this->mT.AsWave();
	}
	const physical::Wave* AsWave() const 
	{
		return this->mT.AsWave();
	}
	bool RegisterProperties(const Properties& properties)
	{
		return mT.RegisterProperties(properties);
	}
	Properties GetProperties() const 
	{
		return this->mT.GetProperties();
	}
	chemical::Atom* AsAtom() 
	{
		return this->mT.AsAtom();
	}
	const chemical::Atom* AsAtom() const 
	{
		return this->mT.AsAtom();
	}
	physical::Wave* Modulate(physical::Wave* signal) 
	{
		return this->mT.Modulate(signal);
	}
	physical::Wave* Demodulate() 
	{
		return this->mT.Demodulate();
	}
	const physical::Wave* Demodulate() const 
	{
		return this->mT.Demodulate();
	}
	Code Attenuate(const physical::Wave* other) 
	{
		return this->mT.Attenuate(other);
	}
	Code Disattenuate(const physical::Wave* other) 
	{
		return this->mT.Disattenuate(other);
	}

	//END: ...::Class<> methods

	//START: chemical::UnorderedStructureInterface methods

	template < typename T >
	T Add(const T t)
	{
		return this->mT.template Add< T >(t);
	}

	template < typename T >
	T Remove(const T t)
	{
		return this->mT.template Remove< T >(t);
	}

	template < typename T >
	void Import(const chemical::UnorderedMotif< T >* other)
	{
		return this->mT.template Import< T >(other);
	}

	template < typename T >
	void Import(const ::bio::Arrangement< T >& other)
	{
		return this->mT.template Import< T >(other);
	}

	Code ImportAll(const physical::Wave* other)
	{
		return this->mT.ImportAll(other);
	}

	template < typename T >
	Index GetCount() const
	{
		return this->mT.template GetCount< T >();
	}

	template < typename T >
	Container* GetAll()
	{
		return this->mT.template GetAll< T >();
	}

	template < typename T >
	const Container* GetAll() const
	{
		return this->mT.template GetAll< T >();
	}

	template < typename T >
	bool Has(T content) const
	{
		return this->mT.template Has< T >(content);
	}

	template < typename T >
	unsigned int GetNumMatching(const Container* other) const
	{
		return this->mT.template GetNumMatching< T >(other);
	}

	template < typename T >
	bool HasAll(const Container* contents) const
	{
		return this->mT.template HasAll< T >(contents);
	}

	template < typename T >
	void Clear()
	{
		return this->mT.template Clear< T >();
	}

	template < typename T >
	std::string GetStringFrom(std::string separator = ", ")
	{
		return this->mT.template GetStringFrom< T >(separator);
	}

	template < typename T >
	std::vector< T > GetAllAsVector()
	{
		return this->mT.template GetAllAsVector< T >();
	}

	template < typename T >
	const ::std::vector< T > GetAllAsVector() const
	{
		return this->mT.template GetAllAsVector< T >();
	}

	//END: chemical::UnorderedStructureInterface methods

	//START: chemical::LinearStructureInterface methods

	template < typename T >
	Code Insert(
		T toAdd,
		const Position position = BOTTOM,
		const Id optionalPositionArg = 0,
		const bool transferSubContents = false
	)
	{
		return this->mT.template Insert< T >(
			toAdd,
			position,
			optionalPositionArg,
			transferSubContents
		);
	}

	template < typename T >
	T GetById(const Id& id)
	{
		return this->mT.template GetById< T >(id);
	}

	template < typename T >
	const T GetById(const Id& id) const
	{
		return this->mT.template GetById< T >(id);
	}

	template < typename T >
	T GetByName(const Name& name)
	{
		return this->mT.template GetByName< T >(name);
	}

	template < typename T >
	const T GetByName(const Name& name) const
	{
		return this->mT.template GetByName< T >(name);
	}

	template < typename T >
	T GetOrCreateById(const Id& id)
	{
		return this->mT.template GetOrCreateById< T >(id);
	}

	template < typename T >
	T GetOrCreateByName(const Name& name)
	{
		return this->mT.template GetOrCreateByName< T >(name);
	}

	template < typename T >
	chemical::Emission ForEach(chemical::ExcitationBase* excitation)
	{
		return this->mT.template ForEach< T >(excitation);
	}

	//END: chemical::LinearStructureInterface methods

	//START: chemical::Substance methods

	void Enable() 
	{
		return this->mT.Enable();
	}
	void Disable() 
	{
		return this->mT.Disable();
	}
	bool IsEnabled() const 
	{
		return this->mT.IsEnabled();
	}

	//END: chemical::Substance methods

	//START: chemical::Atom methods

	template < typename T >
	T As()
	{
		return this->mT.template As< T >();
	}

	template < typename T >
	const T As() const
	{
		return this->mT.template As< T >();
	}

	template < typename T >
	bool FormBond(
		T toBond,
		BondType type = bond_type::Unknown())
	{
		return this->mT.template FormBond< T >(toBond, type);
	}

	template < typename T >
	bool BreakBond(
		T toDisassociate,
		BondType type = bond_type::Unknown())
	{
		return this->mT.template BreakBond< T >(toDisassociate, type);
	}

	chemical::Bonds* GetAllBonds()
	{
		return this->mT.GetAllBonds();
	}

	const chemical::Bonds* GetAllBonds() const
	{
		return this->mT.GetAllBonds();
	}

	//END: chemical::Atom methods

	//START: log::Writer methods

	void Log(
		LogLevel level,
		const char* format,
		...
	) const
	{
		va_list args;
		va_start(args, format);
		this->mT.Log(level, format, args);
		va_end(args);
	}

	void ExternalLog(
		Filter logFilter,
		LogLevel level,
		const char* format,
		...
	) const
	{
		va_list args;
		va_start(args, format);
		this->mT.ExternalLog(logFilter, level, format, args);
		va_end(args);
	}

	void SetLogEngine(::bio::log::Engine* logEngine)
	{
		this->mT.SetLogEngine(logEngine);
	}

	::bio::log::Engine* GetLogEngine()
	{
		return this->mT.GetLogEngine();
	}

	const ::bio::log::Engine* GetLogEngine() const
	{
		return this->mT.GetLogEngine();
	}

	bool HasLogEngine() const
	{
		return this->mT.HasLogEngine();
	}

	//END: log::Writer methods

	//START: physical::Identifiable< Id > methods

	Name GetName() const 
	{
		return this->mT.GetName();
	}

	Id GetId() const
	{
		return this->mT.GetId();
	}

	void SetName(const Name& name)
	{
		return this->mT.SetName(name);
	}

	void SetId(const Id& id)
	{
		return this->mT.SetId(id);
	}
	
	bool IsName(const Name& name) const
	{
		return this->mT.IsName(name);
	}
	
	bool IsNameInsensitive(const Name& name) const
	{
		return this->mT.IsNameInsensitive(name);
	}
	
	bool IsId(const Id& id) const
	{
		return this->mT.IsId(id);
	}
	
	void SetPerspective(physical::Perspective< Id >* perspective)
	{
		return this->mT.SetPerspective(perspective);
	}
	
	void MakeWave(bool force = false) 
	{
		return this->mT.MakeWave();
	}

	physical::Perspective< Id >* GetPerspective() const
	{
		return this->mT.GetPerspective();
	}

	//END: physical::Identifiable< Id > methods

	//START: physical::Wave methods (some of these defined above).

	physical::Symmetry* Spin() const 
	{
		return this->mT.Spin();
	}

	Code Reify(physical::Symmetry* symmetry) 
	{
		return this->mT.Reify(symmetry);
	}

	//END: physical::Wave methods
};

} //bio namespace
