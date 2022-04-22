/*******************************************************************
*
*  DESCRIPTION: Atomic Model tableUpdate
*
*  AUTHOR: Abdul-Rahman Elshafei
*
*  EMAIL: masproject@yahoo.com
*
*  DATE:  11 Feb, 2004
*
*******************************************************************/

/** include files **/
#include "tableUpdate.h"
#include "message.h"    // class ExternalMessage, InternalMessage
#include "mainsimu.h"   // MainSimulator::Instance().getParameter( ... )

 /* +--------+----+----+---------+--------+
 *  |Command |  0 |  0 | Version | Metric |
 *  +--------+----+----+---------+--------+
 *  Command: 1 => request  */
#define commandVersion 100200
#define NumberOfUpdates 3  //referred in the documentation as N 

/*********************************************************************
* Function Name: tableUpdate
* Description: sends request commands every sigma(internal)
*              for N times
************************************************************************/
tableUpdate::tableUpdate(const string &name)
: Atomic (name)
, requestOut( addOutputPort( "requestOut" ) )
, preparationTime( 0, 0, 30, 0 )     //-> initialize preparation time to 30s
{
	string time( MainSimulator::Instance().getParameter( description(), "preparation" ) ) ;
	if( time != "" )
		preparationTime = time ;
}

Model &tableUpdate :: initFunction()
{
	//send a request for update 3 times
   //NumberOfUpdates can be also be determined from the modelfile (.ma) by
   //modifying the constructor feild
	count=NumberOfUpdates;

   //immediately send a request
   this->holdIn (active, preparationTime);
	return *this;
}

//External transition function
Model &tableUpdate::externalFunction( const ExternalMessage &msg)
{
   passivate();
	return *this;
}

//Internal transtion function
Model &tableUpdate::internalFunction ( const InternalMessage &)
{
	//send another update after preparation time
   if(count>0)
     {
       this->holdIn (active, preparationTime);
       return *this;
     }
   //otherwise no update will be done
    else
     {
		 this->passivate();
		 return *this;
     }
}

//output function
Model &tableUpdate::outputFunction( const InternalMessage &msg)
{
	sendOutput(msg.time(), requestOut, commandVersion);
   //decrement the number of requests
   count--;
	return *this;
}