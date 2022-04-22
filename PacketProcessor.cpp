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


/** include files **/
#include "PacketProcessor.h"      // 
#include "message.h"    // class ExternalMessage, InternalMessage
#include "mainsimu.h"   // MainSimulator::Instance().getParameter( ... )
#include <iostream.h>
/** public functions **/

/*******************************************************************
* Function Name: 
* Description: 
********************************************************************/
PacketProcessor::PacketProcessor( const string &name )
: Atomic( name )
, in( addInputPort( "in" ) )
, outInterface( addInputPort( "outInterface" ) )
, packet( addInputPort( "packet" ) )
, cont( addInputPort( "cont" ) )
, destination( addOutputPort( "destination" ) )
, updateTable( addOutputPort( "updateTable" ) )
, requ( addOutputPort( "requ" ) )
, getPacket( addOutputPort( "getPacket" ) )
, out( addOutputPort( "out" ) )
, next( addOutputPort( "next" ) )
, preparationTime( 0, 0, 5, 0 )
{
	string time( MainSimulator::Instance().getParameter( description(), "preparation" ) ) ;

	if( time != "" )
		preparationTime = time ;
}

/*******************************************************************
* Function Name: initFunction
* Description: 
* Precondition: 
********************************************************************/
Model &PacketProcessor::initFunction()
{
	rcv = 0;
	os = unknown;
	return *this ;
}

/*******************************************************************
* Function Name: externalFunction
* Description: 
********************************************************************/
Model &PacketProcessor::externalFunction( const ExternalMessage &msg )
{
	if(msg.port() == in )//a packet is ready for processing.
	{
		inId = msg.value(); //store the value of the input interface.
		sendOutput(msg.time(), getPacket, inId); //signal to get the packet.
		passivate();
	}
	else if (msg.port() == packet )
	{
		switch(rcv)
		{
			case 0:
				v1 = msg.value();
				rcv = 1;//got1;
				passivate();
				break;
			case 1:
				v2 = msg.value();
				rcv = 2;//got2;
				passivate();
				break;
			case 2:
				v3 = msg.value();
				rcv = 3;//got3;
				passivate();
				break;
			case 3:
				v4 = msg.value();
				rcv = 4;
				if ((int)(v1/1000000) == 1)
				{	
					os = request;
				}
				else if ((int)(v1/1000000) == 2)
				{		
					os = response;
				}
				else
				{	
					//extract time to live TTL and CRC from option field (v1).
					// v1 = typ(3 digits),TTL (3 digits, CRC (3 digits)
					int ttl;
					ttl = (int)v1/1000; // remove CRC
					ttl = ttl%1000;
					if(ttl > 0)
					{
						os = data;
						v1=v1-1000; // decrement ttl by 1.
					}
					else
					{//discard packet
						v1=v2=v3=v4=inId=0;
						os = unknown;
					}
				}
				holdIn( active, preparationTime );
				break;
			default:
				passivate();
				break;
		}//end of switch statement

	}//end of if statement
	else if(msg.port() == outInterface)
	{
		if (msg.value() == 0)
			os = sendRequest;
		else
		{
			os = forward;
			outId = msg.value();
		}
		holdIn( active, preparationTime);
	}
	else if(msg.port() == cont)
	{//a signal conferming that the request has been fulfilled
		sendOutput( msg.time(), next, 0);
		passivate();
	}
		
	return *this;
}

/*******************************************************************
* Function Name: internalFunction
* Description: 
********************************************************************/
Model &PacketProcessor::internalFunction( const InternalMessage & )
{
	if (rcv == 4)
		rcv = 0;
	os = unknown;
	passivate();
	return *this ;
}

/*******************************************************************
* Function Name: outputFunction
* Description: 
********************************************************************/
Model &PacketProcessor::outputFunction( const InternalMessage &msg )
{
	switch (os)
	{
		case request: // this is a request so signal table
			sendOutput( msg.time(), requ, inId );
			sendOutput( msg.time(), requ, v2 ); // the address in the request.
			break;
		case response: // we got a responce to update our table
			sendOutput( msg.time(), updateTable, v2 );
			sendOutput( msg.time(), updateTable, (int)v1%1000 );
			sendOutput( msg.time(), updateTable, inId );
			//sendOutput( msg.time(), next, 0); // signal the queue that processing is done.
			break;
		case data: // request the output interface from table
			sendOutput( msg.time(), destination, v3 );
			break;
		case forward: // forward the packet
			sendOutput( msg.time(), out, v1);
			sendOutput( msg.time(), out, v2);
			sendOutput( msg.time(), out, v3);
			sendOutput( msg.time(), out, v4);
			sendOutput( msg.time(), out, outId);
			sendOutput( msg.time(), next, 0); // segnal the queue taht processing is done.
			break;
		case sendRequest:
			//creat a request packet and send it out.
			//we already have the destinationaddress in v3.
			v1 = (1000000);//IP packet options. (command+ rip version + metric)
			sendOutput( msg.time(), out, v1);
			sendOutput( msg.time(), out, v3);// address that we need to be updated about.
			sendOutput( msg.time(), out, 0); // for future use (can be used for the subnet mask).
			sendOutput( msg.time(), out, 0); // to be consistance
			sendOutput( msg.time(), out, -1*inId);// to all output interfaces.
			sendOutput( msg.time(), next, 0); // segnal the queue taht processing is done.
			break;
		case unknown:
			sendOutput( msg.time(), next, 0); // segnal the queue taht processing is done.
	}//end of switch statement.
	return *this ;
}
