#ifndef __physical_H
#define __physical_H

#include "atomic.h"

class physical : public Atomic
{
	public:
		physical( const string &name = "physical" );
		virtual string className()const;
	protected:
		
		Model &initFunction();
		Model &externalFunction( const ExternalMessage &msg );
		Model &internalFunction( const InternalMessage & );
		Model &outputFunction( const InternalMessage &msg );
	private:
		// Ports:
		const Port &in;	// in == input from host 1
		const Port &in1;	// in1== input from host 2
		const Port &sensingPort;
		const Port &type;
		Port &signal;		
		Port &out;		// out to host 1, should be connected to in1
		Port &out1;		// out to host 2, should be connected to in
		
		// Time variables
		Time preparationTime;
		Time timeLeft, delay, delayFactor, timeFirst, timeSecond;
		// placeholder for value inside model.
		double localValue, localValue1, sensor;	
		double linktype;	
		bool send, jamming, sendSensor, Port_1; 
		bool Firstsignal; 	 // when the model is initialized this is true
					 // when it turns into false then we have a second entry of data 	
};

		//	inline methods
inline
string physical::className()const
{
	return "physical";
}

#endif	//__physical_H
