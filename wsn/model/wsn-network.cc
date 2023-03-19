#include "wsn-network.h"

namespace ns3
{
NS_LOG_COMPONENT_DEFINE ("WsnNwkProtocol");


TypeId 
WsnNwkProtocol::GetTypeId (void)
{
    static TypeId tid = TypeId ("ns3::WsnNwkProtocol")
                    .SetParent<Object> ()
                    .AddConstructor<WsnNwkProtocol> ()
    ;
    return tid;
}

NwkHeader 
WsnNwkProtocol::BuildHeader(NwkShortAddress addr)
{
    
    return NwkHeader();
}

void
WsnNwkProtocol::BuildRtable(std::vector<StaticRoute>&rtable)
{
    for(std::vector<StaticRoute>::iterator begin = rtable.begin()
                                ;begin != rtable.end(); ++ begin)
    {
        m_rtable.AddRoute(*begin);
    }
}

void 
WsnNwkProtocol::Install(Ptr<Node> node)
{
    m_node = node;
    CreateAndAggregateObjectFromTypeId(m_node,"WsnNwkProtocol");
}


void
WsnNwkProtocol::CreateAndAggregateObjectFromTypeId (Ptr<Node> node, const std::string typeId)
{
  ObjectFactory factory;
  factory.SetTypeId (typeId);
  Ptr<Object> protocol = factory.Create <Object> ();
  node->AggregateObject (protocol);
}

void 
WsnNwkProtocol::Assign(Ptr<LrWpanNetDevice> netDevice, NwkShortAddress addr)
{
    m_netDevice = netDevice;
    m_addr = addr;
}

void 
WsnNwkProtocol::SetNodeType(uint8_t type)
{
  m_nodeType = type;
}

void 
WsnNwkProtocol::SetDepth(uint8_t depth)
{
  m_depth = depth;
}

void 
WsnNwkProtocol::SetNode(Ptr<Node> node)
{
    NS_LOG_FUNCTION (this << node);
    m_node = node;
}

void 
WsnNwkProtocol::NotifyNewAggregate (void)
{
  NS_LOG_FUNCTION (this);
    if (m_node == 0)
    {
      Ptr<Node>node = this->GetObject<Node> ();
      if (node != 0)
        {
          this->SetNode (node);
        }
    }
  Object::NotifyNewAggregate ();
}

void 
WsnNwkProtocol::DoInitialize (void)
{
    MlmeStartConfirmCallback cb0;
    cb0 = MakeCallback (&StartConfirm);
    m_netDevice->GetMac ()->SetMlmeStartConfirmCallback (cb0);

    
    McpsDataConfirmCallback cb1;
    cb1 = MakeCallback (&TransEndIndication);
    m_netDevice->GetMac ()->SetMcpsDataConfirmCallback (cb1);

    MlmeBeaconNotifyIndicationCallback cb3;
    cb3 = MakeCallback (&BeaconIndication);
    m_netDevice->GetMac ()->SetMlmeBeaconNotifyIndicationCallback (cb3);

    McpsDataIndicationCallback cb4;
    cb4 = MakeCallback (&DataIndication);
    m_netDevice->GetMac ()->SetMcpsDataIndicationCallback (cb4);

    McpsDataIndicationCallback cb5;
    cb5 = MakeCallback (&DataIndicationCoordinator);
    m_netDevice->GetMac ()->SetMcpsDataIndicationCallback (cb5);

}



void 
WsnNwkProtocol::DoDispose (void)
{
    NS_LOG_FUNCTION (this);
    m_node = 0;
    Object::DoDispose ();
}


}