#include "wsn-network-header.h"
#include <ns3/log.h>


namespace ns3
{

NS_LOG_COMPONENT_DEFINE ("NwkHeader");
//............NwkHeader....................//


NS_OBJECT_ENSURE_REGISTERED (NwkHeader);

NwkHeader::NwkHeader():
m_frameControl(0),
m_radius(0), 
m_destAddrIEEE(0),
m_sourceAddrIEEE(0), 
m_multicastContral(0), 
m_relayCount(0), 
m_relayIndex(0) {
      Ptr<UniformRandomVariable> x = CreateObject<UniformRandomVariable> ();
      x->SetAttribute ("Min", DoubleValue (0.0));
      x->SetAttribute ("Max", DoubleValue (10000.0));
      m_seqNum = x->GetInteger ();
}

TypeId
NwkHeader::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::NwkHeader")
    .SetParent<Header> ()
    .AddConstructor<NwkHeader> ();
  return tid;
}

TypeId
NwkHeader::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

void 
NwkHeader::setType(NwkHeader::FrameType frametype)
{
    m_frameControl |= static_cast<uint16_t>(frametype);
}

NwkHeader::FrameType 
NwkHeader::GetType(void) const
{
  return static_cast<NwkHeader::FrameType>(m_frameControl & 0x3);
}

void
NwkHeader::Print(std::ostream &os) const {
  os << "framecContral is" << m_frameControl << std::endl;
  os << "destAddr: " << m_destAddr << std::endl;
  os << "sourceAddr: " << m_sourceAddr << std::endl;
  os << "radius: " << (int)m_radius << std::endl;
  os << "seqNum: " << (int)m_seqNum << std::endl;
  os << "destAddrIEEE: " << m_destAddrIEEE << std::endl;
  os << "sourceAddrIEEE: " << m_sourceAddrIEEE << std::endl;
  os << "multicastContral: " << (int)m_multicastContral << std::endl;
  os << "relayCount: " << (int)m_relayCount << std::endl;
  os << "relayIndex: " << (int)m_relayIndex << std::endl;
  os << "sourceList: [ ";
  for (auto addr : m_sourceList) {
    os << addr << " ";
  }
  os << "]" << std::endl;
}

uint32_t
NwkHeader::GetSerializedSize() const
{
  return 7;
}

void
NwkHeader::Serialize(Buffer::Iterator start) const
{
  if(GetType() == FrameType::NWK_FRAME_DATA || GetType() == FrameType::NWK_FRAME_COMMAND)
  {
    start.WriteU16(m_frameControl);
    start.WriteU16(m_destAddr.GetAddressU16());
    start.WriteU16(m_sourceAddr.GetAddressU16());
    start.WriteU8(m_seqNum);
  }
  // NS_LOG_FUNCTION(this << m_frameControl << " " << m_destAddr << " " << m_sourceAddr << " " << m_seqNum);
  NS_LOG_FUNCTION(this << m_frameControl << " " << m_destAddr.GetAddressU16() << " " << m_sourceAddr.GetAddressU16() << " " << m_seqNum);

  // start.WriteU8(radius); 
  // start.WriteU64(destAddrIEEE);
  // start.WriteU64(sourceAddrIEEE);
  // start.WriteU8(multicastContral);
  // start.WriteU8(relayCount);
  // start.WriteU8(relayIndex);
  // for(std::vector<NwkShortAddress>::const_iterator it = sourceList.begin(); it != sourceList.end(); it ++)
  // {
  //     start.WriteU16((*it).GetAddressU16());
  // }
}

uint32_t 
NwkHeader::Deserialize (Buffer::Iterator start)
{
  if(GetType() == FrameType::NWK_FRAME_DATA || GetType() == FrameType::NWK_FRAME_COMMAND)
  {
    m_frameControl = start.ReadU16();  
    m_destAddr = static_cast<NwkShortAddress>(start.ReadU16());
    m_sourceAddr = static_cast<NwkShortAddress>(start.ReadU16());
    m_seqNum = start.ReadU8();
  }
  NS_LOG_FUNCTION(this << m_frameControl << " " << m_destAddr << " " << m_sourceAddr << " " << m_seqNum);

  // radius = start.ReadU8();
  // destAddrIEEE = start.ReadU64();
  // sourceAddrIEEE = start.ReadU64();
  // multicastContral = start.ReadU8();
  // relayCount = start.ReadU8();
  // relayIndex = start.ReadU8();
  // for(int i = 0; i < relayCount; ++ i) 
  //   sourceList.push_back(static_cast<NwkShortAddress>(start.ReadU16()));
  return GetSerializedSize();
}


}