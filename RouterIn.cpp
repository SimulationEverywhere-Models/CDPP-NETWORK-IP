/*******************************************************************
*
*  DESCRIPTION: Atomic Model RouterIn
*
*  AUTHOR: Amir Barylko & Jorge Beyoglonian 
*
*  Modified by: Kahlil Yonis	
*
*  EMAIL: 
*
*  DATE: 27/12/2003
*
*******************************************************************/

/** include files **/
#include "RouterIn.h"      // class Queue
#include "message.h"    // class ExternalMessage, InternalMessage
#include "mainsimu.h"   // MainSimulator::Instance().getParameter( ... )

/** public functions **/

/*******************************************************************
* Function Name: RouterIn
* Description: 
********************************************************************/
RouterIn::RouterIn( const string &name )
: Atomic( name )
, in( addInputPort( "in" ) )
, ready( addInputPort( "ready" ) )
, to_RPU( addOutputPort( "to_RPU" ) )
, flag( addOutputPort( "flag" ) )
, interfaceNum( addOutputPort( "interfaceNum" ) )
, preparationTime( 0, 0, 10, 0 )
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
Model &RouterIn::initFunction()
{
	elements.erase( elements.begin(), elements.end() ) ;
	rs = nothing; // set the receiving state to recevied nothing.
	myid = id();   //det the in_out model id as thre RouterIn processor id
	holdIn( active, preparationTime );
	return *this ;
}

/*******************************************************************
* Function Name: externalFunction
* Description: this function will rcv the packets parts one after
*              the other untill all received. then it will create
*              a packet object using the received values and 
*              then store the Packets instance in the element list.
*              it also receives a packet request from the router processor
*              and forwards a packet to it from the list
********************************************************************/
Model &RouterIn::externalFunction( const ExternalMessage &msg )
{
	if( msg.port() == in ) // recivig a packet
	{
		switch(rs)
		{
			case nothing:
				v1= msg.value();
				rs= got1;
				passivate();
				break;
			case got1:
				v2= msg.value();
				rs= got2;
				passivate();
				break;
			case got2:
				v3= msg.value();
				rs= got3;
				passivate();
				break;
			case got3:
				v4= msg.value();
				
				Packet pac;	// creat a packet and store the recived values in it.
				pac.IP_options = v1;
				pac.destination = v3;
				pac.source = v2;
				pac.TCP_layer = v4;
				elements.push_back( pac ) ; // store the packet in the list
				
				rs= gotAll;	// set the state to indecate that all the packets value were received.
				outputFlag = 1;	//set the output flag to signal a packet ready.
				holdIn( active, preparationTime );
				break;
		}//end of switch statement
	}// end of if statement
	else if( (msg.port() == ready) && ((int)msg.value() == myid) ) 
	{// the router processor will request a packet by sending the interface's id on the "done" port.
		if( !elements.empty() )
		{	outputFlag = 2;
			holdIn( active, preparationTime );
		}
	}

	return *this;
}

/*******************************************************************
* Function Name: internalFunction
* Description: 
********************************************************************/
Model &RouterIn::internalFunction( const InternalMessage & )
{
	
	rs = nothing;
	outputFlag = 0;
	passivate();
	return *this ;
}

/*******************************************************************
* Function Name: outputFunction
* Description: send outputs, if outputFlag = 2, send a packet, 
*                            if outputflag = 1, send a packet ready signal
********************************************************************/
Model &RouterIn::outputFunction( const InternalMessage &msg )
{
	if (outputFlag == 0)//send the interface id to to the In_Out modelcomponants
	{
		sendOutput( preparationTime, interfaceNum, myid );
	}
	else if (outputFlag == 1)//the output should be a signal that a packet was recieved.
	{
		sendOutput( msg.time(), flag, myid ) ;
	}
	else if (outputFlag == 2) // the output should be a packet going to the router processing unit
	{	
		Packet p = elements.front();
		elements.pop_front();
		sendOutput( msg.time(), to_RPU, p.IP_options ) ;
		sendOutput( msg.time(), to_RPU, p.source ) ;
		sendOutput( msg.time(), to_RPU, p.destination ) ;
		sendOutput( msg.time(), to_RPU, p.TCP_layer ) ;
	}
	return *this ;
}
