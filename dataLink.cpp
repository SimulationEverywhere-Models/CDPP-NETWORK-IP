/*******************************************************************
*
*  DESCRIPTION: Data Link Layer combinig the functions of LLC and MAC
*
*  AUTHOR: Abdul-Rahman Elshafei
*
*  EMAIL:
*
*  DATE: 28/1/2004
*
*******************************************************************/

#include "dataLink.h"
#include "crc.h"
#include<list>
#include "message.h"
#include "mainsimu.h"

	//	Constructor
dataLink::dataLink( const string &name )
: Atomic( name )
, getFrame( addInputPort( "getFrame" ) )
, getPacket( addInputPort( "getPacket" ) )
, status( addInputPort( "status" ) )
, sendFrame( addOutputPort( "sendFrame" ) )
, sendPacket( addOutputPort( "sendPacket" ) )
, senseCarrier( addOutputPort( "senseCarrier" ) )
, preparationTime( 0, 0, 0, 0 )
{
	string time( MainSimulator::Instance().getParameter( description(), "preparation" ) ) ;

	if( time != "" )
		preparationTime = time ;
}

//initial funcion state
Model &dataLink::initFunction()
{
	//preamble=1011; -> can be used in future

   //flag indicates a frame has been sent
   frameSent=false;
   //flag that indicates that collision has happened
   collision=false;
   //initialize all output flags
   sense0=false; frameOut=false;
   busy=false; packetOut=false;
   jamming=false; sense1=false;
}

//external transition function
Model &dataLink::externalFunction( const ExternalMessage &msg )
{

   /* ********************************************** */
   // A packet has been received from the network layer
	if( msg.port() == getPacket )
	{
      //to synchronize the feild received of the packet
      switch(pcount) {
      case 0:  //the other feild of the pacet is received
      	other.push_back(msg.value());
         pcount++;
         break;
      case 1:  //the IP destination address
      	destination.push_back(msg.value());
         pcount++;
         break;
		case 2:  //the IP source address
      	source.push_back(msg.value());
         pcount++;
         break;
		case 3: //the data feild of the IP packet
      	data.push_back(msg.value());
         pcount=0;
         //send a sensing signal
         sense0=true;
         holdIn( active, preparationTime);
         return *this;
         break;
      default:
      	pcount=0;
         break;
      }
	}

	/* ************************************************************** */
	// A frame has been received from another device via physical layer
	if( msg.port() == getFrame)
	{
   	switch(fcount) {
      case 0:   //other feilds in the packet section of the frame
      	temp.other = msg.value();
         fcount++;
         break;
      case 1:  //destination address
      	temp.destination = msg.value();
         fcount++;
         break;
		case 2:  //source address
      	temp.source = msg.value();
         fcount++;
         break;
		case 3:  //receive data feild
      	temp.data = msg.value();
         fcount++;
         break;
      case 4:  //FCS feild of the frame
      	temp.fcs = msg.value();
         fcount=0;
         //check if the frame has any errors by using CRC algorithm
         if(CRC((temp.destination+temp.other+temp.data+temp.source),temp.fcs))
           {
             packetOut=true;
             holdIn( active, preparationTime);
             return *this;
			  }
			//the error frame is discarded by not sending the packet
			break;
		default:
			fcount=0;
			break;
		}
	}

	/* ************************************************************** */
	// the physical layer sends the status of the carrier
	if( msg.port() == status)
	{
   	//Carrier is idle
      if(msg.value()==1)
			{
				//check if there was a collision that happened before
				if(collision)
            	{
                 //resend previous frame
                 frameOut=true;
                 sense0=true;
                 collision=false;
                 holdIn( active, preparationTime);
					  return *this;
					}
				else
					{
					if(frameSent)
					 {
						//last frame successfully sent
						//then send next frame in queue
						other.pop_front();
						destination.pop_front();
						source.pop_front();
						data.pop_front();
						frameSent=false;
					 }
					//sense carrier before sending the next frame
					if(other.size()!=0)
						{
						  frameOut=true;
						  sense0=true;
						  frameSent=true;
						  holdIn( active, preparationTime);
						  return *this;
						}
					}
			}

		//there is a jam in carrier
		else if(msg.value()==-1)
			{
				//check if there is information to send
				if(other.size()!=0)
				{
					jamming=true;
					holdIn( active, preparationTime);
					return *this;
				}
			}

		//Carrier is busy
		else if(msg.value()==0)
			{
				//go to the internal transition function
				busy=true;
				holdIn( active, preparationTime);
				return *this;
			}

		//Collision in carrier
		else if(msg.value()==2)
			{
				collision=true;
				sense1=true;    //-> jam the physical layer
				holdIn( active, preparationTime);
				return *this;
			}
	}
	passivate();
	return *this;
}

//internal transition function
Model &dataLink::internalFunction( const InternalMessage & msg)
{
	//wait for five miliseconds
   //approximately maximum time for 1 frame to be sent in carrier
   //then check the status of the carrier
   if(busy)
   {
	Time adder2(0,0,0,5);
   sendOutput((msg.time()+adder2), senseCarrier, 0);
   busy=false;
   }
	passivate();
	return *this;
}

//output state function
Model &dataLink::outputFunction( const InternalMessage &msg )
{
	int x;

	//send frame
	if(frameOut)
	  {
       sendOutput( msg.time(), sendFrame, other.front());
       sendOutput( msg.time(), sendFrame, destination.front());
       sendOutput( msg.time(), sendFrame, source.front());
       sendOutput( msg.time(), sendFrame, data.front());
       sendOutput( msg.time(), sendFrame, FCS(source.front()+destination.front()+data.front()+other.front()));
       frameOut=false;
	  }

	//sense the carrier before transmission of data
	//a value of 0 means sensing
	if(sense0)
	{
	  sendOutput( msg.time() , senseCarrier, 0);
	  sense0=false;
	}

	//send packet to internet layer
   if(packetOut)
   {
     sendOutput( msg.time(), sendPacket, temp.other);
     sendOutput( msg.time(), sendPacket, temp.destination);
     sendOutput( msg.time(), sendPacket, temp.source);
     sendOutput( msg.time(), sendPacket, temp.data);
     packetOut=false;
	}

   //sense carrier again after random delay to resend frame
   if(jamming)
     {
       //create random delay from 0 to 10 msec
       x = (int)other.front()%10;
       Time adder(0,0,0,x);
       //sense carrier again
       sendOutput((msg.time()+adder), senseCarrier, 0);
       jamming=false;
	  }

	// value of -1 means jamming the physical connection
   if(sense1)
     {
       //send a jamming signal to the carrier
       sendOutput( msg.time() , senseCarrier, -1);
     }
	return *this;
}