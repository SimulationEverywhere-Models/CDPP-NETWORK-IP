/*******************************************************************
*
*  DESCRIPTION: Atomic Model ripTable3
*
*  AUTHOR: Abdul-Rahman Elshafei
*
*  EMAIL: masproject@yahoo.com
*
*  DATE:  17 Feb, 2004
*
*******************************************************************/

/** include files **/
#include <fstream.h>
#include <iostream.h>
#include "ripTable3.h"

#include "message.h"    // class ExternalMessage, InternalMessage
#include "mainsimu.h"   // MainSimulator::Instance().getParameter( ... )

extern void Distance(int n, ConnectionTable cost, MetricTable D, MetricTable m);

/*********************************************************************
* Function Name: ripTable3
* Description: controlling the interface Table for RIPv2
* it includes updating the table
* finding the next interface output for an incoming IP packet via ripProcessor
* responding to requests from other routers
************************************************************************/
ripTable3::ripTable3(const string &name)
: Atomic (name)
, requestIn( addInputPort( "requestIn" ) )
, Address( addInputPort( "Address" ) )
, update( addInputPort( "update" ) )
, respondIn( addInputPort( "respondIn" ) )
, requestOut( addOutputPort( "requestOut" ) )
, respondOut( addOutputPort( "respondOut" ) )
, interfacePort( addOutputPort( "interfacePort" ) )
, preparationTime( 0, 0, 0, 0 )
{
	string time( MainSimulator::Instance().getParameter( description(), "preparation" ) ) ;
	if( time != "" )
		preparationTime = time ;

   //Get the Address of this router
   if( MainSimulator::Instance().existsParameter( description(), "name" ) )
     myAddress = str2Int( MainSimulator::Instance().getParameter( description(), "name" ) );
}

//initial function state
Model &ripTable3 :: initFunction()
{
	sendcnt=0;
	receivecnt=0;
   //Table has zero destinations
	hostSize=0;
   //initlially the network has one node which is this router
   netSize=1;

   //initialize all connections to 16
	for(int j=0;j<NumberOfNodes;j++)
   {
		table[j].metric=16;
      //addresses[j]=0;
      metrics[j]=16;
      table[j].flag=false;
      for(int i=0;i<NumberOfNodes;i++) {
         if(i==j)
           connections[j][i]=0;
         else
           connections[j][i]=16;
      }
   }
   //the local address has index 0
   addresses[hostSize]=myAddress;

   //initialize all ouptut flags
   sendInterface=false;
   sendTable=false;
   sendUpdate=false;
	return *this;
}

