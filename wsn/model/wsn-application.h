#ifndef WSN_APPLICATION_H
#define WSN_APPLICATION_H

#include <stdint.h>

#include "ns3/event-id.h"
#include "ns3/nstime.h"
#include "ns3/object.h"
#include "ns3/ptr.h"
#include "ns3/node.h"
#include "ns3/application.h"
#include "ns3/core-module.h"

#include "wsn-sensor-node.h"
#include "wsn-application-header.h"


namespace ns3 
{


class WsnApplication : public Application
{
public:
    static TypeId GetTypeId (void);
    WsnApplication();
    ~WsnApplication();

private:
    virtual void StartApplication ();
    virtual void StopApplication ();

    void SendSensorNodeInfo();
    
};

}

#endif