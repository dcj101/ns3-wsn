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

void 
WsnNwkProtocol::SendData(NwkShortAddress dstaddr,Ptr<Packet> packet)
{
  NwkHeader nwkHeader;
  nwkHeader.SetDestAddr(dstaddr);
  nwkHeader.SetSourceAddr(m_addr);
  packet->AddHeader(nwkHeader);
  
  McpsDataRequestParams params;
  
  if(m_ack) 
    params.m_txOptions = TX_OPTION_ACK;
  
  params.m_srcAddrMode = EXT_ADDR;
  params.m_dstAddrMode = EXT_ADDR;
  
  NwkShortAddress nextHop = m_rtable.Lookup(dstaddr);
  
  if(nextHop.GetAddressU16() == 0x0000)
      nextHop = m_route;
  
  NeighborTable::NeighborEntry nextNeight = m_ntable.GetNeighborEntry(nextHop.GetAddressU16());

  std::string extAddr64 = WsnAddressAllocator::Get ()->FormatOutPut(nextNeight.extendedAddr,7);
  std::string extAddr48 = WsnAddressAllocator::Get ()->toMAC48(extAddr64);
  params.m_dstExtAddr = Mac48Address(extAddr48.c_str());

  

  return;
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
WsnNwkProtocol::SetNodeType(NODE_TYPE type)
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
WsnNwkProtocol::SetAck(bool ack)
{
  m_ack = ack;
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
    if(m_nodeType == NODE_TYPE::COOR)
    {
      cb4 = MakeCallback (&DataIndicationCoordinator);
      m_netDevice->GetMac ()->SetMcpsDataIndicationCallback (cb4);

      cb0 = MakeCallback (&StartConfirm);
      m_netDevice->GetMac ()->SetMlmeStartConfirmCallback (cb0);
    } 
    else if(m_nodeType == NODE_TYPE::EDGE)
    {
      cb4 = MakeCallback (&DataIndication);
      m_netDevice->GetMac ()->SetMcpsDataIndicationCallback (cb4);

      cb1 = MakeCallback (&TransEndIndication);
      m_netDevice->GetMac ()->SetMcpsDataConfirmCallback (cb1);

      cb3 = MakeCallback (&BeaconIndication);
      m_netDevice->GetMac ()->SetMlmeBeaconNotifyIndicationCallback (cb3);
    } 

}



void 
WsnNwkProtocol::DoDispose (void)
{
    NS_LOG_FUNCTION (this);
    m_node = 0;
    Object::DoDispose ();
}


}