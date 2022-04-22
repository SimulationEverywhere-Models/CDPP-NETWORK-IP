/*******************************************************************
*
*  DESCRIPTION: Atomic Model tableUpdate
*
*  AUTHOR: Abdul-Rahman Elshafei
*
*  EMAIL:
*
*  DATE: 10/2/2004
*
*******************************************************************/

#ifndef __tableUpdate_H
#define __tableUpdate_H


#include "atomic.h"     // class Atomic

class tableUpdate : public Atomic
{
public:
	tableUpdate( const string &name = "ripUpdate" );	//Default constructor
	virtual string className() const ;
protected:
	Model &initFunction();
	Model &externalFunction( const ExternalMessage & );
	Model &internalFunction( const InternalMessage & );
	Model &outputFunction( const InternalMessage & );

private:
	//send a request command to ripTable3 model
	Port &requestOut;
   
	Time preparationTime;

   //keeps track the number of updates sent
   int count;
};

// ** inline ** //
inline
string tableUpdate::className() const
{
	return "tableUpdate" ;
}

#endif