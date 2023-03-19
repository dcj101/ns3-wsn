#include "wsn-application.h"
#include <ns3/log.h>

namespace ns3
{
NS_LOG_COMPONENT_DEFINE ("WsnApplication");

NS_OBJECT_ENSURE_REGISTERED (WsnApplication);

TypeId 
WsnApplication::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::WsnApplication")
    .SetParent<Application> ()
    .AddConstructor<WsnApplication> ()
    ;
    return tid;
}

WsnApplication::WsnApplication()
{

}

WsnApplication::~WsnApplication()
{

}

void WsnApplication::SendData()
{
  NS_LOG_FUNCTION (this);
  
}


void 
WsnApplication::StartApplication() 
{
  NS_LOG_FUNCTION (this);
  Simulator::Schedule (Seconds (1), &ns3::WsnApplication::SendSensorNodeInfo,this);
}

void 
WsnApplication::StopApplication()
{
  NS_LOG_FUNCTION (this);

}

void 
WsnApplication::SendSensorNodeInfo()
{
  NS_LOG_FUNCTION (this);
  Simulator::Schedule (Seconds (1), &ns3::WsnApplication::SendSensorNodeInfo,this);
}

void 
WsnApplication::DoDispose(void)
{
  NS_LOG_FUNCTION (this);


} 

void 
WsnApplication::DoInitialize (void)
{
  NS_LOG_FUNCTION (this);
  
}

}