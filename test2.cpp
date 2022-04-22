/***************************************************
 *	Description:	Test model that interacts with
 *               the data link to test the dataLink
 *
 *	Created by: Abdul-Rahman Elshafei
 ***************************************************/


#include "test2.h"
#include "message.h"
#include "mainsimu.h"

	//	Constructor
test2::test2( const string &name )
: Atomic( name )
, poll( addInputPort( "poll" ) )
, status( addOutputPort( "status" ) )
, preparationTime( 0, 0, 0, 0 )
{
	string time( MainSimulator::Instance().getParameter( description(), "preparation" ) ) ;

	if( time != "" )
		preparationTime = time ;
}

Model &test2::initFunction()
{
   busy=2; //simulate a busy carrier twice until it is idle

   //uinitialize output flags
   sendJamming=false;
   sendStatus=false;
	return *this;
}

Model &test2::externalFunction( const ExternalMessage &msg )
{
	//a poll input from the dataLink to sense the status of the carrier
	if( msg.port() == poll )
	{
   	//if a jamming signal then send a jamming signal to all devices
		if(msg.value()==-1)
         sendJamming=true;
      //otherwise send the status of the carrier
      else
      	sendStatus=true;
      return *this;
	}
   passivate();
	return *this;
}

Model &test2::internalFunction( const InternalMessage & )
{
   passivate();
	return *this;
}

Model &test2::outputFunction( const InternalMessage &msg )
{
	if(sendJamming)
     {
       sendOutput( msg.time(), status, -1);
       sendJamming=false;
     }
   if(sendStatus)
     {
       if(busy>0)
       {
       	sendOutput( msg.time(), status, 0);
         busy--;
       }
       else
       	sendOutput( msg.time(), status, 1);
       sendStatus=false;
     }

  sendOutput( msg.time(), status, 2);
  return *this;
}