
#include "wsn-flow-probe.h"

namespace ns3
{


class WsnFlowProbeTag : public Tag
{
public:

  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (TagBuffer buf) const;
  virtual void Deserialize (TagBuffer buf);
  virtual void Print (std::ostream &os) const;
  WsnFlowProbeTag ();

  WsnFlowProbeTag (uint32_t flowId, uint32_t packetId, uint32_t packetSize, NwkShortAddress src, NwkShortAddress dst);

  void SetFlowId (uint32_t flowId);

  void SetPacketId (uint32_t packetId);

  void SetPacketSize (uint32_t packetSize);

  uint32_t GetFlowId (void) const;

  uint32_t GetPacketId (void) const;

  uint32_t GetPacketSize (void) const;

  bool IsSrcDstValid (NwkShortAddress src, NwkShortAddress dst) const;
private:
  uint32_t m_flowId;      //!< flow identifier
  uint32_t m_packetId;    //!< packet identifier
  uint32_t m_packetSize;  //!< packet size
  NwkShortAddress m_src;      //!< IP source
  NwkShortAddress m_dst;      //!< IP destination
};


TypeId 
WsnFlowProbeTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::WsnFlowProbeTag")
    .SetParent<Tag> ()
    .SetGroupName ("FlowMonitor")
    .AddConstructor<WsnFlowProbeTag> ()
  ;
  return tid;
}
TypeId 
WsnFlowProbeTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}
uint32_t 
WsnFlowProbeTag::GetSerializedSize (void) const
{
  return 4 + 4 + 4 + 8;
}
void 
WsnFlowProbeTag::Serialize (TagBuffer buf) const
{
  buf.WriteU32 (m_flowId);
  buf.WriteU32 (m_packetId);
  buf.WriteU32 (m_packetSize);

  uint8_t tBuf[4];
  m_src.Serialize (tBuf);
  buf.Write (tBuf, 4);
  m_dst.Serialize (tBuf);
  buf.Write (tBuf, 4);
}
void 
WsnFlowProbeTag::Deserialize (TagBuffer buf)
{
  m_flowId = buf.ReadU32 ();
  m_packetId = buf.ReadU32 ();
  m_packetSize = buf.ReadU32 ();

  uint8_t tBuf[4];
  buf.Read (tBuf, 4);
  m_src = NwkShortAddress::Deserialize (tBuf);
  buf.Read (tBuf, 4);
  m_dst = NwkShortAddress::Deserialize (tBuf);
}
void 
WsnFlowProbeTag::Print (std::ostream &os) const
{
  os << "FlowId=" << m_flowId;
  os << " PacketId=" << m_packetId;
  os << " PacketSize=" << m_packetSize;
}
WsnFlowProbeTag::WsnFlowProbeTag ()
  : Tag () 
{
}

WsnFlowProbeTag::WsnFlowProbeTag (uint32_t flowId, uint32_t packetId, uint32_t packetSize, NwkShortAddress src, NwkShortAddress dst)
  : Tag (), m_flowId (flowId), m_packetId (packetId), m_packetSize (packetSize), m_src (src), m_dst (dst)
{
}

void
WsnFlowProbeTag::SetFlowId (uint32_t id)
{
  m_flowId = id;
}
void
WsnFlowProbeTag::SetPacketId (uint32_t id)
{
  m_packetId = id;
}
void
WsnFlowProbeTag::SetPacketSize (uint32_t size)
{
  m_packetSize = size;
}
uint32_t
WsnFlowProbeTag::GetFlowId (void) const
{
  return m_flowId;
}
uint32_t
WsnFlowProbeTag::GetPacketId (void) const
{
  return m_packetId;
} 
uint32_t
WsnFlowProbeTag::GetPacketSize (void) const
{
  return m_packetSize;
}
bool
WsnFlowProbeTag::IsSrcDstValid (NwkShortAddress src, NwkShortAddress dst) const
{
  return ((m_src == src) && (m_dst == dst));
}

// ------------------------------------
// ------------------------------------
// -------WsnFlowProbe-------------
// -------WsnFlowProbe------------------
// ------------------------------------
// ------------------------------------
// ------------------------------------


WsnFlowProbe::WsnFlowProbe (Ptr<FlowMonitor> monitor,
                              Ptr<WsnFlowClassifier> classifier,
                              Ptr<Node> node)
  : FlowProbe (monitor),
    m_classifier (classifier)
{
  NS_LOG_FUNCTION (this << node->GetId ());

  m_wsnNwkProtocol = node->GetObject<WsnNwkProtocol> ();

  if (!m_wsnNwkProtocol->TraceConnectWithoutContext ("SendOutgoing",
                                           MakeCallback (&WsnFlowProbe::SendOutgoingLogger, Ptr<WsnFlowProbe> (this))))
    {
      NS_FATAL_ERROR ("trace fail");
    }
}


void
WsnFlowProbe::DoDispose ()
{
  m_wsnNwkProtocol = 0;
  m_classifier = 0;
  FlowProbe::DoDispose ();
}


void
WsnFlowProbe::SendOutgoingLogger (const NwkHeader &ipHeader, Ptr<const Packet> ipPayload, uint32_t interface)
{
  FlowId flowId;
  FlowPacketId packetId;

  if (!m_Wsn->IsUnicast(ipHeader.GetDestination ()))
    {
      return;
    }

  WsnFlowProbeTag fTag;
  bool found = ipPayload->FindFirstMatchingByteTag (fTag);
  if (found)
    {
      return;
    }

  if (m_classifier->Classify (ipHeader, ipPayload, &flowId, &packetId))
    {
      uint32_t size = (ipPayload->GetSize () + ipHeader.GetSerializedSize ());
      NS_LOG_DEBUG ("ReportFirstTx ("<<this<<", "<<flowId<<", "<<packetId<<", "<<size<<"); "
                                     << ipHeader << *ipPayload);

      m_flowMonitor->ReportFirstTx (this, flowId, packetId, size);

      WsnFlowProbeTag fTag (flowId, packetId, size, ipHeader.GetSourceAddr (), ipHeader.GetDestAddr ());
      ipPayload->AddByteTag (fTag);
    }
}

}   