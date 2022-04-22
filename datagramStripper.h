/*
 * A model of the datagram stripper layer, their should 
 * only be one per complex host coupled model.
 * this model , is used to strip TCP header information, await a signal from the 
 * checksum validator. once received it should send the received data to the 
 * application layer
 * By		: Mohamed Mohamed Abd El-Salam Ahmed
 * Contact	: mohamedmahmed@islamway.net	
 **/
#ifndef __datagramStripper_H
#define __datagramStripper_H

#include "atomic.h"

class datagramStripper : public Atomic
{
	public:
		datagramStripper( const string &name = "datagramStripper" );
		virtual string className()const;
		double stripper(double data);
		double createAck(double data);
		bool checkAck(double data);
	protected:
		
		Model &initFunction();
		Model &externalFunction( const ExternalMessage &msg );
		Model &internalFunction( const InternalMessage & );
		Model &outputFunction( const InternalMessage &msg );
	private:
		//	Ports:
		const Port &in;			// input Data coming in, from the Internet layer. 
								// this should also receive Ack signals from the Internet layer.
		const Port &checkin;	// input data coming in, from the checksum validator
		//	output ports
		Port &receiveAck;			// a port to signal Ack to the DatagramCreator.
		Port &datagramStripperOut;	// output value (data) to the Appliction layer.
		Port &sendAck;
		
// Time varialbes

		Time preparationTime;
		Time timeLeft;
		// placeholder for value inside model.
		double value, checksum, ackVar;	// ackvar is what we send as an ack.
		bool flag, check, ackflag;
		// values to be used later on to create a connection manager.
		int sequence, acknowledge;
};
		//	inline methods
inline
string datagramStripper::className()const
{
	return "datagramStripper";
}

#endif	//__datagramStripper_H