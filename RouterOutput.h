/*******************************************************************
*
*  DESCRIPTION: Atomic Model RouteOutput
*
*  AUTHOR: Khalil Yonis 
*
*  EMAIL: 
*
*  DATE: 26/11/2003
*
*******************************************************************/

#ifndef __ROUTEROUTPUT_H
#define __ROUTEROUTPUT_H


#include "atomic.h"     // class Atomic

class RouterOutput : public Atomic
{
public:
	RouterOutput( const string &name = "RouterOutput" );	//Default constructor

	virtual string className() const ;
protected:
	Model &initFunction();
	Model &externalFunction( const ExternalMessage & );
	Model &internalFunction( const InternalMessage & );
	Model &outputFunction( const InternalMessage & );

private:
	const Port &from_RPU;
	const Port &interfaceNum; // representing the input interface
	Port &out;
	Time preparationTime;
	
	Time timeLeft;
	// to hold the value of the packet.
	double v1;
	double v2;
	double v3;
	double v4;
	// to hold this output port number in the Router
	double id;
	
	enum rcvState {nothing, got1, got2, got3, needPortNum};
	rcvState rs; // holds the state that the queue is at in receiving the packet

};	// class RouteOutput

// ** inline ** // 
inline
string RouterOutput::className() const
{
	return "RouterOutput" ;
}

#endif   //__RouterOutput_H
