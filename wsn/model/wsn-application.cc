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
  m_point = 31;
}

WsnApplication::~WsnApplication()
{

}

void 
WsnApplication::SendData()
{
  NS_LOG_FUNCTION (this);

  Ptr<Packet> m_packet;

  AppHeader appHeader;
  
  m_packet->AddHeader(appHeader);
  
  WsnSensorTag sensor;
  m_packet->AddPacketTag(sensor);
  
  Ptr<WsnNwkProtocol> wsnnwk = m_node->GetObject<WsnNwkProtocol> ();

  wsnnwk->Send(wsnnwk->GetNwkShortAddress(),m_peerAddr,m_packet,NwkHeader::NWK_FRAME_DATA);
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