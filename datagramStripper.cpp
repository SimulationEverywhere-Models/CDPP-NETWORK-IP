/*************************************************
 *	Description:	datagram stripper atomic Model
 *				
 *	Created by:	Mohamed Mohamed Abd El-Salam Ahmed
 *				100302196.
 *	Email:		mohamedmahmed@islamway.net
 *************************************************/

#include "datagramStripper.h"
#include "message.h"
#include "mainsimu.h"

	//	Constructor
datagramStripper::datagramStripper( const string &name )
: Atomic( name )
, in( addInputPort( "in" ) )				// data coming in through the Internet Layer.
, checkin( addInputPort( "checkin" ) )			// data coming in from the checksumValidator.	
, receiveAck( addOutputPort( "receiveAck" ) )		// this is to send ack signals to the datagramCreator. will be connected to the datagramCreator ackPort
, sendAck( addOutputPort( "sendAck" ) )				// this is to signal the datagramCreator to send an Ack.
, datagramStripperOut( addOutputPort( "datagramStripperOut" ) )	// output to application layer		
, preparationTime( 0, 0, 5, 0 )
{
	string time( MainSimulator::Instance().getParameter( description(), "preparation" ) ) ;

	if( time != "" )
		preparationTime = time ;
}

	// Protected functions.

Model &datagramStripper::initFunction()
{
	flag = check = ackflag = false;
	value = ackVar = checksum = 0;
	return *this;
}
//	the datagram stipper will strip the TCP header information. once a 
//  signal is received from the checksumValidator. if the chekcsum is incorrect
//	the data is discarded and, for testing purposes a "0" is sent instead to the
//  Applicaiton.
Model &datagramStripper::externalFunction( const ExternalMessage &msg )
{
	
	if( msg.port() == in )
	{
		value = msg.value();	// received the TCP datagram
		flag = false;
		check = false;
	}
	//---------------------------------------------------------------------------------------------------------------
	if( msg.port() == checkin)
	{
		checksum = msg.value();
		if(checksum == 1)	// data is not corrupted, send data to the applicaiton layer.
							// signal Datagram creator to send an Ack.
		{
			flag = true;
			check = true;
		} else if(checksum == 0)
				{			// data is corrupted, send nothing, later on we would send a request to resend.
					flag = true;
					check = false;
				} else if (checksum == -1)
						{	// data is an ACk
							// signal the ACK to the datagram creator. to delete saved packet, after checking the ack.
							ackflag = true;
						}

	}
	
	holdIn( active, preparationTime );
	return *this;
}

Model &datagramStripper::internalFunction( const InternalMessage & )
{
	passivate();
	return *this;
}


Model &datagramStripper::outputFunction( const InternalMessage &msg )
{
	// will strip the TCP datagram
	// and forward the Applicaiton data to the application layer.
	double result;

	if (flag&&check)	// hence every thing is ok, we received the data, we need to send an ACK, and to delete the 
						// the saved packet in the datagram Creator.
	{
		// so as not to waste the checksum validation signal, the datagram will
		// be strripped only after the checksum been received
		result = stripper(value);
		//-------------------------------------------------------------------------------------
		// we have data to forward to the application, we also need to signal the sending party
		sendOutput( msg.time(), datagramStripperOut, result );
		// the ack is the same packet we received, except that the data is '0'.
		ackVar = createAck(value);	//<-- will create an ack from the data.
		// this is linked with the ackSender on the datagramCreator.
		sendOutput( msg.time(), sendAck, ackVar); 
		
		flag = check = false;
		
	//-----------------------------------------------------------------------------------------
	} else if (flag&&!check)
			{
				// to validate that the data has been discarded
				// we do not send anything.. for now, later on we can send a request to resend the data.
				sendOutput(msg.time(), receiveAck,1);	//the packet is corrupted hence resend.
				flag = check = false;
			}

	if(ackflag)	// we received an ack for data we sent. check the ack and signal to delete.
	{
		bool test = checkAck(value);// will test if the ack we received is true or not. if yes will send '1' else '0'
		if(test)
		{
			sendOutput(msg.time(),receiveAck,0);	// will send a signl to the datagramCreator. that the ack is A O.K
		}else 
			sendOutput(msg.time(),receiveAck,1);	//the ack is corrupted hence resend.
		ackflag = false;
	}
	return *this;
}
// the stripper may need to be changed
double datagramStripper::stripper(double Data)
{
	/*
	step 1 inflate data to an array of 4 entries.
	this step will use the same routine used by the subnetting method
	int dataArray[4];
	dataArray[0] = (int) (Data / 10000000000);
	dataArray[1] = (int) (Data / 1000000) % 10000;
	dataArray[2] = (int) (Data / 10000) % 100000000;
	dataArray[3] = (int) (Data % 1000000000000);
	*/
	//-------------------------------------------------------------------
	//	Another Way of inflating the data in case the previous one fails.
	int dataArray[4], v= (int) (Data/1E6); 
	long y = (Data-(v*1E6));
	dataArray[0] = (int) (Data/1E9);		// will contain port values ss 
	dataArray[1] = (int) (Data/1E6)%1000;	// seq and ack numbers,organized: ssAA
	dataArray[2] = (int) (y/1000)%1000;		// window size and checksum,organized: wwcc
	dataArray[3] = (int) (y%1000);			// data
	//-------------------------------------
	return dataArray[3];	//the data portion of the message.
} 
/*
	this method will be used to check the values of the window, the ack, and the sequence number
	hence currently they are not being changed, it will always return true.
	until the datagram creator is changed to manipulate the numbers as such to create uniquew packets.
	this was not done to test the bigger picture first (meaning, we can send and receive ACKS, hence we have a connection manager)
*/
bool datagramStripper::checkAck(double data)
{
	//if()
	//{
		return true;
	//}
	//else { 
	//	return false;
	//}
}
/*
	this method is used to create acks when we wish to send them. we will have to recheck this method.
*/
double datagramStripper::createAck(double data)
{
	double result;
	int dataArray[4], v= (int) (data/1E6); 
	long y = (data-(v*1E6));
	dataArray[0] = (int) (data/1E9);		// will contain port values ss 
	dataArray[1] = (int) (data/1E6)%1000;	// seq and ack numbers,organized: ssAA
	dataArray[2] = (int) (y/1000)%1000;		// window size and checksum,organized: wwcc
	//---------------------------------------------------------------------------------
	dataArray[3] = (int) (y%1000);			// data 
	result = (dataArray[0]*1E8) + ((dataArray[1]*1E4) + (dataArray[2]));  
	return result;
}