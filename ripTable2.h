// ripTable2.h: interface for the ripTable class.
//
//////////////////////////////////////////////////////////////////////
#ifndef __RIPTABLE2_H
#define __RIPTABLE2_H

#define NumberOfHubs 5
#define commandVersion 200200

#include "atomic.h"     // class Atomic
#include "hostInfo.h"

//struct HostInfo {
//	 double address;
//	 double metric;
//	 double gatewayIP;
//};

class ripTable2 : public Atomic
{
public:
	ripTable2(const string &name = "ripTable2" );
	virtual string className() const ;
protected:
	Model &initFunction();
	Model &externalFunction( const ExternalMessage & );
	Model &internalFunction( const InternalMessage & );
	Model &outputFunction( const InternalMessage & );
private:
	//request for table info incase of update
	const Port &request;
	//send output port for the specified address
	const Port &Address;
	//getting required information to update table
	const Port &update;
	
	//sending output port for adddress
	Port &interfacePort;
	//acknowledgent that table updated
	Port &done;
	//sending table information due to a request
	Port &respond;

	Time preparationTime;
	
	//To keep track of number of updated hosts and hosts sent
	int hostSize, hostSent;
	double temp;

	//to track the snychronization of data coming in and out
	int sendcnt,receivecnt;
	hostInfo hosts[NumberOfHubs];
	hostInfo hostTemp;
	
	//flags for output
	bool sendDone, sendPort, sendTable;
};

inline
string ripTable2::className() const
{
	return "ripTable2" ;
}

#endif