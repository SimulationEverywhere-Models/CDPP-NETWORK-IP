/*******************************************************************
*
*  DESCRIPTION: dataLink header file
*
*  AUTHOR: Abdul-Rahman Elshafei
*
*  EMAIL: masproject@yahoo.com
*
*******************************************************************/

#ifndef __dataLink_H
#define __dataLink_H
#include<list>
#include "atomic.h"

//The MAC frame format and corresponding feilds
struct frame
	{
     double other;
     double destination;
     double source;
     double data;
     double fcs;
   };

class dataLink : public Atomic
{
	public:
		dataLink( const string &name = "dataLink" );
		virtual string className()const;
	protected:

		Model &initFunction();
		Model &externalFunction( const ExternalMessage & );
		Model &internalFunction( const InternalMessage & );
		Model &outputFunction( const InternalMessage & );
	private:
		// Ports:
		const Port &getPacket;
      const Port &getFrame;
      const Port &status;   //->status of carrier
		Port &sendFrame;
      Port &sendPacket;
      Port &senseCarrier;

		// Time varialbes
		Time preparationTime;

		// variables related to data link.
      frame temp;
      list<double> other;       //->queueing the packets before sending them
      list<double> destination;
      list<double> source;
      list<double> data;
      int fcount, pcount;
      bool frameSent,collision;

      //flags for output
      bool sense0, busy, packetOut, frameOut;
      bool jamming, sense1;
};

		//	inline methods
inline
string dataLink::className()const
{
	return "dataLink";
}

#endif