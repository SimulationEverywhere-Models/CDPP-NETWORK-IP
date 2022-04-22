/*******************************************************************
*
*  DESCRIPTION: Atomic Model ripTable
*
*  AUTHOR: Abdul-Rahman Elshafei
*
*  EMAIL: mailto: masproject@yahoo.com
*   
*  DATE:  20 Oct, 2003
*
*******************************************************************/

/** include files **/
#include<fstream.h>
#include<iostream.h>
#include "ripTable.h"

#include "message.h"    // class ExternalMessage, InternalMessage
#include "mainsimu.h"   // MainSimulator::Instance().getParameter( ... )

/*********************************************************************
* Function Name: ripTable
* Description: "the router processor, you will receive an ip address,
* that you will have to compare with a list of ip's, your output will be
* a port #(a port that is connected to the router)" -Mohammed
************************************************************************/
ripTable::ripTable(const string &name)
: Atomic (name)
, requestPort( addInputPort( "requestPort" ) )
, sendPort( addOutputPort( "sendPort" ) )
, preparationTime( 0, 0, 10, 0 )
{
	string time( MainSimulator::Instance().getParameter( description(), "preparation" ) ) ;
	if( time != "" )
		preparationTime = time ;
}



Model &ripTable :: initFunction()
{
	//j is the current host number
	int j=0;

	//initialize the destination address to zero
	dest=0;

	//read rip table file
	ifstream fin("/home/sce-0566/cd++/table.txt");
	if(fin.fail()) cout<<"file not found\n";
	while(!fin.eof())
	{
	  //read all hosts in table and put it into array hosts
	  if(j==NumberOfHosts) break;
	  fin>>hosts[j].address;
	  fin>>hosts[j].metric;
	  j++;
	}
	fin.close();

	this->passivate();
	return *this;
}

Model &ripTable::externalFunction( const ExternalMessage &msg)
{
	bool found=true;
	if ( msg.port() == requestPort);
	{	
		dest = (long) msg.value();
		double temp;
		//find the port number of the destination host
		for(int i=0;i<NumberOfHosts;i++)
		{
			found =true;
			if(hosts[i].address!=dest)
				found=false;
			if(found)
			{
			  temp = (int)i + 1;
			  this->sendOutput(msg.time(), sendPort, temp);
			  this->holdIn (active, preparationTime);
			  return *this;
			}
		}
	}
	//unknown msg and port not found would return a zero
	cerr<<"Error messag: unknown port and message input\n";
	this->passivate();
	//this->sendOutput (0, sendPort, 0);
	//this->holdIn (active, preparationTime);
	return *this;
}

Model &ripTable::internalFunction ( const InternalMessage &)
{
	this->passivate();
	return *this;
}
Model &ripTable::outputFunction( const InternalMessage &msg)
{
	passivate();
	return *this; 
}
