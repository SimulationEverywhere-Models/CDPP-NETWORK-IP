// ripTable.h: interface for the ripTable class.
//
//////////////////////////////////////////////////////////////////////
#ifndef __RIPTABLE_H
#define __RIPTABLE_H

#include "atomic.h"     // class Atomic
#include <fstream.h>
#include "hostInfo.h"

class ripTable : public Atomic
{
public:
	ripTable(const string &name = "ripTable" );
	virtual string className() const ;
protected:
	Model &initFunction();
	Model &externalFunction( const ExternalMessage & );
	Model &internalFunction( const InternalMessage & );
	Model &outputFunction( const InternalMessage & );
private:
	const Port &requestPort;
	Port &sendPort;
	Time preparationTime;
	//ifstream fin;
	long dest;
	hostInfo hosts[NumberOfHosts];
};

inline
string ripTable::className() const
{
	return "ripTable" ;
}

#endif