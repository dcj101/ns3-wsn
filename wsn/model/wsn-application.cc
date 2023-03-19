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
  m_point = 0;
}

WsnApplication::~WsnApplication()
{

}

void 
WsnApplication::SendData()
{
  NS_LOG_FUNCTION (this);
  AppHeader appHeader;
  m_packet->AddHeader(appHeader);
  
  WsnSensorTag sensor;
  m_packet->AddPacketTag(sensor);
  

}

void 
WsnApplication::SetRemote(NwkShortAddress addr, uint8_t point)
{
  m_peerAddr = addr;
  m_point = point;
}

void 
WsnApplication::SetRemote(NwkShortAddress addr)
{
  m_peerAddr = addr;
}

void 
WsnApplication::SetNode(Ptr<Node> node) 
{
  m_node = node;
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