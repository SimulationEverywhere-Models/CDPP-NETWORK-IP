/*************************************************
 *	Description:	networkTransmit (Complex IP) Atomic Model
 *			A network layer simualtion for the Complex IP Model.
 *			the Transmit receives data from the TCP layer, generates the required header information
 *			and then sends them to the next layer of the Reference Model(Data Link)
 *	Created by:	Mohamed Mohamed Abd El-Salam Ahmed
 *			100302196.
 *	Email:		mohamedmahmed@islamway.net
 *************************************************/

#include "networkTransmit.h"
#include "message.h"
#include "mainsimu.h"
#include "iostream.h"
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


//	Constructor
networkTransmit::networkTransmit( const string &name )
: Atomic( name )
, ingress( addInputPort( "ingress" ) )		// data comes in on this port, from the transport layer
, sip( addInputPort( "sip" ) )			// source networkTransmit is defined through this port
, dip( addInputPort( "dip" ) )			// destination networkTransmit is found through this port
, egress( addOutputPort( "egress" ) )		// data is outputed through this port. to the Data link layer
, resend( addInputPort( "resend") )		// input port to receive resend requests on.
, preparationTime( 0, 0, 10, 0 )
{
	//string time( MainSimulator::Instance().getParameter( description(), "preparation" ) ) ;
	string ip( MainSimulator::Instance().getParameter( description(), "IP" ) ) ;
	cout<<ip<<endl;
	//if( time != "" )
		//preparationTime = time ;
	//if( ip != "")
	//	sourceIP = IP;
const char *c;  c = ip.c_str();
sourceIP = atof(c);
//sourceIP = g_strtod(IP, NULL);
cout<< (int)sourceIP/1000000<< (int)sourceIP%1000000<< endl;
}

	// Protected functions.

Model &networkTransmit::initFunction()
{
	update = true;
	//sourceIP = 0;
	destinationIP = 0;
	receive = false;
	send = false;
	holdIn(active, preparationTime);
	return *this;
}

//networkTransmit will receive data from the transport Layer and send it, to the data link layer
//after adding the neccessary headers

Model &networkTransmit::externalFunction( const ExternalMessage &msg )
{
	if( msg.port() == ingress )
	{
		localValue = msg.value();	// TCP message.
		//cout<< "output function network Transmit ingress\n";
		send = false;
	
		//holdIn( active, preparationTime);	// so that the output function is executed
		
	}

	if( msg.port() == sip)	// if we receive the addressing info through the model file then this is not needed.
	{
		sourceIP = msg.value();
		update = true;
	}

	if( msg.port() == dip)
	{
		destinationIP = msg.value();
		resendIP = msg.value();	// to save the ip we are sending on so as to resend.
		send = true;			// this way we can control when the host will send
								// data
		//cout<< "output function network Transmit DESTINATION\n";
		
		
	}
	
	if( msg.port() == resend)
	{
		// problem with resend is that the packet gets a new ID, this will work for simple retransmits.
		// however we will need to maintain the complete packet info for later on.
		localValue = msg.value();
		destinationIP = resendIP;
		send = true;
	}
	holdIn( active, preparationTime);
	return *this;
}

Model &networkTransmit::internalFunction( const InternalMessage & )
{
	passivate();
	return *this;
}

Model &networkTransmit::outputFunction( const InternalMessage &msg )
{
	if(send)
	{
		send = false;
		double options = headerMaker();
		//cout << "options sent::"<<options<<"\n";
		sendOutput( msg.time(), egress, options);	// 1
		//--------------------------------------------------
		cout << "source IP sent::"<<sourceIP<<"\n";
		sendOutput( msg.time(), egress, sourceIP);	// 2
		//--------------------------------------------------
		cout << "destination IP sent::"<<destinationIP<<"\n";
		sendOutput( msg.time(), egress, destinationIP);	// 3
		//--------------------------------------------------
		cout << "TCP data sent::"<<localValue<<"\n";
		sendOutput( msg.time(), egress, localValue);	// 4
	} else if (update)
		{
			cout<<"we have updated the router"<<endl;
			update = false;
			double updateField = 2000000;
			sendOutput( msg.time(), egress, updateField);
			sendOutput( msg.time(), egress, sourceIP);
			sendOutput( msg.time(), egress, 0);
			sendOutput( msg.time(), egress, 0);
		}

	return *this;
}

double networkTransmit::headerMaker()
{
	// step 1 networkTransmit options field, this field can be completed through user input.
	// for now (to test models interoprability) it will have preset values.
	int ttl = 15; 			// max life value for a packet according to the basic RnetworkTransmit algorithm.
	int fragOffset = 0; 		// fragment offset is set by internetworking devices, if the packet is fragmented.
					// the fragment offset is not included in this implementation see line 157.
	
	// this should ensure a random ID for all networkTransmit packets.
	int identification = rand()%1000;  // a random value so as to attmept to assure unique identification for a packet.
	while(identification==1 || identification==2) identification = rand()%1000;
	int checksum = ttl + fragOffset + identification; 
			 // networkTransmit header checksum calculation, is made without the data, and the checksum field set to 0
			 // due to limitations in CD++ (we can only send doubles, for now.) the checksum will not include
			 // the addition of the source and destination networkTransmit, since those have very large numbers.	

	
	double united = (((identification*1E3)+ttl)*1E3)+checksum;
	return united;
}
