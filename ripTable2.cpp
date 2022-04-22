/*******************************************************************
*
*  DESCRIPTION: Atomic Model ripTable2 
*
*  AUTHOR: Abdul-Rahman Elshafei
*
*  EMAIL: mailto: masproject@yahoo.com
*   
*  DATE:  6 Jan, 2004
*
*******************************************************************/

/** include files **/
#include<fstream.h>
#include<iostream.h>
#include "ripTable2.h"

#include "message.h"    // class ExternalMessage, InternalMessage
#include "mainsimu.h"   // MainSimulator::Instance().getParameter( ... )

/*********************************************************************
* Function Name: ripTable2
* Description: controlling the interface Table for RIPv2
* it includes updating the table
* finding the next interface output for an incoming IP packet
* responding to requests from other routers 
************************************************************************/
ripTable2::ripTable2(const string &name)
: Atomic (name)
, request( addInputPort( "request" ) )
, Address( addInputPort( "Address" ) )
, update( addInputPort( "update" ) )
, respond( addOutputPort( "respond" ) )
, done( addOutputPort( "done" ) )
, interfacePort( addOutputPort( "interfacePort" ) )
, preparationTime( 0, 0, 5, 0 )
{
	string time( MainSimulator::Instance().getParameter( description(), "preparation" ) ) ;
	if( time != "" )
		preparationTime = time ;
}


Model &ripTable2 :: initFunction()
{
	sendcnt=0;
	receivecnt=0;
	hostSize=0;
	hostSent=0;
   //initialize all connections to metric 16 or infinity
	for(int j=0;j<NumberOfHubs;j++)
		hosts[j].metric=16;
	
	//initialize the output flags
	sendDone=false;
	sendTable=false;
	sendPort=false;
	this->passivate();
	return *this;
}


Model &ripTable2::externalFunction( const ExternalMessage &msg)
{
	int i;
	bool found;

   //input address of an intended destination
	if ( msg.port() == Address )
	{
		temp=0;
		//find the port number of the destination host
		for(i=0;i<hostSize;i++)
		{
			found =true;
			if(hosts[i].address!=msg.value())
				found=false;
         //the destination is in the table
			if(found==true)
			{
			  temp = hosts[i].gatewayIP;
			}
		}
      //send the next interface
		sendPort=true;
		this->holdIn (active, preparationTime);
		return *this;
	}

   //an update input to update the table
	if( msg.port() == update )
	{
   	//receive the destination address first
		if(receivecnt==0)
		{
      	//save the destination temporarly
		  hostTemp.address=msg.value();
		  receivecnt++;  //->get next input message of update
		}
      //second: receive the metric of the desired destination
		else if(receivecnt==1)
		{
		  hostTemp.metric=msg.value();
		  receivecnt++;
		}
      //last: the interface port of the destination
		else if(receivecnt==2)
		{
		   hostTemp.gatewayIP=msg.value();
		   receivecnt=0;     //->reset message sequence
		   sendDone=true;
		   i=0;
         //find the destination if it is in the table already
		   while(i<hostSize)
			{
			 if(hosts[hostSize].address==hostTemp.address) break;
		 	 i++;
			}
         //if the destination is not in the table, then add the new destination
         //with the its corresponding information
		   if(i==hostSize)
		     {
		   	hosts[hostSize].address=hostTemp.address;
		   	hosts[hostSize].metric=hostTemp.metric;
		   	hosts[hostSize].gatewayIP=hostTemp.gatewayIP;
	 	   	hostSize++;  // ->increment table size
		     }
		   else
 		     {
            //the destination already exists, then check if the new metric cost
            //is less than the metric already stored
			   if(hosts[i].metric>hostTemp.metric)
				  hosts[i].metric=hostTemp.metric;
           }
		   this->holdIn (active, preparationTime);
		   return *this;
		}
	}

   //a request message to output table information
	if( msg.port() == request )
	{
		temp=msg.value(); // -> store temporarly the address of the requestor
		sendTable=true;
		this->holdIn (active, preparationTime);
		return *this;
	}

   //unknown input port will passivate the model
	this->passivate();
	return *this;
}


Model &ripTable2::internalFunction ( const InternalMessage &)
{
	this->passivate();
	return *this;
}


Model &ripTable2::outputFunction( const InternalMessage &msg)
{
	int i;
   //send interface port of the desired destination
	if(sendPort)
	{
		this->sendOutput(msg.time(), interfacePort, temp);
		sendPort=false;
	}
   //send an acknowledgement that the table has been updated
	if(sendDone)
	{
		this->sendOutput(msg.time(), done, 0);
		sendDone=false;
	}
   //send the RIP table information in response to a request
	if(sendTable)
	{
		for(i=0;i<hostSize;i++)
  		{
			this->sendOutput(msg.time(), respond, (commandVersion + hosts[i].metric) );
			this->sendOutput(msg.time(), respond, hosts[i].address);
			this->sendOutput(msg.time(), respond, 0 );
			this->sendOutput(msg.time(), respond, 0 );
			this->sendOutput(msg.time(), respond, temp );
		}
		sendTable=false;
	}
	return *this;
}