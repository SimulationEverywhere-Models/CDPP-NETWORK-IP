/*******************************************************************
*
*  DESCRIPTION: Atomic Model RouterIn
*
*  AUTHOR: Kalil Yonis 
*
*  EMAIL: 
*
*  DATE: 27/12/2003
*
*******************************************************************/

#ifndef __ROUTERIN_H
#define __ROUTERIN_H

#include <list>
#include "atomic.h"     // class Atomic
//#include "Packet.h"		//class Packet

class RouterIn : public Atomic
{
public:
	RouterIn( const string &name = "RouterIn" );//Default constructor

	virtual string className() const ;
protected:
	Model &initFunction();
	Model &externalFunction( const ExternalMessage & );
	Model &internalFunction( const InternalMessage & );
	Model &outputFunction( const InternalMessage & );

private:
	const Port &in;		//for receiving packets
	const Port &ready;	//signals the model to send a packet from the packet list.
	Port &to_RPU;			//for outputting the packets upon receiving a signal on the "ready" port.
	Port &flag;			//for signaling every time a packet is ready in the packet list.
	Port &interfaceNum;	//to send the interface number to other "in_out" coupled model componants.
	Time preparationTime;

	struct Packet		//this structure will represent a hole packet that gets stored in the list
	{
		double IP_options;
		double destination;
		double source;
		double TCP_layer;
	};
	typedef list<Packet> ElementList;	//creating a list to hold the received packets.
	ElementList elements ;

	enum rcvState {nothing, got1, got2, got3, gotAll};	//packet receiving states;
	rcvState rs; // holds the state that the queue is at in receiving the packet.
	
	double v1, v2, v3, v4;	//to temporarily hold the values representing the packet 
	double myid;				// to hold the value representing the input inerface. (used to identefy the in_out coupled model componants).
	int outputFlag;		// 0= nothing, 1= flag the pocessor that a packet is ready, 2 output a packet
	Time timeLeft;

};	// class Queue

// ** inline ** // 
inline
string RouterIn::className() const
{
	return "RouterIn" ;
}

#endif   //__PROUTERIN_H
