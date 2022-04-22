/*************************************************
 *	Description:	Physical layer(Complex IP) Atomic Model
 *			A simple model of the physical layer
 *	Created by:	Mohamed Mohamed Abd El-Salam Ahmed
 *			100302196.
 *	Email:		mohamedmahmed@islamway.net
 *************************************************/

#include "physical.h"
#include "message.h"
#include "mainsimu.h"

	//	Constructor
physical::physical( const string &name )
: Atomic( name )
, in( addInputPort( "in" ) )	// data comes in on this port, From the data link layer
, in1( addInputPort( "in1" ) ) 
, sensingPort( addInputPort( "sensingPort" ) )			  
, type( addInputPort( "type" ) )		// used to specify what physical layer type is required.
, out( addOutputPort( "out" ) )		// data is outputed through this port. 
, out1( addOutputPort( "out1" ) )
, signal( addOutputPort( "signal" ) )	// The physical layer tells other models of it current state through this port
, preparationTime( 0, 0, 0, 1 )	// the standard preparation time will be used to model ethernet links (standard cables)
{
	string time( MainSimulator::Instance().getParameter( description(), "preparation" ) ) ;

	if( time != "" )
		preparationTime = time ;
}

	// Protected functions.

Model &physical::initFunction()
{
	localValue = localValue1 = 0;
	bool Firstsignal = true;
	return *this;
}

//	the external method will receive data and send them through out the other way (simplest possible way)
Model &physical::externalFunction( const ExternalMessage &msg )
{
	if( msg.port() == type )
	{
		//	the speed factors need more 
		linktype = msg.value();		
		if(linktype == 1 )				// ethernet link medium
		{ 
			delayFactor = (0, 0, 0, 20);			
		}else if(linktype == 2 )		// T1 links slowest
			{
				delayFactor = (0,0,0,6);
			} else if (linktype == 3 )	// Fiber optic fastest
				{
					delayFactor = (0, 0, 0, 5);
				}
		
		send = false;				// no data to send here, just setting the model parameters.	 
	}
	if( msg.port() == sensingPort )
	{
		if(localValue == -1)		// therefore jamming signal
		{
			sensor = -1;
			jamming = true;
			
		} else if(localValue == 0)	// therefore sensing signal
				{
					// what is the state of the sensor here.
					sendSensor = true;
				}
		send = false;
	}
	if( msg.port() == in1 )
	{
		Port_1 = true;
		localValue1 == msg.value();
		send = true;
		if(Firstsignal)
		{
			timeFirst = msg.time();
			Firstsignal = false;
			send = true;
		} else {
					timeSecond = msg.time();
		
					if( timeSecond == timeFirst )
					{
						sensor = 2;
						jamming = true;
						send = false;
					}
		}
	}
	if( msg.port() == in )	
	{
		localValue = msg.value();	
		Port_1 = false;
		send = true;
		
		if(Firstsignal)
		{
			timeFirst = msg.time();	// time the message arrived
			Firstsignal = false;
			send = true;
		} else {
					timeSecond = msg.time();
					if(timeSecond == timeFirst)
					{//	therefore Collision
						sensor = 2; //collision
						jamming = true;
						send = false;
					}
					send = true;
				}
	}
	holdIn( active, preparationTime);	// after data or any relevant information we need to wait
	return *this;
}

Model &physical::internalFunction( const InternalMessage & )
{
	sensor = 0;
	passivate();
	return *this;
}

Model &physical::outputFunction( const InternalMessage &msg )
{
	if(send)
	{
	send = false;	
		if(Port_1)	// then the data came on port in1 and should be outputed on port out
		{
			double sensor = 1;
			sendOutput( msg.time(), out, localValue);	// sending data through physical layer
									// carrier is busy
			sendOutput( msg.time(), signal, sensor); 
			delay = preparationTime ;		// i'm not sure this can work. but we need to change 									the delay factor according to the type of data.
		}else	// the data came on port in and should be outputed on port out1
		{
			double sensor = 1;				// carrier is busy
			sendOutput( msg.time(), out1, localValue);	// sending data through physical layer
			sendOutput( msg.time(), signal, sensor); 
			delay = preparationTime + delayFactor;		// i'm not sure this can work. but we need to change 									the delay factor according to the type of data.
		}
			//holdIn( active, delay);	// according to the link type the delay factor is set
			
	} else if (sendSensor)
			{
				// carrier idle 0; 
				// carrier busy 1; 
				// collision detected 2; 
				// jamming present -1;	
				sendOutput( msg.time(), signal, sensor);
			} else if (jamming)
					{
						sensor = -1;
						sendOutput( msg.time(), signal, sensor);
					}
		sendOutput( msg.time(), signal, sensor);

	return *this;
}

