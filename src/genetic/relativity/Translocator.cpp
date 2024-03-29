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


#include "bio/genetic/relativity/Translocator.h"

namespace bio {
namespace genetic {

TranslocatorImplementation::TranslocatorImplementation()
{
    
}
TranslocatorImplementation::~TranslocatorImplementation()
{
    
}
bool TranslocatorImplementation::AssociateSignalPeptidase(
	Location location,
	Epitope epitope,
	chemical::ExcitationBase* peptidase
)
{
	BIO_SANITIZE(peptidase,,return false)
	SignalPeptide* signal = GetBraneAs< SignalPeptide* >(location);
	BIO_SANITIZE(signal,,return false)
	Epitope peptidaseId = signal->mPeptidases.GetIdFromName(SafelyAccess< EpitopePerspective >()->GetNameFromId(epitope));
	return signal->mPeptidases.AssociateType(peptidaseId, peptidase->AsWave());
}

bool TranslocatorImplementation::DisassociateSignalPeptidase(
	Location location,
	Epitope epitope
)
{
	SignalPeptide* signal = GetBraneAs< SignalPeptide* >(location);
	BIO_SANITIZE(signal,,return false)
	Epitope peptidaseId = signal->mPeptidases.GetIdFromName(SafelyAccess< EpitopePerspective >()->GetNameFromId(epitope));
	return signal->mPeptidases.DisassociateType(peptidaseId);
}

chemical::ExcitationBase* TranslocatorImplementation::GetPeptidase(
	Location location,
	Epitope epitope
)
{
    SignalPeptide* signal = GetBraneAs< SignalPeptide* >(location);
	BIO_SANITIZE(signal,,return NULL)
	Epitope peptidaseId = signal->mPeptidases.GetIdFromName(SafelyAccess< EpitopePerspective >()->GetNameFromId(epitope));
	return signal->mPeptidases.template GetNewObjectFromIdAs< chemical::ExcitationBase* >(peptidaseId);
}

chemical::ExcitationBase* TranslocatorImplementation::GetPeptidase(
	Location location,
	const Name& epitope
)
{
	return GetPeptidase(location, EpitopePerspective::Instance().GetIdFromName(epitope));
}

physical::Brane< Location >* TranslocatorImplementation::CreateBrane(Location id, const Name& name)
{
    return new SignalPeptide(id, name);
}

} //genetic namespace
} //bio namespace
