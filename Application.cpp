/*************************************************
 *	Description:	Application Atomic Model
 *				
 *	Created by:	Mohamed Mohamed Abd El-Salam Ahmed
 *			100302196.
 *	Email:		mohamedmahmed@islamway.net
 *************************************************/

#include "Application.h"
#include "message.h"
#include "mainsimu.h"

	//	Constructor
Application::Application( const string &name )
: Atomic( name )
, in( addInputPort( "in" ) )	
, HTTP_In( addInputPort( "HTTP_In" ) )
, FTP_In( addInputPort( "FTP_In" ) )
, SNMP_In( addInputPort( "SNMP_In" ) )
, SMTP_In( addInputPort( "SMTP_In" ) )
, Telnet_In( addInputPort( "Telnet_In" ) )
, ApplicationOut( addOutputPort( "ApplicationOut" ) )
, RootOut( addOutputPort( "RootOut" ) )		
, preparationTime( 0, 0, 5, 0 )
{
	string time( MainSimulator::Instance().getParameter( description(), "preparation" ) ) ;

	if( time != "" )
		preparationTime = time ;
}

	// Protected functions.

Model &Application::initFunction()
{	flag=false;
	port = 0;
        value=0;
	return *this;
}
//	in the simple application layer, it is assumed that the source port for
//	an application will also be used as the dest port, however, for accuracy
//	later on the dest port must be changed to allow for full functionallity
//	of the TCP conection manager, and to allow for multiple connections.
Model &Application::externalFunction( const ExternalMessage &msg )
{
	
	if( msg.port() == HTTP_In )
	{
		port = 80;
		value = msg.value();
	}
	if( msg.port() == FTP_In )
	{
		port = 21;
		value = msg.value();
	}
	// SNMP should not be used untill more research has been 
	// done into it.
	if( msg.port() == SNMP_In ) 
	{
		//port = unknown port number so far;
		value = msg.value();
	}
	
	if( msg.port() == SMTP_In )
	{
		port = 25;
		value = msg.value();
	} 
	if( msg.port() == Telnet_In ) 
	{
		port = 23;
		value = msg.value();
	}
	if( msg.port() == in )
	{
		flag = true;
		value = (int)(msg.value()/100); 
		if( ( (long)msg.value()%100) == 8 )	// therefore HTTP
		{
			port = 80;
		}else 
			port = (int) msg.value()%100;	
	}
		
	holdIn( active, preparationTime );
	return *this;
}

Model &Application::internalFunction( const InternalMessage & )
{
	passivate();
	return *this;
}

// output format : value(data sent by user [data generator Atomic model]), source port, dest port
Model &Application::outputFunction( const InternalMessage &msg )
{
	if(!flag)
	{
		value = (( value * 100 ) + port) ;
		sendOutput( msg.time(), ApplicationOut, value );
	} else {
				// will need to verify that a port that output directly to
				// the Root so that we can print values received.
				sendOutput( msg.time(), RootOut, value); /*ApplicationOut*/
				sendOutput( msg.time(), RootOut, port); /*ApplicationOut*/
				flag = false;	
			}	
		
	flag = false;
	return *this;
}