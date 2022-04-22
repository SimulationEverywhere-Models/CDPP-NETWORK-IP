#ifndef __test2_H
#define __test2_H

#include "atomic.h"

class test2 : public Atomic
{
	public:
		test2( const string &name = "test2" );
		virtual string className()const;
	protected:

		Model &initFunction();
		Model &externalFunction( const ExternalMessage & );
		Model &internalFunction( const InternalMessage & );
		Model &outputFunction( const InternalMessage & );
	private:
		// Ports:
      const Port &poll;
		Port &status;

		bool sendJamming,sendStatus;
		int busy;
		// Time varialbes
		Time preparationTime;

   };

		//	inline methods
inline
string test2::className()const
{
	return "test2";
}

#endif