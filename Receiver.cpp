/*************************************************
 *	Description:	Receiver (Complex IP) Atomic Model
 *			A network layer simualtion for the Complex IP Model.
 *			The Receiver receives incoming data from the Data link layer, strips of the 
 *					
 *	Created by:	Mohamed Mohamed Abd El-Salam Ahmed
 *			100302196.
 *	Email:		mohamedmahmed@islamway.net
 *************************************************/

#include "Receiver.h"
#include "message.h"
#include "mainsimu.h"
//#include ""		// due to the use of the random function.

	//	Constructor
Receiver::Receiver( const string &name )
: Atomic( name )
, ingress( addInputPort( "ingress" ) )		// data comes in on this port, from the Data Link layer
, sip( addInputPort( "sip" ) )			// to know Source IP (self IP) from.
, egress( addOutputPort( "egress" ) )		// data is outputed through this port. to the Transport layer
, preparationTime( 0, 0, 10, 0 )
{
	string time( MainSimulator::Instance().getParameter( description(), "preparation" ) ) ;

	if( time != "" )
		preparationTime = time ;
}

	// Protected functions.

Model &Receiver::initFunction()
{
	valueCounter=0;
	receive=false;
	send = false;
	return *this;
}

//	Receiver can receive or send data, if data is is being received
//	then Receiver will forward data to the TCP Layer.' this is done in the Receiver module
//	if the data is being sent then Receiver will create the extra 3 headers and send them
//	through the gateway to an internetworking device. this is done in the transmitter module

Model &Receiver::externalFunction( const ExternalMessage &msg )
{
	if( msg.port() == ingress )
	{
		switch(valueCounter)
		{
			case 0 :
				localValue = msg.value();	// options field
				valueCounter++;	// now the counter is 1
				break;
			case 1 :
				localValue2 = msg.value();	// source ip filed
				valueCounter++;	// now the counter is 2
				break;
			case 2 :
				localValue3 = msg.value();	// destination ip filed
				valueCounter++;	// now the counter is 3
				break;
			case 3 :
				localValue4 = msg.value();	// value
				valueCounter = 0; // we have received every thing, the counter is back to 0.
				break;
			default :
				valueCounter = 0;
				break;
		}
		receive = true;
		holdIn( active, preparationTime);
		
	}

	if( msg.port() == egress)
	{
		cerr<<"Can not receive messages on output port\n";
	}

	if( msg.port() == sip)
	{
		sourceIP = msg.value();
	}

	return *this;
}

Model &Receiver::internalFunction( const InternalMessage & )
{
	passivate();
	return *this;
}

Model &Receiver::outputFunction( const InternalMessage &msg )
{
	if(receive&&(valueCounter == 0))	// then we went through the external method 4 times.
	{
		int test = define (localValue4);
		if(test==1)	// hence an ack.
		{
			sendOutput( msg.time(), egress, localValue4);	// Ack
		}
		else if (test == 0)	// hence data.
		{
			send = verify (localValue);
			if(send)
			{
				sendOutput( msg.time(), egress, localValue4);	// TCP data
				send = false;
			}
			receive = false;
		}
	}
	return *this;
}
/*
	
*/
bool Receiver::verify(double value)
{
	// will take the information from the first received packet and recalcualte the checksum,
	// if they match, then the packet hasn't been corrupted.

	// also we  can compare the values of the IP(source IP stored in the model and the destination IP of the packet)
	// to verify the packet is truelly sent to this client. this step is taken to facilitate the createon of a 
	// connection manager later on.
	if(sourceIP == localValue2)
	{
		cout<<"testing that this is our packet"<<endl;
	} 
	//==============================================================================================================
	// Packet Format: Port Sequence Acknowledge Window checksum data
	
	int subnet[4], addresses[4], v=(int) (value/1E6);
	long y = (value-(v*1E6));
	addresses[0] = (int) (value/1E9);
	//cout << " byte 1 :"<<addresses[0]<<"\n";
	addresses[1] = (int) (value/1E6)%1000;
	//cout << " byte 2 :"<<addresses[1]<<"\n";
	addresses[2] = (int) (value/1000)%1000;
	//cout << " byte 3 :"<<addresses[2]<<"\n";
	addresses[3] = (int) (y%1000);		// this is the host count.
	//cout << " byte 4 :"<<addresses[3]<<"\n";
		
	if( addresses[3] == (addresses[0] + addresses[1] + addresses[2]) )
	{
		//checksum is Correct
		 return true;
	} else
		return false;
}
/*
	this function is used to identify a corrupted ack from a normal one.
	
*/
int Receiver::define(double value)
{
	/*
		the numbers on the sides are supposed to help read the code
		because they mean we are sending a packet with the following data:
		
		  88 22 33 44 55 11 the goal of the function is to separate every field.
		  if the packet is an ack then the 11 field will have "0"
	*/
	int halfData= (value/1E6);	//882233
	//-------------------------
	int quarterData = halfData/1E2;	// 8822
	//-----------------------
	int secondHAlf = (value-(halfData*1E6));	// 445511
	//-----------------------------------------
	int port = secondHAlf%100;	// 88
	int data = halfData/10000;	// 11
	int sequence = quarterData%100; // 22
	int ack = halfData%100;	// 33	
	int window = secondHAlf/10000;	// 44
	int quarterSecondHalf = port/1e2;	// 4455
	int checksum = quarterSecondHalf%100; // 55
		
	
	if(data == 0)	// thus its an ACK
		return 1;
	else			// thus its data 
		return 0;
}

