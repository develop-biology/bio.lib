/*
 * This file is a part of the Biology project by eons LLC. <BR />
 * Biology (aka Develop Biology) is a framework for approaching software <br />
 * development from a natural sciences perspective. <br />
 *
 * Copyright (C) 2022 Séon O'Shannon & eons LLC <BR />
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

#include "Dendrite.h"
#include "Axon.h"

namespace bio {
namespace neural {

class Neuron;

/**
 * Synapses take data from one Neuron, the "presynapticNeuron", and make it available to another Neuron, the "postsynapticNeuron". <br />
 * Synapses are Cells. This is unusual. However, if we think of the proteins within the synaptic cleft as a specialized & distinct unit, this starts to make sense. The only major difference then is the lack of a membrane around the synaptic cleft. <br />
 * So, in our Biology framework, Synapses have their own Genes, Proteins, local Dissolved Substances, and can be specialized just like any other Cell. <br />
 * <br />
 * In order to remove data that was previously added while simultaneously allowing more data to be pushed through *this, a history of the data in *this must be kept. <br />
 * This history need only be a single value (i.e. not a complete history). <br />
 * The reason for this is that if a new datum is pushed through this that would overwrite data that has not been added, the previous, unadded data are no longer useful and can be freely overwritten. <br />
 * Similarly, if the old datum always represents what is currently added to to the postsynapticNeuron, there is no need to maintain a history of more than this single datum. <br />
 * For example a Synapse that passes a float might have: float mUnprocessed float mProcessed This type of 2 value system will be a common motif among bio::Synapses. The unprocessed, or "new", value is a value from the presynapticNeuron that has not yet been added  to the postsynapticNeuron. Once this value is added, it becomes processed, or "old". <br />
 * In general, Synapses look like: [SOURCE]----[*this]---->[TARGET] <br />
 * When sending data, [SOURCE]-->ProcessOutgoing()-->Update() [SOURCE]-->DepotentiateSignal() (totally optional) <br />
 * When receiving data, ProcessPotentiation()-->[TARGET] (postsynapticNeuron calls ProcessPotentiation, which affects ProcessDepotentiation()-->[TARGET] the postsynapticNeuron in some way) <br />
 * <br />
 * NOTE: Creating synapses outside of Neuron::ConnectTo(...) is not supported. <br />
 * <br />
 * To use *this between any pair of neurons, use ConfigureFor() (below). <br />
*/
class Synapse :
	public neural::Class< Synapse >,
	public Dendrite,
	public Axon
{
public:
	/**
	 * Disambiguate all class methods. <br />
	 */
	BIO_DISAMBIGUATE_ALL_CLASS_METHODS(neural, Synapse)

	BIO_DEFAULT_IDENTIFIABLE_CONSTRUCTORS(
		neural,
		Synapse,
		filter::Neural()
	)

	/**
	 * @param rhs
	 */
	Synapse(const Synapse& rhs);

	/**
	 *
	 */
	virtual ~Synapse();

	/**
	 * Use this method to populate any member variable Protein*s. <br />
	 * You'll want to do this to speed up your code by bypassing the dynamic execution provided by genetic::Expressor. <br />
	 */
	virtual Code CacheProteins();

	/**
	 * If you use CacheProteins, you'll likely want to create your default Proteins here. <br />
	 * This will prevent dereferencing null or garbage pointers when using your cached Proteins. <br />
	 */
	virtual Code CreateDefaultProteins();

	/**
	 * This should be called with ProcessOutgoing <br />
	 * When this is called, ShouldBeProcessed will return true on or after addDatumAt and before addDatumAt + timeout See ShouldBeProcessed for more info. <br />
	 * IMPORTANT: when overriding this, make sure to call this method too! <br />
	 * @param whenToPotentiate the time at which *this should be added to the postsynapticNeuron
	 */
	virtual void Update(Timestamp whenToPotentiate);

	
	/**
	 * ConfigureFor is used by Neuron::ConnectTo to clone *this such that it may be added between the given presynapticNeuron and postsynapticNeuron <br />
	 * This will call postsynapticNeuron->AddDendrite() <br />
	 * @param presynapticNeuron the Neuron that will send data to the new Neurite
	 * @param postsynapticNeuron the Neuron that will receive data from the new Neurite
	 * @return a Clone of *this but with the given postsynapticNeuron
	 */
	Synapse* ConfigureFor(
		const Neuron* presynapticNeuron,
		Neuron* postsynapticNeuron
	) const;


	/**
	 * PROTEIN BASED. <br />
	 * Called when *this is configured for use between the two given neurons. <br />
	 * This will be called on a newly created Clone() of *this. <br />
	 * @param presynapticNeuron the Neuron that will be sending data through *this. <br />
	 * @param postsynapticNeuron the Neuron that will be receiving data through *this. <br />
	 */
	virtual void AdditionalConfiguration(
		const Neuron* presynapticNeuron,
		Neuron* postsynapticNeuron
	);

	/**
	 * This method does not need to be overwritten. See the IsZero, above. <br />
	 * @return true if *this should be added at the current time, and the value of the data are >0 || AreZerosAllowed is true. false otherwise
	 */
	virtual bool ShouldBePotentiated() const;

	/**
	 * @return true if *this has expired.
	 */
	virtual bool ShouldBeDepotentiated() const;

	/**
	 * Sets the expiration time such that *this will expire at the given time. <br />
	 * Has no effect if the given time is later than the current time. <br />
	 * @param timeToDepotentiate the time that *this should expire at.
	 */
	virtual void ExtendTimeoutUntil(Timestamp timeToDepotentiate);

protected:
	molecular::Protein* mcAdditionalConfiguration;
};

} //neural namespace
} //bio namespace
