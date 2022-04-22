/*
 * A model of the application layer, their should 
 * only be one per complex host coupled model.
 * this model is only interested in applications running TCP
 * By		: Mohamed Mohamed Abd El-Salam Ahmed
 * Contact	: mohamedmahmed@islamway.net	
 **/
#ifndef __APPLICATION_H
#define __APPLICATION_H

#include "atomic.h"

class Application : public Atomic
{
	public:
		Application( const string &name = "Application" );
		virtual string className()const;
	protected:
		
		Model &initFunction();
		Model &externalFunction( const ExternalMessage &msg );
		Model &internalFunction( const InternalMessage & );
		Model &outputFunction( const InternalMessage &msg );
	private:
		//	Ports:
		const Port &in;	// general application input port, from TCP
		const Port &HTTP_In;
		const Port &FTP_In;
		const Port &SNMP_In;
		const Port &SMTP_In;
		const Port &Telnet_In;
		//	output ports
		Port &ApplicationOut;	// general appliaction output port, to TCP
                Port &RootOut;  // Application Root Access to output incoming data from other    
		
// Time varialbes

		Time preparationTime;
		Time timeLeft;
		// placeholder for value inside model.
		double value, port;
		bool flag;
};
		//	inline methods
inline
string Application::className()const
{
	return "Application";
}

#endif	//__APPLICATION_H