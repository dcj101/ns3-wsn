#include "wsn-network.h"
#include "wsn-address-allocator.h"

namespace ns3
{
NS_LOG_COMPONENT_DEFINE ("WsnNwkProtocol");

NS_OBJECT_ENSURE_REGISTERED (WsnNwkProtocol);


TypeId 
WsnNwkProtocol::GetTypeId (void)
{
    static TypeId tid = TypeId ("ns3::WsnNwkProtocol")
                    .SetParent<Object> ()
                    .AddConstructor<WsnNwkProtocol>()
                    .AddTraceSource ("SendTrace",
                                    "A newly-generated packet by this node is "
                                    "about to be queued for transmission",
                                    MakeTraceSourceAccessor (&WsnNwkProtocol::m_sendTrace),
                                    "ns3::WsnNwkProtocol::SentTracedCallback")
                    .AddTraceSource ("UnicastForwardTrace",
                                    "A unicast Wsn packet was received by this node "
                                    "and is being forwarded to another node",
                                    MakeTraceSourceAccessor (&WsnNwkProtocol::m_unicastForwardTrace),
                                    "ns3::WsnNwkProtocol::SentTracedCallback")
                    .AddTraceSource ("LocalDeliverTrace",
                                    "An Wsn packet was received by/for this node, "
                                    "and it is being forward up the stack",
                                    MakeTraceSourceAccessor (&WsnNwkProtocol::m_localDeliverTrace),
                                    "ns3::WsnNwkProtocol::SentTracedCallback")
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
WsnNwkProtocol::Send(NwkShortAddress sourceaddr,  
                      NwkShortAddress dstaddr,Ptr<Packet> packet, 
                      NwkHeader::FrameType ftype
                      , bool isForward = 0)
{
  NS_LOG_INFO(this << Simulator::Now ().GetSeconds () << " source " << sourceaddr << " dst " << dstaddr << " ftype = " << ftype 
              << " is Forward = " << isForward);

  if(m_depth == -1) 
  {
    NS_LOG_ERROR("No Join Request");
  }

  if(!isForward)
      packet->RemoveAllByteTags();

  NwkHeader nwkHeader;
  nwkHeader.setType(ftype);
  nwkHeader.SetDestAddr(dstaddr);
  nwkHeader.SetSourceAddr(sourceaddr);
  packet->AddHeader(nwkHeader);

  if(!isForward)
      m_sendTrace(nwkHeader,packet);

  McpsDataRequestParams params;
  m_ack = 1;
  if(m_ack) 
    params.m_txOptions = TX_OPTION_ACK;
  params.m_dstPanId = 1;
  params.m_msduHandle = 0;
  params.m_srcAddrMode = EXT_ADDR;
  params.m_dstAddrMode = EXT_ADDR;
  
  NwkShortAddress nextHop = m_rtable.Lookup(dstaddr);
  
  if(nextHop.GetAddressU16() == 0x0000)
      nextHop = m_route;

  NS_LOG_FUNCTION(this << " ntable is " << nextHop.GetAddressU16());
  NeighborTable::NeighborEntry nextNeight = m_ntable.GetNeighborEntry(nextHop.GetAddressU16());

  params.m_dstExtAddr = nextNeight.extendedAddr;
  NS_LOG_FUNCTION(this << " ntable MAC addr is  " << nextNeight.extendedAddr);
  // m_netDevice->GetMac()->McpsDataRequest(params,packet);
  Simulator::Schedule(Seconds(0.0),
                      &LrWpanMac::McpsDataRequest,
                      m_netDevice->GetMac(),params,packet);
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
  NS_LOG_FUNCTION(this << " " << wsnNwkProtocol << m_nodeType);
  uint16_t addr;

  MlmeStartRequestParams params;
  NeighborTable* ntable;
  NwkShortAddress parents;
  Ptr<LrWpanNetDevice> netDevice;
  
  NeighborTable::NeighborEntry That;
  NeighborTable::NeighborEntry This;

  if(m_nodeType != NODE_TYPE::COOR)
  {
    m_depth = wsnNwkProtocol->GetDepth() + 1;
    m_panId = wsnNwkProtocol->GetPanID();

    ntable = wsnNwkProtocol->GetNeighborTable();
    parents = wsnNwkProtocol->GetNwkShortAddress();
    netDevice = wsnNwkProtocol->GetLrWpanNetDevice();
    
    addr = WsnAddressAllocator::Get()->AllocateNwkAddress(m_depth-1,IsRoute(),
                                                            parents.GetAddressU16());
    NS_LOG_FUNCTION(this << "addr is = " << addr);
    m_addr = std::move(NwkShortAddress(addr));

    m_route = parents;

    That.extendedAddr = netDevice->GetMac()->GetExtendedAddress();
    That.networkAddr = wsnNwkProtocol->GetNwkShortAddress();

    NS_LOG_FUNCTION(this << " that net addr is " << That.networkAddr << " " << That.extendedAddr);

    This.extendedAddr = m_netDevice->GetMac()->GetExtendedAddress();
    This.networkAddr = GetNwkShortAddress();

    NS_LOG_FUNCTION(this << " This net addr is " << This.networkAddr << " " << This.extendedAddr);

    m_ntable.AddNeighborEntry(That);
    ntable->AddNeighborEntry(This);

  }

  switch(m_nodeType)
  {
    case NODE_TYPE::EDGE :
      params.m_panCoor = false;
      params.m_PanId = m_panId;
      m_netDevice->GetMac ()->SetPanId (m_panId);
      m_netDevice->GetMac ()->SetAssociatedCoor(netDevice->GetMac()
                                                          ->GetAssociatedMac64AddressCoor());
    break;
    case NODE_TYPE::COOR :
      params.m_panCoor = true;
      params.m_PanId = 1;
      m_panId = 1;
      m_addr = std::move(NwkShortAddress("00:00"));
      m_depth = 0;
    break;
    case NODE_TYPE::ROUTE :
      params.m_panCoor = false;
      params.m_PanId = m_panId;
      m_netDevice->GetMac ()->SetPanId (m_panId);
      m_netDevice->GetMac ()->SetAssociatedCoor(netDevice->GetMac()
                                                          ->GetAssociatedMac64AddressCoor());
    break;
    default:
      break;
  }
  params.m_bcnOrd = 15; // 非时隙
    
  SetCallbackSet();
  
  Simulator::Schedule(Seconds(0.0),&LrWpanMac::MlmeStartRequest,
                        this->m_netDevice->GetMac(),params);
  
  if(m_nodeType != NODE_TYPE::COOR)
    Simulator::Schedule(Seconds(0.01),&WsnNwkProtocol::Send,
                        this,m_addr,parents,Create<Packet>(1),
                        NwkHeader::NWK_FRAME_COMMAND,0);
}

void 
WsnNwkProtocol::SetNodeType(NODE_TYPE type)
{
  m_nodeType = type;
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
  NS_LOG_FUNCTION(this << m_addr << " Get Addr");
  return m_addr.GetAddressU16();
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
    NS_LOG_UNCOND (m_addr << " " <<Simulator::Now ().GetSeconds () << " secs | Received BEACON packet of size " << p->GetSize ());
}

void
WsnNwkProtocol::DataIndication (McpsDataIndicationParams params, Ptr<Packet> p)
{    
    NS_LOG_FUNCTION(this);
    if(m_nodeType == NODE_TYPE::EDGE)
      NS_LOG_UNCOND (m_addr << " " <<Simulator::Now ().GetSeconds () << "s EDGE Received packet of size " << p->GetSize ());
    else if(m_nodeType == NODE_TYPE::COOR)
      NS_LOG_UNCOND (m_addr << " " <<Simulator::Now ().GetSeconds () << "s Coordinator Received packet (size " << p->GetSize () << " bytes)");
    else 
      NS_LOG_UNCOND (m_addr << " " <<Simulator::Now ().GetSeconds () << "s Route Received packet (size " << p->GetSize () << " bytes)");
    
    NwkHeader receiverNwkHeader;
    p->RemoveHeader(receiverNwkHeader);

    double Delay = 0.0;
    double gap = 0.01;

    if(m_nodeType != NODE_TYPE::EDGE)
    {
      
      if(receiverNwkHeader.GetType() == NwkHeader::NWK_FRAME_COMMAND)
      {
        m_localDeliverTrace(receiverNwkHeader,p);
        NS_LOG_UNCOND (m_addr << " " <<Simulator::Now ().GetSeconds () << "s Received Command packet of size " << p->GetSize ());
        std::vector<NeighborTable::NeighborEntry> ntable = m_ntable.GetNeighborEntries();
        for(auto it : ntable)
        {
          if(it.extendedAddr == params.m_srcExtAddr) 
          {
            NS_LOG_UNCOND (m_addr << " " <<Simulator::Now ().GetSeconds () << " secs update route ");
            // 更新路由表
            StaticRoute newRoute(receiverNwkHeader.GetSourceAddr(),it.networkAddr);
            m_rtable.AddRoute(newRoute);
            m_rtable.Print();
            continue;
          }
          // Send(receiverNwkHeader.GetSourceAddr(),it.networkAddr,p,NwkHeader::NWK_FRAME_COMMAND);
          Simulator::Schedule(Seconds(Delay),&WsnNwkProtocol::Send,
                        this,receiverNwkHeader.GetSourceAddr(),it.networkAddr
                        ,p,NwkHeader::NWK_FRAME_COMMAND,0);
          Delay += gap;
          NS_LOG_UNCOND (m_addr << " " <<Simulator::Now ().GetSeconds () << " send update route --->>>");
        }
      }
      else 
      {
        if(m_nodeType == NODE_TYPE::ROUTE || (receiverNwkHeader.GetDestAddr() != m_addr))
        {
          NS_LOG_UNCOND (m_addr << " " <<Simulator::Now ().GetSeconds () << " secs | Received DATA packet of size " << p->GetSize () << 
          ",but i am a " << m_nodeType << " m_addr is " << m_addr << " ,so i will forwarding packet");
          // 路由器转发数据包
          // Send(receiverNwkHeader.GetSourceAddr(),receiverNwkHeader.GetDestAddr(),p,NwkHeader::NWK_FRAME_DATA);
            m_unicastForwardTrace(receiverNwkHeader,p);
            Simulator::Schedule(Seconds(0.0),&WsnNwkProtocol::Send,
                  this,receiverNwkHeader.GetSourceAddr(),receiverNwkHeader.GetDestAddr(),
                  p,NwkHeader::NWK_FRAME_DATA,1);          
        }
        else 
        {
          m_localDeliverTrace(receiverNwkHeader,p);
          NS_LOG_UNCOND (m_addr << " " <<Simulator::Now ().GetSeconds () << " secs | Coor Received DATA packet of size " << p->GetSize ());
          //实现应用层回调 
        }
      }
    }
    else 
    {
      if(receiverNwkHeader.GetType() == NwkHeader::NWK_FRAME_COMMAND)
      {
        m_localDeliverTrace(receiverNwkHeader,p);
        // 报文抛弃 end Device
        NS_LOG_UNCOND (m_addr << " " <<Simulator::Now ().GetSeconds () << " secs | Received Command packet of size " << p->GetSize () << 
        ", but i am a EDGE ! my addr is = " << m_addr);
      }
      else 
      {
        m_localDeliverTrace(receiverNwkHeader,p);
        NS_LOG_UNCOND (m_addr << " " <<Simulator::Now ().GetSeconds () << " secs | Received DATA packet of size " << p->GetSize ()
        << " m_addr is " << m_addr);
        //实现应用层回调
      }
    }
}

void 
WsnNwkProtocol::TransEndIndication (McpsDataConfirmParams params)
{
    if (params.m_status == LrWpanMcpsDataConfirmStatus::IEEE_802_15_4_SUCCESS)
    {
        NS_LOG_UNCOND (m_addr << " " <<Simulator::Now ().GetSeconds () << " secs | Transmission successfully sent");
    }
}

void 
WsnNwkProtocol::DataIndicationCoordinator (McpsDataIndicationParams params, Ptr<Packet> p)
{
    NS_LOG_UNCOND (m_addr << " " <<Simulator::Now ().GetSeconds () << "s Coordinator Received DATA packet (size " << p->GetSize () << " bytes)");
}

void 
WsnNwkProtocol::StartConfirm (MlmeStartConfirmParams params)
{
    if (params.m_status == MLMESTART_SUCCESS)
    {
        NS_LOG_UNCOND (m_addr << " " <<Simulator::Now ().GetSeconds () << "Beacon status SUCESSFUL");
    }
}

void 
WsnNwkProtocol::SetCallbackSet()
{
    NS_LOG_FUNCTION(this);
    if(m_nodeType == NODE_TYPE::COOR)
    { 
      m_MlmeStartConfirmCallback = MakeCallback (&WsnNwkProtocol::StartConfirm,this);
      m_netDevice->GetMac ()->SetMlmeStartConfirmCallback (m_MlmeStartConfirmCallback);
    } 
    else
    {
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

void
WsnNwkProtocolHelper::CreateAndAggregateObjectFromTypeId (NodeContainer container, const std::string typeId)
{
  NS_LOG_FUNCTION(this << " " << typeId);
  NodeContainer::Iterator i;
  for (i = container.Begin (); i != container.End (); ++i)
  {
    ObjectFactory factory;
    factory.SetTypeId (typeId);
    Ptr<Object> protocol = factory.Create <Object> ();
    (*i)->AggregateObject (protocol);
  }
}

void
WsnNwkProtocolHelper::CreateAndAggregateObjectFromTypeId (Ptr<Node> node, const std::string typeId)
{
  NS_LOG_FUNCTION(this << " " << typeId);
  ObjectFactory factory;
  factory.SetTypeId (typeId);
  Ptr<Object> protocol = factory.Create <Object> ();
  node->AggregateObject (protocol);
}

}