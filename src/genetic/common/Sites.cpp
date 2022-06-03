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

#include "bio/genetic/common/Sites.h"
#include "bio/genetic/macros/Macros.h"
#include "bio/molecular/Surface.h"
#include "bio/molecular/Molecule.h"
#include "bio/molecular/Vesicle.h"
#include "bio/chemical/reaction/Excitation.h"
#include "bio/chemical/structure/motif/LinearMotif.h"
#include "bio/physical/Periodic.h"

namespace bio {

//BIO_SITE_FUNCTION_BODY(InSurface,
//	molecular::Molecule*)


namespace site {
::bio::Site InSurfaceLocalizationSite()
{
	static ::bio::CachedId< ::bio::Site > sInSurfaceLocalizationSite(
		"InSurfaceLocalizationSite",
		::bio::LocalizationSitePerspective::Instance());
	return sInSurfaceLocalizationSite;
}
bool gInSurfaceLocalizationSiteRegistered = SafelyAccess< ::bio::LocalizationSitePerspective >()->AssociateType(
	::bio::LocalizationSitePerspective::Instance().GetIdFromName("InSurfaceLocalizationSite"),
	(new ::bio::chemical::Excitation< ::bio::chemical::LinearMotif< molecular::Molecule* >, molecular::Molecule*, const Name& >(
		&::bio::chemical::LinearMotif< molecular::Molecule* >::GetByNameImplementation,
		__null
	))->AsWave());
::bio::Site InSurfaceInsertionSite()
{
	static ::bio::CachedId< ::bio::Site > sInSurfaceInsertionSite(
		"InSurfaceInsertionSite",
		::bio::InsertionSitePerspective::Instance());
	return sInSurfaceInsertionSite;
}
bool gInSurfaceInsertionSiteRegistered = SafelyAccess< ::bio::InsertionSitePerspective >()->AssociateType(
	::bio::InsertionSitePerspective::Instance().GetIdFromName("InSurfaceInsertionSite"),
	(new ::bio::chemical::Excitation< ::bio::chemical::LinearMotif< molecular::Molecule* >, molecular::Molecule*, molecular::Molecule* >(
		&::bio::chemical::LinearMotif< molecular::Molecule* >::AddImplementation,
		__null
	))->AsWave());
}

BIO_SITE_FUNCTION_BODY(OnMolecule,
	molecular::Surface*)

BIO_SITE_FUNCTION_BODY(InVesicle,
	molecular::Molecule*)

BIO_SITE_FUNCTION_BODY(ToVesicle,
	molecular::Vesicle*)

} //bio namespace
