#include "wsn-application-header.h"
#include "ns3/double.h"
#include <ns3/log.h>

namespace ns3
{

NS_LOG_COMPONENT_DEFINE ("wsnApplication");

//......................................
NS_OBJECT_ENSURE_REGISTERED (AppHeader);
void 
AppHeader::setFrameType(AFrameType frametype)
{
    m_frameControl |= static_cast<uint16_t>(frametype);
}

AFrameType 
AppHeader::getFrameType() const
{
    return static_cast<AFrameType>(m_frameControl & 0x3);
}

void 
AppHeader::setDeliveryMode(ADeliveryMode deliverymode)
{
    m_frameControl |= static_cast<uint16_t>(deliverymode) << 2;
}

ADeliveryMode 
AppHeader::getDeliveryMode() const 
{
    return static_cast<ADeliveryMode>((m_frameControl >> 2) & 0x3);
}

void 
AppHeader::setApsAckFormat(ApsAckFormat apsackformat)
{
    m_frameControl |= static_cast<uint16_t>(apsackformat) << 4;
}

ApsAckFormat 
AppHeader::getApsAckFormat() const
{
    return static_cast<ApsAckFormat>((m_frameControl >> 4) & 0x1);
}

void 
AppHeader::setSecurityLevel(ASecurityLevel securitylevel)
{
    m_frameControl |= static_cast<uint16_t>(securitylevel) << 5;
}

ASecurityLevel 
AppHeader::getSecurityLevel() const
{
    return static_cast<ASecurityLevel>((m_frameControl >> 5) & 0x1);
}

void 
AppHeader::setAcknowledgment(Acknowledgment acknowledgment) 
{
    m_frameControl |= static_cast<uint16_t>(acknowledgment) << 6;
}

Acknowledgment 
AppHeader::getAcknowledgment() const 
{
    return static_cast<Acknowledgment>((m_frameControl >> 6) & 0x1);
}



AppHeader::AppHeader():
        sourceEndpoint(0x1),
        destinationEndpoint(0x1),
        m_frameControl(0),
        groupaddress(0xffff),
        clusterID(0x0402),
        profileID(0x000)
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
    os << "Frame Control: " << m_frameControl << std::endl;
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
    return 2 + 1 + 2 + 2 + 2 + 1 + 1 + 2;
}

void 
AppHeader::Serialize (Buffer::Iterator start) const
{
    start.WriteU8(sourceEndpoint);
    start.WriteU8(destinationEndpoint);
    start.WriteU16(m_frameControl);
    start.WriteU16(groupaddress);
    start.WriteU16(clusterID);
    start.WriteU16(profileID);
    start.WriteU8(apsCount);
}

uint32_t 
AppHeader::Deserialize (Buffer::Iterator start)
{
    sourceEndpoint = start.ReadU8();
    destinationEndpoint = start.ReadU8();
    m_frameControl = start.ReadU16();
    groupaddress = start.ReadU16();
    clusterID = start.ReadU16();
    profileID = start.ReadU16();
    apsCount = start.ReadU8();
    return GetSerializedSize();
}






} // namespace ns3
