#include "wsn-application-header.h"

namespace ns3
{

ApsFrame::ApsFrame():
m_frameControl(0)
{
}

void 
ApsFrame::setFrameType(FrameType frametype)
{
    m_frameControl |= static_cast<uint16_t>(frametype);
}

FrameType 
ApsFrame::getFrameType() const
{
    return static_cast<FrameType>(m_frameControl & 0x3);
}

void 
ApsFrame::setDeliveryMode(DeliveryMode deliverymode)
{
    m_frameControl |= static_cast<uint16_t>(deliverymode) << 2;
}

DeliveryMode 
ApsFrame::getDeliveryMode() const 
{
    return static_cast<DeliveryMode>((m_frameControl >> 2) & 0x3);
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
ApsFrame::setSecurityLevel(SecurityLevel securitylevel)
{
    m_frameControl |= static_cast<uint16_t>(securitylevel) << 5;
}

SecurityLevel 
ApsFrame::getSecurityLevel() const
{
    return static_cast<SecurityLevel>((m_frameControl >> 5) & 0x1);
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
                    .AddConstructor<Header> ()
    ;
}

void 
ApsFrame::Print(std::ostream &os) const 
{
    
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
    start.ReadU16(m_frameControl);
}
//........................

AppHeader::AppHeader():
        frameControl(),
        destinationEndpoint(0),
        groupaddress(0),
        clusterID(0),
        profileID(0),
        sourceEndpoint(0),
        FSC(0)
{ 

}

TypeId 
AppHeader::GetTypeId (void)
{
    static TypeId tid = TypeId ("ns3::AppHeader")
                    .SetParent<Header> ()
                    .AddConstructor<Header> ()
    ;
}

TypeId 
AppHeader::GetInstanceTypeId (void) const 
{
    return GetTypeId()
}

void 
AppHeader::Print(std::ostream &os) const 
{
    
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
    start.WriteU16(FSC);
}

uint32_t 
AppHeader::Deserialize (Buffer::Iterator start)
{
    frameControl.Deserialize(start);
    start.ReadU8(destinationEndpoint);
    start.ReadU16(groupaddress);
    start.ReadU16(clusterID);
    start.ReadU16(profileID);
    start.ReadU8(sourceEndpoint);
    start.ReadU8(apsCount);
    start.ReadU16(FSC);
}






} // namespace ns3
