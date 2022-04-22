/*******************************************************************
*
*  DESCRIPTION: Simulator::registerNewAtomics()
*
*  AUTHOR: Amir Barylko & Jorge Beyoglonian 
*
*  EMAIL: mailto://amir@dc.uba.ar
*         mailto://jbeyoglo@dc.uba.ar
*
*  DATE: 27/6/1998
*
*******************************************************************/

#include "modeladm.h" 
#include "mainsimu.h"
#include "queue.h"      // class Queue
#include "generat.h"    // class Generator
//-----------------------------------------------------------------------
#include "Application.h"    // class Application
#include "datagramCreator.h"    // class datagramCreator
#include "datagramStripper.h"    // class datagramStripper
#include "checksumCreator.h"    // class checksumCreator
#include "checksumValidator.h"    // class checksumVerifier
#include "networkTransmit.h"    // class networkTransmit
#include "Receiver.h"    // class Receiver
#include "datalink.h"
#include "physical.h"
#include "test2.h"
#include "PacketProcessor.h"
#include "ripTable2.h"
#include "ripTable.h"
#include "ripTable3.h"
#include "tableUpdate.h"
#include "RouterOutput.h"
#include "RouterIn.h"

void MainSimulator::registerNewAtomics()
{
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<Queue>() , "Queue" ) ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<Generator>() , "Generator" ) ;

	//--------------------------------------------------------------------------------------
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<Application>() , "Application" ) ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<datagramStripper>() , "datagramStripper" ) ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<datagramCreator>() , "datagramCreator" ) ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<checksumCreator>() , "checksumCreator" ) ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<checksumValidator>() , "checksumValidator" ) ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<networkTransmit>() , "networkTransmit" ) ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<Receiver>() , "Receiver" ) ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<dataLink>() , "dataLink" ) ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<physical>() , "physical" ) ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<test2>() , "test2" ) ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<PacketProcessor>() , "PacketProcessor" ) ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<ripTable2>() , "ripTable2" ) ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<ripTable>() , "ripTable" ) ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<ripTable3>() , "ripTable3" ) ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<tableUpdate>() , "tableUpdate" ) ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<RouterOutput>() , "RouterOutput" ) ;
	SingleModelAdm::Instance().registerAtomic( NewAtomicFunction<RouterIn>() , "RouterIn" ) ;


}
