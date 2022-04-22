/*************************************************
 *	Description:datagram creator Atomic Model
 *				
 *	Created by:	Mohamed Mohamed Abd El-Salam Ahmed
 *				100302196.
 *	Email:		mohamedmahmed@islamway.net
 *************************************************/

#include "datagramCreator.h"
#include "message.h"
#include "mainsimu.h"

//	Constructor
datagramCreator::datagramCreator( const string &name )
: Atomic( name )
, in( addInputPort( "in" ) )
, checkin( addInputPort( "checkin" ) )
, ackPort( addInputPort( "ackPort" ) )		// this is receive Acks on from the datagramStripper.
, ackSender( addInputPort( "ackSender" ) )	// this is receive request to send Ack on. the data to send is the message.
, gocheck( addOutputPort( "gocheck" ) )        // a port to send data to the checksum creator through, should be connected to cheksum "in"
, resendPort( addOutputPort( "resendPort" ) ) 
, datagramCreatorOut( addOutputPort( "datagramCreatorOut" ) )		
, preparationTime( 0, 0, 5, 0 )
{
	string time( MainSimulator::Instance().getParameter( description(), "preparation" ) ) ;

	if( time != "" )
		preparationTime = time ;
}

	// Protected functions.

Model &datagramCreator::initFunction()
{
	/*
		once we can keep track of time(either Real time or A time model) 
		and would like to fully activate the connection manager, to fully implement Jackobson algorithm
		we should have a function that would change the following values  accordingly
	*/
	list1.erase( list1.begin(), list1.end() ) ;	
	value=0;
	checksumValue = 0;
	sequence = 0;
	acknowledge = 0;
	window = 0;				 
	ackflag = sendflag = sendAck = checkflag = false;
	return *this;
}
//	in the simple datagramCreator layer, it is assumed that the source port for
//	an datagramCreator will also be used as the dest port, however, for accuracy
//	later on the dest port must be changed to allow for full functionallity
//	of the TCP conection manager, and to allow for multiple connections.
Model &datagramCreator::externalFunction( const ExternalMessage &msg )
{
	
	if (msg.port() == in)	// therefore data to send
	{
		value = msg.value();
		sendflag = false;	// no data to send yet. create and send the partial packet to the checksum creator.
		checkflag = true;
	} 

	if(msg.port() == checkin)
	{
		checksumValue = 0;
		checksumValue = msg.value();
		sendflag = true;	// data could be sent.
	}

	if(msg.port() == ackPort)
	{
		ackValue = msg.value();
		if(ackValue != 1)		// one means every thing is OK, other than that its not OK
		{
			ackflag = false;	// we received an ack. no need to resend
			list1.pop_front();	// terminating the saved packet.					
		}
		else
			ackflag = true;		// problems encountered needs resending.
	} 

	if(msg.port() == ackSender)	// if we receive a message through here then we send an Ack
	{
		sendAck = true;
		ackVariable = msg.value();	// value we would like to send in the ack message.
	}

	holdIn(active, preparationTime);
	return *this;
}

Model &datagramCreator::internalFunction( const InternalMessage & )
{
	value=0;
	checksumValue = 0;
	sequence = 0;
	acknowledge = 0;
	window = 0;			
	passivate();
	return *this;
}

// output format : value(data sent by user [data generator Atomic model]), source port, dest port
Model &datagramCreator::outputFunction( const InternalMessage &msg )
{
	// will create the TCP datagram as agreed: SSDD:SSAA:WWCC:DD
	if (sendflag)
	{
		sendflag = false; //resetting the sendflag value.
		
		//double packet = creator(value);
		// save the packet before sending it.
		list1.push_back(checksumValue);	
		sendOutput( msg.time(), datagramCreatorOut, checksumValue );
	}
	/*	
		we are resending the packet.
	*/
	if (ackflag)
	{
		/*
			resending the data
			this assumes that list().pop_front() will return the first and should be only eliment in the list.
			another aproach would be to save the packet in the Internet layer. and just send a list index from here.
		*/
	ackflag = false;	// resetting the flag value.
	double temp = list1.front();
	list1.pop_front();	
	sendOutput( msg.time(), resendPort, temp);
	}
	if (sendAck)
	{	// the resendPort is used since it would have reference to the  Ip we sent the packet to.
		// when the IP layer receives a new Destination IP, it will hold it in the resend area.
		sendAck = false;
		sendOutput( msg.time(), resendPort, ackVariable);	// changes to the IP is a resend check in the external method.
	}

	if(checkflag)
	{
		checkflag = false;
		double uncheckedData = 0;
		//-------------------------
		uncheckedData= creator(value);
		//-------------------------
		sendOutput(msg.time(), gocheck/*instead of the checksum getting data from the outside, it gets it here*/, uncheckedData);
	}
	return *this;
}

double datagramCreator::creator( double appData )
{
	
	int first = appData/100;
	int second = (int)appData%100;
	double result = 0.0;
	result = (first*1E11)+(sequence*1E9)+(acknowledge*1E7)+(window*1E5)+(checksumValue*1e2)+second; 
	//---------------------------------------------------------------------------------------------------
	int firstOne = appData/1E11;
	int secondOne = (appData-(firstOne*1e11))/1E9;
	int thirdOne = (appData-firstOne*1e11-secondOne*1E9)/1E7;
	int fourthOne = (appData-firstOne*1e11-secondOne*1E9-thirdOne*1E7)/1E5;
	int fifthOne = (appData-firstOne*1e11-secondOne*1E9-thirdOne*1E7-fourthOne*1e5)/100;
	int sixthOne = (appData-firstOne*1e11-secondOne*1E9-thirdOne*1E7-fourthOne*1e5-fifthOne*100);
	cout<< "firstOne: "<< firstOne<<" second one :"<<secondOne<<" thirdOne :"<< thirdOne<<" fourthOne :"<<fourthOne<<endl;
	cout<< "fifthOne: "<<fifthOne<<" sixthOne :"<<sixthOne<<endl;
	cout<<"result: "<<result<<endl;
	return result;
}
