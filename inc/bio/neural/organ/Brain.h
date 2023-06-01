/*
 * This file is a part of the Biology project by eons LLC. <br />
 * Biology (aka Develop Biology) is a framework for approaching software <br />
 * development from a natural sciences perspective. <br />
 *
 * Copyright (C) 2022 Séon O'Shannon & eons LLC <br />
 *
 * This program is free software: you can redistribute it and/or modify <br />
 * it under the terms of the GNU Affero General Public License as <br />
 * published by the Free Software Foundation, either version 3 of the <br />
 * License, or (at your option) any later version. <br />
 *
 * This program is distributed in the hope that it will be useful, <br />
 * but WITHOUT ANY WARRANTY; without even the implied warranty of <br />
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the <br />
 * GNU Affero General Public License for more details. <br />
 *
 * You should have received a copy of the GNU Affero General Public License <br />
 * along with this program.  If not, see <https://www.gnu.org/licenses/>. <br />
 */

#pragma once

#include "bio/neural/common/Class.h"
#include "bio/neural/common/Types.h"
#include "bio/neural/common/Filters.h"
#include "bio/neural/macro/Macros.h"
#include "bio/cellular/Organ.h"

namespace bio {
namespace neural {

/**
 * The Brain provides an easy to use interface for creating an intelligent creature. <br />
 * There is only one Brain per creature but multiple creatures may use the same Brain configuration (but not instance). <br />
 * The Brain holds all the Neurons needed to handle inputs, outputs, and other operations within the neural framework for your project. <br />
 * It is also responsible for instantiating, connecting, and configuring them. <br />
 */
class Brain:
	public neural::Class< Brain >,
	public cellular::Organ
{
public:

	/**
	 * Disambiguate all the Class methods. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(neural, Brain)

	/**
	 * Default constructors. <br />
	 */
	BIO_DEFAULT_IDENTIFIABLE_CONSTRUCTORS(
		neural,
		Brain,
		filter::Neural()
	)

	/**
	 *
	 */
	virtual ~Brain();


	/**
	 * PROTEIN BASED. <br />
	 * Activate anything necessary before setting up the Neural Framework. <br />
	 * IMPORTANT: when overriding this method, don't forget to call the parent's method too! <br />
	 * @return a code
	 */
	virtual Code PreSetup();
	
	/**
	 * PROTEIN BASED. <br />
	 * As an Organ, the Brain operates on Tissues, not individual Cells. As a result, you must create the Neuropils & other Tissues you need before attempting to Add< cellular:Cell* >s within *this. <br />
	 * IMPORTANT: when overriding this method, don't forget to call the parent's method too! <br />
	 * @return a code
	 */
	virtual Code CreateNeuropils();

	/**
	 * PROTEIN BASED. <br />
	 * Creates all the Synapse*s that will be used within CreateNeuralFramework. <br />
	 * Plasmids or other systems that define Synapse features may add their own Proteins to this method that create Synapses for later use. If this is done, the IDs of the created Synapses should be documented or otherwise defined in the plasmid, etc. <br />
	 * If you wish to define your own Synapses, override this method and use the CreateSynapse method to get a base Synapse which can be configured to your liking. OR Create a child class of Synapse and new it within this method. <br />
	 * You should add all created Synapses to a Neuropil (or Tissue) in *this with Add< cellular::Cell* >(YOUR_SYNAPSE) (Synapses are Cells). <br />
	 * IMPORTANT: when overriding this method, don't forget to call the parent's method too! <br />
	 * @return a code 
	 */
	virtual Code CreateSynapses();

	/**
	 * PROTEIN BASED. <br />
	 * Create all Neurons that will be used in CreateNeuralFramework. <br />
	 * This is also a good place to create any additional Neuropils that your Neurons will need. <br />
	 * Please don't name any 2 Neurons the same name. <br />
	 * Same with Neuropils & all Identifiable< Id > children. <br />
	 * Any duplicate names will just add confusion to your programs and introduce potential points of error, if not outright failure. <br />
	 * You should add all created Neurons to a Neuropil (or Tissue) in *this with Add< cellular::Cell* >(YOUR_NEURON). <br />
	 * IMPORTANT: when overriding this method, don't forget to call the parent's method too! <br />
	 * @return a code
	 */
	virtual Code CreateNeurons();

	/**
	 * PROTEIN BASED. <br />
	 * Setup the Neural Framework. <br />
	 * This is where all Synapses should be made and where most configuration should happen. <br />
	 * Helper functions for configuring or otherwise setting up Neurons, Synapses, or any other part of the brain or Neural Framework should be called here. <br />
	 * IMPORTANT: when overriding this method, don't forget to call the parent's method too! <br />
	 * @return a code
	 */
	virtual Code CreateNeuralFramework();

	/**
	 * PROTEIN BASED. <br />
	 * Do anything that must be done to ensure smooth operation of the Neural Framework. <br />
	 * This step is where most pre-start, post-init cleanup would take place. <br />
	 * IMPORTANT: when overriding this method, don't forget to call the parent's method too! <br />
	 * @return a code
	 */
	virtual Code PostSetup();

	/**
	 * Override of genetic::Expressor method; see that class for more details. <br>
	 */
	virtual Code CacheProteins();

	/**
	 * Override of genetic::Expressor method; see that class for more details. <br>
	 */
	virtual Code CreateDefaultProteins();

	/**
	 * Override of Organ method; see that class for more details. <br>
	 * @return whether or not *this has grown all the Tissues it needs to function
	 */
	virtual Code GrowTissues()
	{
		Code code;
		code = PreSetup();
		BIO_SANITIZE(code == code::Success() || code == code::NoErrorNoSuccess(), , return code)

		code = CreateNeuropils();
		BIO_SANITIZE(code == code::Success() || code == code::NoErrorNoSuccess(), , return code)

		code = CreateSynapses();
		BIO_SANITIZE(code == code::Success() || code == code::NoErrorNoSuccess(), , return code)

		code = CreateNeurons();
		BIO_SANITIZE(code == code::Success() || code == code::NoErrorNoSuccess(), , return code)

		code = CreateNeuralFramework();
		BIO_SANITIZE(code == code::Success() || code == code::NoErrorNoSuccess(), , return code)

		code = PostSetup();
		BIO_SANITIZE(code == code::Success() || code == code::NoErrorNoSuccess(), , return code)

		return Organ::GrowTissues();
	}

protected:
	molecular::Protein* mcPreSetup;
	molecular::Protein* mcCreateNeuropils;
	molecular::Protein* mcCreateSynapses;
	molecular::Protein* mcCreateNeurons;
	molecular::Protein* mcCreateNeuralFramework;
	molecular::Protein* mcPostSetup;

};

} //namespace neural
} //bio namespace
