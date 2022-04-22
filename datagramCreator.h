/*
 * A model of the datagramCreator layer, their should 
 * only be one per complex host coupled model.
 * this model is only interested in datagramCreators running TCP
 * By		: Mohamed Mohamed Abd El-Salam Ahmed
 * Contact	: mohamedmahmed@islamway.net	
 **/
#ifndef __datagramCreator_H
#define __datagramCreator_H

#include "atomic.h"
#include <list>

class datagramCreator : public Atomic
{
	public:
		datagramCreator( const string &name = "datagramCreator" );
		double creator(double data);
		virtual string className()const;
	protected:
		
		Model &initFunction();
		Model &externalFunction( const ExternalMessage &msg );
		Model &internalFunction( const InternalMessage & );
		Model &outputFunction( const InternalMessage &msg );
	private:
		//	Ports:
		const Port &in;	// input Data coming in, from the application layer.
		const Port &checkin;
		const Port &ackPort;
		const Port &ackSender;
		//	output ports
		Port &datagramCreatorOut;	// checksum value going to the datagram maker.
		Port &resendPort;			// a port to signal an old data being resend.
		Port &gocheck;				// port to send data to the checksumCreator.
		// Time varialbes

		Time preparationTime;
		Time timeLeft;
		
		// placeholder for value inside model.
		
		double value, checksumValue, ackValue, ackVariable;
		bool sendflag, ackflag, sendAck, checkflag;
		int sequence, acknowledge, window;
		// variable used to accomodate the connection manager.
		typedef list<double> PacketList;
		PacketList list1;
};
		//	inline methods
inline
string datagramCreator::className()const
{
	return "datagramCreator";
}

#endif	//__datagramCreator_H