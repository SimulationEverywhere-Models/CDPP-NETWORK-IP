/*
 * A model of the checksumValidator layer, their should 
 * only be one per complex host coupled model.
 * this model is used to validate checksum information of TCP
 * By		: Mohamed Mohamed Abd El-Salam Ahmed
 * Contact	: mohamedmahmed@islamway.net	
 **/
#ifndef __checksumValidator_H
#define __checksumValidator_H

#include "atomic.h"

class checksumValidator : public Atomic
{
	public:
		checksumValidator( const string &name = "checksumValidator" );
		virtual string className()const;
		int validator(double data);
	protected:
		
		Model &initFunction();
		Model &externalFunction( const ExternalMessage &msg );
		Model &internalFunction( const InternalMessage & );
		Model &outputFunction( const InternalMessage &msg );
	private:
		//	Ports:
		const Port &in;	// input Data coming in, 
		//	output ports
		Port &checksumValidatorOut;	// checksum value going to the datagram maker.
		
// Time varialbes

		Time preparationTime;
		Time timeLeft;
		// placeholder for value inside model.
		double value;
};
		//	inline methods
inline
string checksumValidator::className()const
{
	return "checksumValidator";
}

#endif	//__checksumValidator_H