#ifndef WSN_APPLICATION_HEADER_H
#define WSN_APPLICATION_HEADER_H

#include <stdint.h>
#include "ns3/header.h"

namespace ns3
{
/*
Zigbee Transmission Layer ZTL

1 byte
| 0 |  1  |  2  |  3   |      4     |      5      |      6       | 7  |
|FrameType|DeliveryMode|ApsAckFormat|SecurityLevel|Acknowledgment| Re |

*/



class ApsFrame : public Header
{
public:
    enum FrameType 
    {
        FRAME_TYPE_DATA = 0x00,
        FRAME_TYPE_COMMAND = 0x01,
        FRAME_TYPE_ACKNOWLEDGE = 0x02,
        FRAME_TYPE_RESERVED = 0x03
    };

    enum DeliveryMode 
    {
        UNICAST = 0x00,
        INDIRECT = 0x01,
        BROADCAST = 0x02,
        GROUP = 0x03,
        MULTICAST = 0x04
    };

    enum ApsAckFormat 
    {
        APS_ACK_NO_ACK = 0x0,
        APS_ACK_YES_ACK = 0x1
    };

    enum SecurityLevel 
    {
        SECURITY_NONE = 0x0,
        SECURITY_ENC = 0x1
    };

    enum Acknowledgment 
    {
        NO_ACKNOWLEDGMENT = 0x0,
        ACKNOWLEDGMENT_REQUIRED = 0x1
    };

    static TypeId GetTypeId (void);

    TypeId GetInstanceTypeId (void) const override;
    void Print (std::ostream &os) const override;
    uint32_t GetSerializedSize (void) const override;
    void Serialize (Buffer::Iterator start) const override;
    uint32_t Deserialize (Buffer::Iterator start) override;

    ApsFrame();

    void setFrameType(FrameType frametype);
    FrameType getFrameType() const;

    void setDeliveryMode(DeliveryMode deliverymode);
    DeliveryMode getDeliveryMode() const;

    void setApsAckFormat(ApsAckFormat apsackformat);
    ApsAckFormat getApsAckFormat() const;

    void setSecurityLevel(SecurityLevel securitylevel);
    SecurityLevel getSecurityLevel() const;

    void setAcknowledgment(Acknowledgment acknowledgment);
    Acknowledgment getAcknowledgment() const;

private:
    uint16_t m_frameControl;
};


class AppHeader : public Header
{
public:
    AppHeader();

    static TypeId GetTypeId (void);

    TypeId GetInstanceTypeId (void) const override;
    void Print (std::ostream &os) const override;
    uint32_t GetSerializedSize (void) const override;
    void Serialize (Buffer::Iterator start) const override;
    uint32_t Deserialize (Buffer::Iterator start) override;

private:    
    ApsFrame frameControl;
    uint8_t destinationEndpoint;
    uint16_t groupaddress;
    uint16_t clusterID;
    uint16_t profileID;
    uint8_t sourceEndpoint;
    uint8_t apsCount;
    uint16_t FSC;
};


}

#endif