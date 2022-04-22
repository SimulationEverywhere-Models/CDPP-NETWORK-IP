#ifndef __HUB_H
#define __HUB_H
/* 5-4-3 rule which means a maximum of five segments, 
 * four repeaters (hubs), and only three mixing segments
 * the only visible difference between a repeater and a hub. 
 * is the ability of a hub to interconnect different physical lines 
 ******************************************************************/
#include "atomic.h"

class hub : public Atomic
{
	public:
		hub( const string &name = "Hub" );
		virtual string className()const;
	protected:
		
		Model &initFunction();
		Model &externalFunction( const ExternalMessage &msg );
		Model &internalFunction( const InternalMessage & );
		Model &outputFunction( const InternalMessage &msg );
	private:
		// Ports:
		const Port &setter;		// input port
		const Port &ingress;		// input port
		// creating a 5 port hub, 4 for connected devices and the 5th for the next hub
		Port &egress1;			// output port
		Port &egress2;			// output port
		Port &egress3;			// output port
		Port &egress4;			// output port
		Port &egress5;			// output port 
		Port &egress6;			// output port
		Port &egress7;			// output port
		Port &egress8;			// output port
		Port &egress9;			// output port for next hub
		

		// Time varialbes
		Time preparationTime;
		Time timeLeft;
		// placeholder for value inside model.
		double localValue;
		double MAC;
		bool send; 
};
		//	inline methods
inline
string hub::className()const
{
	return "Hub";
}

#endif	//__HUB_H

