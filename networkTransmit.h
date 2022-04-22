#ifndef __networkTransmit_H
#define __networkTransmit_H

#include "atomic.h"

class networkTransmit : public Atomic
{
	public:
		networkTransmit( const string &name = "networkTransmit" );
		virtual string className()const;
		double headerMaker();
	protected:
		
		Model &initFunction();
		Model &externalFunction( const ExternalMessage &msg );
		Model &internalFunction( const InternalMessage & );
		Model &outputFunction( const InternalMessage &msg );
	private:
		// Ports:
		const Port &ingress;		// input port, for the networkTransmit networkTransmit Module 
		const Port &sip;		// to receive source ip on
		const Port &dip;		// to receive destination ip on
		const Port &resend;		// to receive resend requests on.
		Port &egress;			// output port
		
		// Time varialbes
		Time preparationTime;
		Time timeLeft;
		// placeholder for value inside model.
		double localValue;	// will be used to hold TCP data
		double sourceIP;	// self networkTransmit
		double destinationIP;	// networkTransmit to send Data to.
		bool send, receive, update, trueModel;	// a flag to signal that we are ready to send data.
		double resendIP;
		
		//--------------------------------------------------------------------------------
		double IP, networkAddress, hostAddress;// Mask;	// to receive data from the model file in.
};

		//	inline methods
inline
string networkTransmit::className()const
{
	return "networkTransmit";
}

#endif	//__networkTransmit_H
