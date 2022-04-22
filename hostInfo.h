#ifndef __hostInfo_H
#define __hostInfo_H

typedef struct hostInfo {
	 double address;
	 double metric;
	 double gatewayIP;
	 bool flag;
//		 int routeTimer;  //Not for now
} ;

#define NumberOfNodes 100
#define NumberOfHosts 5

typedef int MetricTable[NumberOfNodes];
typedef int ConnectionTable[NumberOfNodes][NumberOfNodes];

#endif	//__hostInfo_H
