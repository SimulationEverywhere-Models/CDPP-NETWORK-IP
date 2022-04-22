/*******************************************************************
*
*  DESCRIPTION: Atomic Model RouteOutput
*
*  AUTHOR: Khalil Yonis 
*
*  EMAIL: 
*
*  DATE: 26/11/2003
*
*******************************************************************/


/** include files **/
#include "RouterOutput.h"      // 
#include "message.h"    // class ExternalMessage, InternalMessage
#include "mainsimu.h"   // MainSimulator::Instance().getParameter( ... )

/** public functions **/

/*******************************************************************
* Function Name: 
* Description: 
********************************************************************/
RouterOutput::RouterOutput( const string &name )
: Atomic( name )
, from_RPU( addInputPort( "from_RPU" ) )
, interfaceNum( addInputPort( "interfaceNum" ) )
, out( addOutputPort( "out" ) )
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
Model &RouterOutput::initFunction()
{
	rs = nothing;
	return *this ;
}

/*******************************************************************
* Function Name: externalFunction
* Description: 
********************************************************************/
Model &RouterOutput::externalFunction( const ExternalMessage &msg )
{
	if( msg.port() == interfaceNum) // the router output port number
	{
		id = msg.value();
		passivate();
	}
	if( msg.port() == from_RPU ) // recivig a packet
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
				rs= needPortNum;
				passivate();
				break;
			case needPortNum:
				//if it is the same as the models PortNum, we will output the pacet,
				// else desregard the packet.
				if ( (id == msg.value())||((msg.value()<0)&&(msg.value() != (-1*id))))
				{
					holdIn( active, preparationTime );
				}
				else
				{
					v1 =0;
					v2 =0;
					v3 =0;
					v4 =0;
					
					passivate();
				}
				rs = nothing;
				break;
		}//end of switch statement
	}// end of if statement

	return *this;
}

/*******************************************************************
* Function Name: internalFunction
* Description: 
********************************************************************/
Model &RouterOutput::internalFunction( const InternalMessage & )
{
	v1 =0;
	v2 =0;
	v3 =0;
	v4 =0;
	rs = nothing;
	passivate();
	return *this ;
}

/*******************************************************************
* Function Name: outputFunction
* Description: 
********************************************************************/
Model &RouterOutput::outputFunction( const InternalMessage &msg )
{
	sendOutput( msg.time(), out, v1 ) ;
	sendOutput( msg.time(), out, v2 ) ;
	sendOutput( msg.time(), out, v3 ) ;
	sendOutput( msg.time(), out, v4 ) ;
	return *this ;
}
