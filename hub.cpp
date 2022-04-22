/*************************************************
 *	Description:	hub Atomic Model
 *			An 8 repeater for data packets, that can
 *			interconnect different link speeds. 
 *			the Hub follows the IEEE 802.3 5-4-3 rule	
 *	Created by:	Mohamed Mohamed Abd El-Salam Ahmed
 *			100302196.
 *	Email:		mohamedmahmed@islamway.net
 *	Date:		28th of november 2003
 *************************************************/

#include "hub.h"
#include "message.h"
#include "mainsimu.h"

	//	Constructor
hub::hub( const string &name )
: Atomic( name )
, setter( addInputPort( "setter" ) )
, ingress( addInputPort( "ingress" ) )	
, egress1( addOutputPort( "egress1" ) )	
, egress2( addOutputPort( "egress2" ) )	
, egress3( addOutputPort( "egress3" ) )	
, egress4( addOutputPort( "egress4" ) )	
, egress5( addOutputPort( "egress5" ) )	
, egress6( addOutputPort( "egress6" ) )	
, egress7( addOutputPort( "egress7" ) )	
, egress8( addOutputPort( "egress8" ) )	
, egress9( addOutputPort( "egress9" ) )	
, preparationTime( 0, 0, 00, 0 )
{
	string time( MainSimulator::Instance().getParameter( description(), "preparation" ) ) ;

	if( time != "" )
		preparationTime = time ;
}

	// Protected functions.

Model &hub::initFunction()
{
	localValue = 0;
	send = false;
	MAC = 0;
	return *this;
}

Model &hub::externalFunction( const ExternalMessage &msg )
{
	if( msg.port() == ingress )
	{
		localValue = msg.value();
		holdIn( active, preparationTime);
		send = true;
	} else  if( msg.port() == setter )
		{
			MAC = msg.value();
		}
	return *this;
}

Model &hub::internalFunction( const InternalMessage & )
{
	passivate();
	return *this;
}

Model &hub::outputFunction( const InternalMessage &msg )
{
	if(send)
	{	
		// regenerating traffic to all connected ports.
		sendOutput( msg.time(), egress1, localValue);
		sendOutput( msg.time(), egress2, localValue);
		sendOutput( msg.time(), egress3, localValue);
		sendOutput( msg.time(), egress4, localValue);
		sendOutput( msg.time(), egress5, localValue);
		sendOutput( msg.time(), egress6, localValue);
		sendOutput( msg.time(), egress7, localValue);
		sendOutput( msg.time(), egress8, localValue);
		sendOutput( msg.time(), egress9, localValue);
	}
	return *this;
}