#include "wsn-application-header.h"
#include "ns3/double.h"
#include <ns3/log.h>

namespace ns3
{

NS_LOG_COMPONENT_DEFINE ("wsnApplication");

NS_OBJECT_ENSURE_REGISTERED (ApsFrame);

ApsFrame::ApsFrame():
m_frameControl(0)
{
  NS_LOG_FUNCTION (this);
}

ApsFrame::~ApsFrame()
{
  NS_LOG_FUNCTION (this);
}

void 
ApsFrame::setFrameType(AFrameType frametype)
{
    m_frameControl |= static_cast<uint16_t>(frametype);
}

AFrameType 
ApsFrame::getFrameType() const
{
    return static_cast<AFrameType>(m_frameControl & 0x3);
}

void 
ApsFrame::setDeliveryMode(ADeliveryMode deliverymode)
{
    m_frameControl |= static_cast<uint16_t>(deliverymode) << 2;
}

ADeliveryMode 
ApsFrame::getDeliveryMode() const 
{
    return static_cast<ADeliveryMode>((m_frameControl >> 2) & 0x3);
}

void 
ApsFrame::setApsAckFormat(ApsAckFormat apsackformat)
{
    m_frameControl |= static_cast<uint16_t>(apsackformat) << 4;
}

ApsAckFormat 
ApsFrame::getApsAckFormat() const
{
    return static_cast<ApsAckFormat>((m_frameControl >> 4) & 0x1);
}

void 
ApsFrame::setSecurityLevel(ASecurityLevel securitylevel)
{
    m_frameControl |= static_cast<uint16_t>(securitylevel) << 5;
}

ASecurityLevel 
ApsFrame::getSecurityLevel() const
{
    return static_cast<ASecurityLevel>((m_frameControl >> 5) & 0x1);
}

void 
ApsFrame::setAcknowledgment(Acknowledgment acknowledgment) 
{
    m_frameControl |= static_cast<uint16_t>(acknowledgment) << 6;
}

Acknowledgment 
ApsFrame::getAcknowledgment() const 
{
    return static_cast<Acknowledgment>((m_frameControl >> 6) & 0x1);
}

TypeId 
ApsFrame::GetTypeId (void)
{
    static TypeId tid = TypeId ("ns3::ApsFrame")
                    .SetParent<Header> ()
                    .AddConstructor<ApsFrame> ()
    ;
    return tid;
}

void 
ApsFrame::Print(std::ostream &os) const 
{
    os << "m_frameControl" << m_frameControl << std::endl;

}

TypeId
ApsFrame::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

uint32_t 
ApsFrame::GetSerializedSize (void) const
{
    return 2;
}

void 
ApsFrame::Serialize (Buffer::Iterator start) const
{
    start.WriteU16(m_frameControl);
}

uint32_t 
ApsFrame::Deserialize (Buffer::Iterator start)
{
    m_frameControl = start.ReadU16();
    return GetSerializedSize();
}
//......................................
NS_OBJECT_ENSURE_REGISTERED (AppHeader);


AppHeader::AppHeader():
        frameControl(),
        destinationEndpoint(0x1),
        groupaddress(0xffff),
        clusterID(0x0402),
        profileID(0x000),
        sourceEndpoint(0x1)
{ 
    Ptr<UniformRandomVariable> x = CreateObject<UniformRandomVariable> ();
    x->SetAttribute ("Min", DoubleValue (0.0));
    x->SetAttribute ("Max", DoubleValue (100.0));
    apsCount = x->GetInteger ();
}

AppHeader::~AppHeader()
{

}

TypeId 
AppHeader::GetTypeId (void)
{
    static TypeId tid = TypeId ("ns3::AppHeader")
                    .SetParent<Header> ()
                    .AddConstructor<AppHeader> ()
    ;
    return tid;
}

TypeId 
AppHeader::GetInstanceTypeId (void) const 
{
    return GetTypeId();
}

void 
AppHeader::Print(std::ostream &os) const {
    frameControl.Print(os);
    os << "Frame Control: " << frameControl << std::endl;
    os << "Destination Endpoint: " << static_cast<int>(destinationEndpoint) << std::endl;
    os << "Group Address: " << groupaddress << std::endl;
    os << "Cluster ID: " << clusterID << std::endl;
    os << "Profile ID: " << profileID << std::endl;
    os << "Source Endpoint: " << static_cast<int>(sourceEndpoint) << std::endl;
    os << "APS Count: " << static_cast<int>(apsCount) << std::endl;
}



uint32_t 
AppHeader::GetSerializedSize (void) const
{
    return 
    frameControl.GetSerializedSize()
     + 1 + 2 + 2 + 2 + 1 + 1 + 2;
}

void 
AppHeader::Serialize (Buffer::Iterator start) const
{
    frameControl.Serialize(start);
    start.WriteU8(destinationEndpoint);
    start.WriteU16(groupaddress);
    start.WriteU16(clusterID);
    start.WriteU16(profileID);
    start.WriteU8(sourceEndpoint);
    start.WriteU8(apsCount);
}

uint32_t 
AppHeader::Deserialize (Buffer::Iterator start)
{
    frameControl.Deserialize(start);
    destinationEndpoint = start.ReadU8();
    groupaddress = start.ReadU16();
    clusterID = start.ReadU16();
    profileID = start.ReadU16();
    sourceEndpoint = start.ReadU8();
    apsCount = start.ReadU8();
    return GetSerializedSize();
}






} // namespace ns3
