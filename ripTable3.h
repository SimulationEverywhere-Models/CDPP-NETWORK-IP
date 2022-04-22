// ripTable3.h: interface for the ripTable class.
//
//////////////////////////////////////////////////////////////////////
#ifndef __RIPTABLE3_H
#define __RIPTABLE3_H

#include "atomic.h"     // class Atomic
#include <fstream.h>
#include "hostInfo.h"

/*  +--------+----+----+---------+--------+
 *  |Command |  0 |  0 | Version | Metric |
 *  +--------+----+----+---------+--------+
 *  Command: 2 => repond  */
#define commandVersion 200200

//define the metric cost link connected to all nighbors = 1
#define neighborMetric 1

using namespace std;

// The format of the RIP table
//struct hostInfo {
//	 double address;
//	 double metric;
//	 double gatewayIP;
//	 bool flag;
	 //int routeTimer; for future use
//};

// Temporory stores the RIP packet information

struct Packet {
	double metric;
   	double source;
   	double destination;
   	double interface;
   };

class ripTable3 : public Atomic
{
public:
	ripTable3(const string &name = "ripTable3" );
	virtual string className() const ;
protected:
	Model &initFunction();
	Model &externalFunction( const ExternalMessage & );
	Model &internalFunction( const InternalMessage & );
	Model &outputFunction( const InternalMessage & );

private:
	
	//request for table info incase of update
	const Port &requestIn;

   //respond command message from another device
	const Port &respondIn;
	//send output port for the specified address
	const Port &Address;
   //input from tableUpdate to send a request
    const Port &update;

	//sending output port for adddress
	Port &interfacePort;
	//sending table information due to a request
	Port &respondOut;
   //request message to neighboring nodes
    Port &requestOut;

	Time preparationTime;

   //Adjacency table of the network interconnection
   ConnectionTable connections;
   MetricTable metrics, interface;
   //The name of the address for each index
   double addresses[NumberOfNodes];

   //To keep track of number of destinations in the RIP table
	int hostSize, netSize;
	//to track the snychronization of data coming in
	int receivecnt,sendcnt;

   //the address of the source node or the local router
   double myAddress;

   //temporary storage variables
   Packet packet;
   double temp;

   //RIP version 2 Table
	hostInfo table[NumberOfNodes];

   //Output flags
   bool sendInterface, sendTable, sendUpdate;
};

inline
string ripTable3::className() const
{
	return "ripTable3" ;
}

#endif