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
                    .AddConstructor<WsnNwkProtocol>()
                    ;
    return tid;
}

WsnNwkProtocol::WsnNwkProtocol()
{
  NS_LOG_INFO(this);
  m_depth = -1;
}

WsnNwkProtocol::WsnNwkProtocol(NODE_TYPE type)
{
  NS_LOG_INFO(this);
  m_depth = -1;
  m_nodeType = type;
}


void 
WsnNwkProtocol::Send(NwkShortAddress sourceaddr, NwkShortAddress dstaddr,Ptr<Packet> packet, NwkHeader::FrameType ftype)
{
  NS_LOG_INFO(this << Simulator::Now ().GetSeconds () << " " << sourceaddr << " " << dstaddr);
  if(m_depth == -1) 
  {
    NS_LOG_ERROR("No Join Request");
  }
  
  NwkHeader nwkHeader;
  nwkHeader.setType(ftype);
  nwkHeader.SetDestAddr(dstaddr);
  nwkHeader.SetSourceAddr(sourceaddr);
  packet->AddHeader(nwkHeader);
  
  McpsDataRequestParams params;
  
  if(m_ack) 
    params.m_txOptions = TX_OPTION_ACK;
  params.m_dstPanId = 1;
  params.m_msduHandle = 0;
  params.m_srcAddrMode = EXT_ADDR;
  params.m_dstAddrMode = EXT_ADDR;
  
  NwkShortAddress nextHop = m_rtable.Lookup(dstaddr);
  
  if(nextHop.GetAddressU16() == 0x0000)
      nextHop = m_route;
  
  NeighborTable::NeighborEntry nextNeight = m_ntable.GetNeighborEntry(nextHop.GetAddressU16());

  params.m_dstExtAddr = nextNeight.extendedAddr;
  NS_LOG_FUNCTION(this << " Send dst extmac" << params.m_dstExtAddr );
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
    CreateAndAggregateObjectFromTypeId(m_node,"ns3::WsnNwkProtocol");
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
WsnNwkProtocol::Assign(Ptr<LrWpanNetDevice> netDevice)
{
    NS_LOG_FUNCTION(this << netDevice);
    m_netDevice = netDevice;
}

void 
WsnNwkProtocol::JoinRequest(Ptr<WsnNwkProtocol> wsnNwkProtocol)
{
  NS_LOG_FUNCTION(this << " " << wsnNwkProtocol);
  uint8_t depth;
  uint8_t panid;
  
  MlmeStartRequestParams params;
  NeighborTable* ntable;
  NwkShortAddress parents;
  Ptr<LrWpanNetDevice> netDevice;
  
  NeighborTable::NeighborEntry That;
  NeighborTable::NeighborEntry This;

  if(m_nodeType != NODE_TYPE::COOR)
  {
    NS_LOG_FUNCTION(this << "is not COOR");
    ntable = wsnNwkProtocol->GetNeighborTable();
    parents = wsnNwkProtocol->GetNwkShortAddress();
    netDevice = wsnNwkProtocol->GetLrWpanNetDevice();


    That.extendedAddr = netDevice->GetMac()->GetExtendedAddress();
    That.networkAddr = wsnNwkProtocol->GetNwkShortAddress();

    This.extendedAddr = m_netDevice->GetMac()->GetExtendedAddress();
    This.networkAddr = GetNwkShortAddress();

    m_ntable.AddNeighborEntry(That);
    ntable->AddNeighborEntry(This);

    depth = wsnNwkProtocol->GetDepth();
    panid = wsnNwkProtocol->GetPanID();
  }
  NS_LOG_FUNCTION(this << panid << " mypanid" );
  uint16_t addr;

  switch(m_nodeType)
  {
    case NODE_TYPE::EDGE :
      params.m_panCoor = false;
      params.m_PanId = panid;
      m_netDevice->GetMac ()->SetPanId (panid);
      m_netDevice->GetMac ()->SetAssociatedCoor(netDevice->GetMac()
                                                          ->GetAssociatedMac64AddressCoor());
      addr = WsnAddressAllocator::Get()->AllocateNwkAddress(depth,0,
                                                            parents.GetAddressU16());
      NS_LOG_FUNCTION(this << " EDGE " << addr);
      m_addr = std::move(NwkShortAddress(addr));
      m_route = parents;
      m_depth = depth + 1;
    break;
    case NODE_TYPE::COOR :
      params.m_panCoor = true;
      params.m_PanId = 1;
      panid = params.m_PanId;
      m_addr = std::move(NwkShortAddress("00:00"));
      m_depth = 0;
    break;
    case NODE_TYPE::ROUTE :
      params.m_panCoor = false;
      params.m_PanId = panid;
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
  m_panId = panid;
  Simulator::Schedule(Seconds(0.0),&LrWpanMac::MlmeStartRequest,
                        this->m_netDevice->GetMac(),params);
  SetCallbackSet();
  if(m_nodeType != NODE_TYPE::COOR)
    Simulator::Schedule(Seconds(5),&WsnNwkProtocol::Send,
                        this,m_addr,parents,Create<Packet>(1),
                        NwkHeader::NWK_FRAME_COMMAND);
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
  NS_LOG_FUNCTION(this << " getmypanID" << m_panId);
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
WsnNwkProtocol::BeaconIndication (MlmeBeaconNotifyIndicationParams params, Ptr<Packet> p)
{
    NS_LOG_UNCOND (Simulator::Now ().GetSeconds () << " secs | Received BEACON packet of size " << p->GetSize ());
}

void
WsnNwkProtocol::DataIndication (McpsDataIndicationParams params, Ptr<Packet> p)
{
    NS_LOG_FUNCTION(this);
    if(m_depth > 0)
      NS_LOG_UNCOND (Simulator::Now ().GetSeconds () << " secs | Received packet of size " << p->GetSize ());
    else 
      NS_LOG_UNCOND (Simulator::Now ().GetSeconds () << "s Coordinator Received DATA packet (size " << p->GetSize () << " bytes)");

    NwkHeader receiverNwkHeader;
    p->RemoveHeader(receiverNwkHeader);

    if(m_nodeType != NODE_TYPE::EDGE)
    {
      if(receiverNwkHeader.GetType() == NwkHeader::NWK_FRAME_COMMAND)
      {
        std::vector<NeighborTable::NeighborEntry> ntable = m_ntable.GetNeighborEntries();
        for(auto it : ntable)
        {
          if(it.extendedAddr == params.m_srcExtAddr) 
          {
            // 更新路由表
            StaticRoute newRoute(receiverNwkHeader.GetSourceAddr(),it.networkAddr);
            m_rtable.AddRoute(newRoute);
            continue;
          }
          Send(receiverNwkHeader.GetSourceAddr(),receiverNwkHeader.GetDestAddr(),p,NwkHeader::NWK_FRAME_COMMAND);
        }
      }
      else 
      {
        // 路由器转发数据包
        Send(receiverNwkHeader.GetSourceAddr(),receiverNwkHeader.GetDestAddr(),p,NwkHeader::NWK_FRAME_DATA);
      }
    }
    else 
    {
      if(receiverNwkHeader.GetType() == NwkHeader::NWK_FRAME_COMMAND)
      {
        // 报文抛弃 end Device
      }
      else 
      {
        NS_LOG_UNCOND (Simulator::Now ().GetSeconds () << " secs | Received DATA packet of size " << p->GetSize ());
        //实现应用层回调
      }
    }
}

void 
WsnNwkProtocol::TransEndIndication (McpsDataConfirmParams params)
{
    if (params.m_status == LrWpanMcpsDataConfirmStatus::IEEE_802_15_4_SUCCESS)
    {
        NS_LOG_UNCOND (Simulator::Now ().GetSeconds () << " secs | Transmission successfully sent");
    }
}

void 
WsnNwkProtocol::DataIndicationCoordinator (McpsDataIndicationParams params, Ptr<Packet> p)
{
    NS_LOG_UNCOND (Simulator::Now ().GetSeconds () << "s Coordinator Received DATA packet (size " << p->GetSize () << " bytes)");
}

void 
WsnNwkProtocol::StartConfirm (MlmeStartConfirmParams params)
{
    if (params.m_status == MLMESTART_SUCCESS)
    {
        NS_LOG_UNCOND (Simulator::Now ().GetSeconds () << "Beacon status SUCESSFUL");
    }
}

void 
WsnNwkProtocol::SetCallbackSet()
{
    NS_LOG_FUNCTION(this);
    if(m_nodeType == NODE_TYPE::COOR)
    { 
      NS_LOG_FUNCTION(this << "m_netDevice is --- " << m_netDevice->GetMac());
      m_MlmeStartConfirmCallback = MakeCallback (&WsnNwkProtocol::StartConfirm,this);
      m_netDevice->GetMac ()->SetMlmeStartConfirmCallback (m_MlmeStartConfirmCallback);
    } 
    else if(m_nodeType == NODE_TYPE::EDGE)
    {
      NS_LOG_FUNCTION(this << "m_netDevice is " << &m_netDevice);
      m_McpsDataConfirmCallback = MakeCallback (&WsnNwkProtocol::TransEndIndication,this);
      m_netDevice->GetMac ()->SetMcpsDataConfirmCallback (m_McpsDataConfirmCallback);

      m_MlmeBeaconNotifyIndicationCallback = MakeCallback (&WsnNwkProtocol::BeaconIndication,this);
      m_netDevice->GetMac ()->SetMlmeBeaconNotifyIndicationCallback (m_MlmeBeaconNotifyIndicationCallback);
    } 
     m_McpsDataIndicationCallback = MakeCallback (&WsnNwkProtocol::DataIndication,this);
     m_netDevice->GetMac ()->SetMcpsDataIndicationCallback (m_McpsDataIndicationCallback);
}

void 
WsnNwkProtocol::DoInitialize (void)
{

}

void 
WsnNwkProtocol::DoDispose (void)
{
    NS_LOG_FUNCTION (this);
    m_node = 0;
    Object::DoDispose ();
}


}