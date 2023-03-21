#include "wsn-network-header.h"
#include <ns3/log.h>


namespace ns3
{

NS_LOG_COMPONENT_DEFINE ("NwkHeader");

NS_OBJECT_ENSURE_REGISTERED (NwkFrame);

NwkFrame::NwkFrame ():m_frameControl(0)
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

NwkFrame::FrameType 
NwkFrame::GetType(void) const
{
  return static_cast<FrameType>(m_frameControl & 0x3);
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

NwkHeader::NwkHeader()
:radius(0), 
destAddrIEEE(0),
sourceAddrIEEE(0), 
multicastContral(0), 
relayCount(0), 
relayIndex(0) {
      Ptr<UniformRandomVariable> x = CreateObject<UniformRandomVariable> ();
      x->SetAttribute ("Min", DoubleValue (0.0));
      x->SetAttribute ("Max", DoubleValue (10000.0));
      seqNum = x->GetInteger ();
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

NwkFrame::FrameType 
NwkHeader::GetType(void) const
{
  return nwkframe.GetType();
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
  return 5 + nwkframe.GetSerializedSize();
}

void
NwkHeader::Serialize(Buffer::Iterator start) const
{
  
  nwkframe.Serialize(start);
  if(nwkframe.GetType() == NwkFrame::NWK_FRAME_DATA)
  {
    start.WriteU16(destAddr.GetAddressU16());
    start.WriteU16(sourceAddr.GetAddressU16());
    start.WriteU8(seqNum);
  }
  

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
  nwkframe.Deserialize(start);
  destAddr = static_cast<NwkShortAddress>(start.ReadU16());
  sourceAddr = static_cast<NwkShortAddress>(start.ReadU16());
  seqNum = start.ReadU8();

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