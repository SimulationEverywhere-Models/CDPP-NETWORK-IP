/*************************************************
 *	Description:	Checksum validator Atomic Model
 *				
 *	Created by:	Mohamed Mohamed Abd El-Salam Ahmed
 *				100302196.
 *	Email:		mohamedmahmed@islamway.net
 *************************************************/

#include "checksumValidator.h"
#include "message.h"
#include "mainsimu.h"

	//	Constructor
checksumValidator::checksumValidator( const string &name )
: Atomic( name )
, in( addInputPort( "in" ) )	
, checksumValidatorOut( addOutputPort( "checksumValidatorOut" ) )		
, preparationTime( 0, 0, 5, 0 )
{
	string time( MainSimulator::Instance().getParameter( description(), "preparation" ) ) ;

	if( time != "" )
		preparationTime = time ;
}

	// Protected functions.

Model &checksumValidator::initFunction()
{
	value=0;
	return *this;
}
//	the model will receive a TCP datagram, verify the checksum 
//  by means of getting the checksum from the datagram, and then recomputing and compare.
Model &checksumValidator::externalFunction( const ExternalMessage &msg )
{
	if( msg.port() == in )
	{
		value = msg.value();
	}
	
	holdIn( active, preparationTime );
	return *this;
}

Model &checksumValidator::internalFunction( const InternalMessage & )
{
	passivate();
	return *this;
}

// output format : value(data sent by user [data generator Atomic model]), source port, dest port
Model &checksumValidator::outputFunction( const InternalMessage &msg )
{
	double result = validator(value);
	sendOutput( msg.time(), checksumValidatorOut, result );
	return *this;
}

int checksumValidator::validator (double data)
{
	// step 1 inflate data to an array of 4 entries.
	// this step will use the same routine used by the subnetting method
	/*
	int dataArray[4];
	dataArray[0] = ((int) data / 1E9);
	dataArray[1] = ((int) data / 1000000) % 10000;
	dataArray[2] = ((int) data / 10000) % 100000000;
	dataArray[3] = ((int) data % 1E12);
	//-------------------------------------------------------------------
	*/
	//	Another Way of inflating the data in case the previous one fails.
	int dataArray[4]; 
	int v = (int) (data/1E6); 
	long y = (data-(v*1E6));
	dataArray[0] = (int) (data/1E9);	// will contain port values ssdd, where ss: source. dd: destination 
	dataArray[1] = (int) (data/1E6)%1000;	// seq and ack numbers,organized: ssAA
	dataArray[2] = (int) (y/1000)%1000;	// window size and checksum,organized: wwcc
	dataArray[3] = (int) (y%1000);		// data
	
	/*
		if this is an ACK
	*/
	if(dataArray[3] == 0)
		return -1;
	/*
		if this is data
	*/
	
	// we require the 3rd field of the array, thus
	int temp = dataArray[2];
	// the field is organized WWCC,
	int checksumCheck = temp % 100;
	int windowsize = temp / 100;  
	// to validate the checksum it is first set to "0", then the checksum is recomputed
	dataArray[2] = windowsize;
	double sum = 0;
	for(int i = 0; i < 4; i++)
	{
		sum = sum + dataArray[i];
	}
	if (sum == checksumCheck)
		return 1;
	else 
		return 0;	
}