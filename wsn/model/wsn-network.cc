#include "wsn-network.h"
#include "wsn-address-allocator.h"

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

WsnNwkProtocol::WsnNwkProtocol()
{
  m_depth = -1;
  DoInitialize();
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
  params.m_dstExtAddr = Mac64Address(extAddr64.c_str());

  Simulator::Schedule(Seconds(0.0),
                      &LrWpanMac::McpsDataRequest,
                      m_netDevice->GetMac(),params,packet);

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
    m_netDevic e = netDevice;
    m_addr = addr;
}

void 
WsnNwkProtocol::JoinRequest(NODE_TYPE type, Ptr<WsnNwkProtocol> wsnNwkProtocol)
{
  // static const uint8_t PanID = 1;
  m_nodeType = type;
  MlmeStartRequestParams params;
  // RoutingTable * rtable = wsnNwkProtocol->GetRoutingTable();
  // NeighborTable * ntable = wsnNwkProtocol->GetNeighborTable();
  NwkShortAddress parents = wsnNwkProtocol->GetNwkShortAddress();
  Ptr<LrWpanNetDevice> netDevice = wsnNwkProtocol->GetLrWpanNetDevice();

  uint8_t depth = wsnNwkProtocol->GetDepth();
  uint8_t panid = wsnNwkProtocol->GetPanID();
  uint16_t addr;

  switch(m_nodeType)
  {
    case NODE_TYPE::EDGE:
      params.m_panCoor = false;
      m_netDevice->GetMac ()->SetPanId (panid);
      m_netDevice->GetMac ()->SetAssociatedCoor(netDevice->GetMac()
                                                          ->GetAssociatedMac64AddressCoor());
      addr = WsnAddressAllocator::Get()->AllocateNwkAddress(depth,0,
                                                            parents.GetAddressU16());
      m_addr = std::move(NwkShortAddress(addr));
      m_route = parents;
      m_depth = depth + 1;
    break;
    case NODE_TYPE::COOR:
      params.m_panCoor = true;
      params.m_PanId = 1;
      // params.m_sfrmOrd = 6;
      m_addr = std::move(NwkShortAddress("00:00"));
      m_depth = 0;
      break;
    case NODE_TYPE::ROUTE:
      params.m_panCoor = false;
      m_netDevice->GetMac ()->SetPanId (panid);
      m_netDevice->GetMac ()->SetAssociatedCoor(netDevice->GetMac()
                                                          ->GetAssociatedMac64AddressCoor());
      addr = WsnAddressAllocator::Get()->AllocateNwkAddress(depth,1,
                                                                  parents.GetAddressU16());
      m_addr = std::move(NwkShortAddress(addr));
      m_route = parents;
      m_depth = depth + 1;
      break;
    default:
      break;
  }
  params.m_bcnOrd = 15; // 非时隙
  Simulator::Schedule(Seconds(0.0),&LrWpanMac::MlmeStartRequest,
                        this->m_netDevice->GetMac(),params);
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

uint8_t
WsnNwkProtocol::GetPanID()
{
  return m_panId;
}

uint8_t
WsnNwkProtocol::GetDepth()
{
  return m_depth;
}

NwkShortAddress 
WsnNwkProtocol::GetNwkShortAddress()
{
  return m_addr;
}

Ptr<LrWpanNetDevice> 
WsnNwkProtocol::GetLrWpanNetDevice()
{
  return m_netDevice;
}

RoutingTable*
WsnNwkProtocol::GetRoutingTable()
{
  return &m_rtable;
}

NeighborTable* 
WsnNwkProtocol::GetNeighborTable()
{
  return &m_ntable;
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
    m_McpsDataIndicationCallback = MakeCallback (&DataIndicationCoordinator);
    m_netDevice->GetMac ()->SetMcpsDataIndicationCallback (m_McpsDataIndicationCallback);
    if(m_nodeType == NODE_TYPE::COOR)
    {
      m_MlmeStartConfirmCallback = MakeCallback (&StartConfirm);
      m_netDevice->GetMac ()->SetMlmeStartConfirmCallback (m_MlmeStartConfirmCallback);
    } 
    else if(m_nodeType == NODE_TYPE::EDGE)
    {
      m_McpsDataConfirmCallback = MakeCallback (&TransEndIndication);
      m_netDevice->GetMac ()->SetMcpsDataConfirmCallback (m_McpsDataConfirmCallback);

      m_MlmeBeaconNotifyIndicationCallback = MakeCallback (&BeaconIndication);
      m_netDevice->GetMac ()->SetMlmeBeaconNotifyIndicationCallback (m_MlmeBeaconNotifyIndicationCallback);
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