// External transition function of ripTable3
Model &ripTable3::externalFunction( const ExternalMessage &msg)
{
   //local counters where s:source, d:destination
	int i,s=0,d=0;

   /* *************************************************** */
   // Find the next interface for the destination address
	if ( msg.port() == Address )
	{
      //temp stores the next interface so that when the output function
      //is called, the output is temp
   	temp=0;
		//find the port number of the destination host
		for(i=0;i<hostSize;i++)
		{
			if(table[i].address==msg.value())
			{
           temp=table[i].gatewayIP;
			}
		}
      sendInterface=true;  //-> flag that send a the gatewayIP
		this->holdIn (active, preparationTime);
		return *this;
	}

   /* *************************************************** */
   //  A response has been received from another router
	if( msg.port() == respondIn )
	{
      //This is for synchronized inputs
      //receivecnt keeps track the current feild of mesage being sent
		if(receivecnt==0)
        {
          //get the metric from the first double
		    packet.metric=msg.value() - commandVersion;
          receivecnt++;
        }
		else if(receivecnt==1)
        {
          //get the source address
		    packet.source=msg.value();
          //check if this address is in the database
          for(i=0;i<netSize;i++)
          {
            if(addresses[i]==packet.source)
              break;   //->source address found
          }
          //if address does not exist then register this address
          if(i==netSize)
          {
            addresses[i]=packet.source;
            netSize++;
            connections[0][i]=neighborMetric;
          }
          receivecnt++;
        }
		else if(receivecnt==2)
		{
         //get the destination of the connection. Note that the destination
         //address here does not mean the local address. it is the address of
         //next interface in the RIP table of the sending device
		   packet.destination= msg.value();
         receivecnt=0; //-> reset the sychronizing message

         //check if the interface is known by local this router
         for(i=0;i<netSize;i++)
	  		 {
            if(addresses[i]==packet.destination)
              break;
	  		 }
         //register this destination in the network as seen by this router
         if(i==netSize)
	       {
            addresses[i]=packet.destination;
	   		netSize++;
          }

         //find the indexes of the source and destination address
			i=0;
         while(i<netSize)
           {
             if(packet.destination==addresses[i])
               d=i;
             if(packet.source==addresses[i])
               s=i;
	     		 i++;
           }
         //link these devices together with th metric sent
         connections[s][d]= (int)packet.metric;

         //update the RIP table using the dijkstras algorithm by finding
         //the new shortest path to each destination
         Distance(netSize, connections, metrics, interface);
         for(i=1;i<=netSize;i++)
           {
              table[i-1].address=addresses[i];
              table[i-1].metric=metrics[i];
              table[i-1].gatewayIP=interface[i];
              table[i-1].flag=true;
           }
         hostSize=netSize-1;  //->update table size
		}
      this->holdIn (active, preparationTime);
      return *this;
	}

   /* *************************************************** */
   // A request from another router
	if( msg.port() == requestIn )
	{
      //tracking the message feild sent by the request
      if(!sendcnt)
        {
          //get metric feild from header double
          temp=msg.value()-commandVersion;
          sendcnt++;
        }
      else
        {
          sendcnt=0;
          //check if the source address is in the network identified by router
          for(i=0;i<netSize;i++)
            if(addresses[i]==msg.value()) break;
          if(i==netSize)
           {
             addresses[i]=msg.value();
             netSize++;
             connections[i][0]=(int)temp;
             connections[0][i]=neighborMetric;
           }
          //check if address exists in the RIP table and add this address
          for(i=0;i<hostSize;i++)
             {
               if(table[i].address==msg.value())
                 break;
             }
          if(i==hostSize)
             {
               table[i].address=msg.value();
               hostSize++;
               table[i].metric=neighborMetric;
               table[i].gatewayIP=msg.value();
               table[i].flag=true;
             }
           sendTable=true;
        }
		this->holdIn (active, preparationTime);
      return *this;
	}

   /* *************************************************** */
   // update to send a request message
   if(msg.port() == update)
     {
       //store the command/Version feild of the message in order to send
       //this header+myAddress to the ripProcessor
       temp=msg.value();
       sendUpdate=true;  //->send requestOut
       this->holdIn (active, preparationTime);
       return *this;
     }

   //invalid message ports will automatically passivate the router
	this->passivate();
	return *this;
}

// Internal transition function
Model &ripTable3::internalFunction ( const InternalMessage &)
{
	this->passivate();
	return *this;
}

//output function state
Model &ripTable3::outputFunction( const InternalMessage &msg)
{
   //send the next interface for a required destination
   if(sendInterface==true)
     {
       this->sendOutput(msg.time(), interfacePort, temp);
       sendInterface=false;   //-> reset the send flag
     }

   //send the information in the RIP table
   if(sendTable==true)
     {
       for(int i=0;i<hostSize;i++)
  		  {
			 this->sendOutput(msg.time(), respondOut, ( commandVersion + table[i].metric ) );
			 this->sendOutput(msg.time(), respondOut, myAddress);
			 this->sendOutput(msg.time(), respondOut, table[i].address);
		  }
       sendTable=false;
     }

   //send an Update message to the main router processor so that it broadcasts
   //the message to all neighboring devices
   if(sendUpdate)
     {
       this->sendOutput(msg.time(), requestOut, temp+neighborMetric);
       this->sendOutput(msg.time(), requestOut, myAddress);
       sendUpdate=false;
     }
	return *this;
}

//end of ripTable3