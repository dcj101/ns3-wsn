#include "wsn-flow-probe-tag.h"

namespace ns3
{

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
  buf.WriteU16(m_src.GetAddressU16());
  buf.WriteU16(m_dst.GetAddressU16());
}
void 
WsnFlowProbeTag::Deserialize (TagBuffer buf)
{
  m_flowId = buf.ReadU32 ();
  m_packetId = buf.ReadU32 ();
  m_packetSize = buf.ReadU32 ();
  m_src = NwkShortAddress(buf.ReadU16());
  m_dst = NwkShortAddress(buf.ReadU16());
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

}