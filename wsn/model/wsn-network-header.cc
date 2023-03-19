#include "wsn-network-header.h"
#include <ns3/log.h>


namespace ns3
{

NS_LOG_COMPONENT_DEFINE ("NwkHeader");

NS_OBJECT_ENSURE_REGISTERED (NwkFrame);

NwkFrame::NwkFrame ()
{
 
}

NwkFrame::~NwkFrame ()
{
  
}

TypeId
NwkFrame::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::NwkFrame")
    .SetParent<Header> ()
    .AddConstructor<NwkFrame> ();
  return tid;
}

TypeId
NwkFrame::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

void
NwkFrame::Print (std::ostream &os) const
{
  os << "m_frameControl : " << m_frameControl << std::endl;
}

uint32_t
NwkFrame::GetSerializedSize (void) const
{
  return 2;
}

void
NwkFrame::Serialize (Buffer::Iterator start) const
{
  start.WriteU16(m_frameControl);
}

uint32_t
NwkFrame::Deserialize (Buffer::Iterator start)
{
  m_frameControl = start.ReadU16();
  return GetSerializedSize();
}
//............NwkHeader....................//


NS_OBJECT_ENSURE_REGISTERED (NwkHeader);

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
NwkHeader::Print(std::ostream &os) const {
  nwkframe.Print(os);
  os << "destAddr: " << destAddr << std::endl;
  os << "sourceAddr: " << sourceAddr << std::endl;
  os << "radius: " << (int)radius << std::endl;
  os << "seqNum: " << (int)seqNum << std::endl;
  os << "destAddrIEEE: " << destAddrIEEE << std::endl;
  os << "sourceAddrIEEE: " << sourceAddrIEEE << std::endl;
  os << "multicastContral: " << (int)multicastContral << std::endl;
  os << "relayCount: " << (int)relayCount << std::endl;
  os << "relayIndex: " << (int)relayIndex << std::endl;
  os << "sourceList: [ ";
  for (auto addr : sourceList) {
    os << addr << " ";
  }
  os << "]" << std::endl;
}

uint32_t
NwkHeader::GetSerializedSize() const
{
  return 8;
}

/*
    uint16_t destAddr;
    uint16_t sourceAddr;
    uint8_t radius;
    uint8_t seqNum;
    uint16_t destAddrIEEE;
    uint16_t sourceAddrIEEE;
    uint8_t multicastContral;
    uint8_t relayCount;
    uint8_t relayIndex;
*/

void
NwkHeader::Serialize(Buffer::Iterator start) const
{
  nwkframe.Serialize(start);
  start.WriteU8(destAddr);
  start.WriteU8(sourceAddr);
  start.WriteU8(radius);
  start.WriteU8(seqNum);
  start.WriteU16(destAddrIEEE);
  start.WriteU16(sourceAddrIEEE);
  start.WriteU8(multicastContral);
  start.WriteU8(relayCount);
  start.WriteU8(relayIndex);
  for(std::vector<uint16_t>::const_iterator it = sourceList.begin(); it != sourceList.end(); it ++)
  {
      start.WriteU16(*it);
  }
}

uint32_t 
NwkHeader::Deserialize (Buffer::Iterator start)
{
  nwkframe.Deserialize(start);
  destAddr = start.ReadU8();
  sourceAddr = start.ReadU8();
  radius = start.ReadU8();
  seqNum = start.ReadU8();
  destAddrIEEE = start.ReadU16();
  sourceAddrIEEE = start.ReadU16();
  multicastContral = start.ReadU8();
  relayCount = start.ReadU8();
  relayIndex = start.ReadU8();
  for(int i = 0; i < relayCount; ++ i) 
    sourceList.push_back(static_cast<uint16_t>(start.ReadU16()));
  return GetSerializedSize()+nwkframe.GetSerializedSize();
}


}