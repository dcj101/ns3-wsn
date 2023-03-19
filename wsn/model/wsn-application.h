#ifndef WSN_APPLICATION_H
#define WSN_APPLICATION_H

#include <stdint.h>
#include "ns3/simulator.h"
#include "ns3/event-id.h"
#include "ns3/nstime.h"
#include "ns3/object.h"
#include "ns3/ptr.h"
#include "ns3/node.h"
#include "ns3/application.h"
#include "ns3/core-module.h"

#include "wsn-nwk-short-address.h"
#include "wsn-sensor-tag.h"
#include "wsn-application-header.h"
#include "wsn-network-header.h"
#include "wsn-network-pl.h"




namespace ns3 
{

enum PROTOCOL_TYPE
{
  DATA_PROTOCOL = 0x00,
  COMMAND_PROTOCOL = 0x01,
};

class  WsnApplication : public Application
{
public:

    static TypeId GetTypeId (void);
    WsnApplication();
    ~WsnApplication();
    
    void SendData();
    

private:
    virtual void StartApplication ();
    virtual void StopApplication ();

    void SendSensorNodeInfo();

protected:
    virtual void DoDispose (void);
    virtual void DoInitialize (void);
  
  
    
};

}

#endif