/*************************************************
 *	Description:	Checksum CreatorAtomic Model
 *				
 *	Created by:	Mohamed Mohamed Abd El-Salam Ahmed
 *				100302196.
 *	Email:		mohamedmahmed@islamway.net
 *************************************************/

#include "checksumCreator.h"
#include "message.h"
#include "mainsimu.h"

	//	Constructor
checksumCreator::checksumCreator( const string &name )
: Atomic( name )
, in( addInputPort( "in" ) )	
, checksumCreatorOut( addOutputPort( "checksumCreatorOut" ) )		
, preparationTime( 0, 0, 5, 0 )
{
	string time( MainSimulator::Instance().getParameter( description(), "preparation" ) ) ;

	if( time != "" )
		preparationTime = time ;
}

	// Protected functions.

Model &checksumCreator::initFunction()
{
	value=0;
	return *this;
}
//	in the simple checksumCreator layer, it is assumed that the source port for
//	an checksumCreator will also be used as the dest port, however, for accuracy
//	later on the dest port must be changed to allow for full functionallity
//	of the TCP conection manager, and to allow for multiple connections.
Model &checksumCreator::externalFunction( const ExternalMessage &msg )
{
	
	if( msg.port() == in )
	{
		value = msg.value();
	}
	
	holdIn( active, preparationTime );
	return *this;
}

Model &checksumCreator::internalFunction( const InternalMessage & )
{
	passivate();
	return *this;
}

// output format : value(data sent by user [data generator Atomic model]), source port, dest port
Model &checksumCreator::outputFunction( const InternalMessage &msg )
{
	double result = checksum(value);
	sendOutput( msg.time(), checksumCreatorOut, result );
	return *this;
}

double checksumCreator::checksum (double data)
{
	/*
		ddssaawwcccpp
		1100000000000
		  22000000000
		    330000000
		      4400000
			    555X0
				   88
		1122334455588
	*/
	int firstOne = data/1E11;	
	int secondOne = (data-(firstOne*1e11))/1E9;
	int thirdOne = (data-firstOne*1e11-secondOne*1E9)/1E7;
	int fourthOne = (data-firstOne*1e11-secondOne*1E9-thirdOne*1E7)/1E5;
	int fifthOne = (data-firstOne*1e11-secondOne*1E9-thirdOne*1E7-fourthOne*1e5)/100;	// since we are creating the checksum this should be zero.
	int sixthOne = (data-firstOne*1e11-secondOne*1E9-thirdOne*1E7-fourthOne*1e5-fifthOne*100);
	// after all field are isolated.
	fifthOne = firstOne+secondOne+thirdOne+fourthOne+sixthOne;
	//cout<< "firstOne: "<< firstOne<<" second one :"<<secondOne<<" thirdOne :"<< thirdOne<<" fourthOne :"<<fourthOne<<endl;
	//cout<< "fifthOne: "<<fifthOne<<" sixthOne :"<<sixthOne<<endl;
	double result =(firstOne*1E11)+(secondOne*1E9)+(thirdOne*1E7)+(fourthOne*1E5)+(fifthOne*1e2)+sixthOne;
	return result;	
}