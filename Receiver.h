#ifndef __Receiver_H
#define __Receiver_H

#include "atomic.h"

class Receiver : public Atomic
{
	public:
		Receiver( const string &name = "Receiver" );
		virtual string className()const;
		bool verify(double value);
		int define(double value);
	protected:
		
		Model &initFunction();
		Model &externalFunction( const ExternalMessage &msg );
		Model &internalFunction( const InternalMessage & );
		Model &outputFunction( const InternalMessage &msg );
		
	private:
		
		// Ports:
		const Port &ingress;		// input port, for the Receiver Module, will receive data from DATA link layer 
		const Port &sip;			// to receive own ip on
		Port &egress;				// output port from network to Transport
		
		// Time varialbes
		//---------------------------------------------------------------------
		Time preparationTime;
		Time timeLeft;
		//---------------------------------------------------------------------
		// placeholder for value inside model.
		//---------------------------------------------------------------------
		int valueCounter; 
		double localValue;	// will be used to hold IP Options data field
		double localValue2;	// will be used to hold IP Source IP
		double localValue3;	// will be used to hold IP Destination IP
		double localValue4;	// will be used to hold IP Data (TCP Datagram.)
		//----------------------------------------------------------------------
		double sourceIP;	// self IP Receiver	< we can either receive an IP, or use the model file to send the addressing info.
		double networkAddress, hostAddress, mask;	// variables used for model file addressing.
		bool send;		// a flag to signal that we are ready to send data.
		bool receive;		// thus we have received all data.
};

		//	inline methods
inline
string Receiver::className()const
{
	return "Receiver";
}

#endif	//__Receiver_H
