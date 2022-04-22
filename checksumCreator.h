/*
 * A model of the checksumCreator layer, their should 
 * only be one per complex host coupled model.
 * this model is only interested in checksumCreators running TCP
 * By		: Mohamed Mohamed Abd El-Salam Ahmed
 * Contact	: mohamedmahmed@islamway.net	
 **/
#ifndef __checksumCreator_H
#define __checksumCreator_H

#include "atomic.h"

class checksumCreator : public Atomic
{
	public:
		checksumCreator( const string &name = "checksumCreator" );
		virtual string className()const;
		double checksum(double data);
	protected:
		
		Model &initFunction();
		Model &externalFunction( const ExternalMessage &msg );
		Model &internalFunction( const InternalMessage & );
		Model &outputFunction( const InternalMessage &msg );
	private:
		//	Ports:
		const Port &in;	// input Data coming in, from the application layer.
		//	output ports
		Port &checksumCreatorOut;	// checksum value going to the datagram maker.
		
// Time varialbes

		Time preparationTime;
		Time timeLeft;
		// placeholder for value inside model.
		double value;
};
		//	inline methods
inline
string checksumCreator::className()const
{
	return "checksumCreator";
}

#endif	//__checksumCreator_H