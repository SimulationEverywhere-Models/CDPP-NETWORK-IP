/*******************************************************************
*
*  DESCRIPTION: Atomic Model PacketProcessor
*
*  AUTHOR: Khalil Yonis 
*
*  EMAIL: 
*
*  DATE: 26/11/2003
*
*******************************************************************/

#ifndef __PACKETPROCESSOR_H
#define __PACKETPROCESSOR_H


#include "atomic.h"     // class Atomic

class PacketProcessor : public Atomic
{
public:
	PacketProcessor( const string &name = "PacketProcessor" );	//Default constructor

	virtual string className() const ;
protected:
	Model &initFunction();
	Model &externalFunction( const ExternalMessage & );
	Model &internalFunction( const InternalMessage & );
	Model &outputFunction( const InternalMessage & );

private:
	const Port &in;			//to receive the id of the interface holding the packet
	const Port &outInterface;//to receive the output interface to forward the packet.
	const Port &packet;		//to receive the packet.
	const Port &cont;		//to receive confirm that the request was completed
	Port &destination;		//to send the distination to table to get the output interface if packet is data
	Port &updateTable;		//to send the update information to the table if packet is an update
	Port &requ;			//to send a request signal if the packet is a request.
	Port &getPacket;		//to signal asking the in_out interface to send the ready packet.
	Port &out;			//to forward the packet through.
	Port &next;			//signal the queue to send next value.
	Time preparationTime;
	Time timeLeft;
	
	int rcv;
	enum outState {unknown, request, response, data, forward, sendRequest};
	outState os; // this will hold the state of the output to determain which port to be used

	double inId;	// holds the input interface that received the packet
	double v1;		// part of the packet, holds teh type, metric, and some options
	double v3;		// part of the packet, holds the distination address.
	double v2;		// part of the packet, holds the source address,
	double v4;		// part of the packet, hold the TCP options.
	double outId;	// holds the id for the output interface.

};	// class PacketProcessor

// ** inline ** // 
inline
string PacketProcessor::className() const
{
	return "PacketProcessor" ;
}

#endif   //__PacketProcessor_H
