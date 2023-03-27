#ifndef WSN_APPLICATION_HEADER_H
#define WSN_APPLICATION_HEADER_H

#include <stdint.h>
#include "ns3/header.h"
#include "ns3/random-variable-stream.h"

namespace ns3
{
/*
Zigbee Transmission Layer ZTL

1 byte
| 0 |  1  |  2  |  3   |      4     |      5      |      6       | 7  |
|FrameType|ADeliveryMode|ApsAckFormat|SecurityLevel|Acknowledgment| Re |

*/

enum AFrameType 
{
    APS_FRAME_TYPE_DATA = 0x00,
    APS_FRAME_TYPE_COMMAND = 0x01,
    APS_FRAME_TYPE_ACKNOWLEDGE = 0x02,
    APS_FRAME_TYPE_RESERVED = 0x03,
};

enum ADeliveryMode 
{
    APS_UNICAST = 0x00,
    APS_INDIRECT = 0x01,
    APS_BROADCAST = 0x02,
    APS_GROUP = 0x03,
    APS_MULTICAST = 0x04
};

enum ApsAckFormat 
{
    APS_ACK_NO_ACK = 0x0,
    APS_ACK_YES_ACK = 0x1
};

enum ASecurityLevel 
{
    APS_SECURITY_NONE = 0x0,
    APS_SECURITY_ENC = 0x1
};

enum Acknowledgment 
{
    APS_NO_ACKNOWLEDGMENT = 0x0,
    APS_ACKNOWLEDGMENT_REQUIRED = 0x1
};


class AppHeader : public Header
{
public:
    AppHeader();
    ~AppHeader();

    void setFrameType(AFrameType frametype);
    AFrameType getFrameType() const;

    void setDeliveryMode(ADeliveryMode deliverymode);
    ADeliveryMode getDeliveryMode() const;

    void setApsAckFormat(ApsAckFormat apsackformat);
    ApsAckFormat getApsAckFormat() const;

    void setSecurityLevel(ASecurityLevel securitylevel);
    ASecurityLevel getSecurityLevel() const;

    void setAcknowledgment(Acknowledgment acknowledgment);
    Acknowledgment getAcknowledgment() const;

    uint16_t GetFrameControl() const { return m_frameControl; }
    void SetFrameControl(uint16_t fc) { m_frameControl = fc; }

    uint8_t GetDestinationEndpoint() const { return destinationEndpoint; }
    void SetDestinationEndpoint(uint8_t de) { destinationEndpoint = de; }

    uint16_t GetGroupAddress() const { return groupaddress; }
    void SetGroupAddress(uint16_t ga) { groupaddress = ga; }

    uint16_t GetClusterID() const { return clusterID; }
    void SetClusterID(uint16_t cid) { clusterID = cid; }

    uint16_t GetProfileID() const { return profileID; }
    void SetProfileID(uint16_t pid) { profileID = pid; }

    uint8_t GetSourceEndpoint() const { return sourceEndpoint; }
    void SetSourceEndpoint(uint8_t se) { sourceEndpoint = se; }

    uint8_t GetApsCount() const { return apsCount; }
    void SetApsCount(uint8_t count) { apsCount = count; }


    static TypeId GetTypeId (void);

    TypeId GetInstanceTypeId (void) const override;
    void Print (std::ostream &os) const override;
    uint32_t GetSerializedSize (void) const override;
    void Serialize (Buffer::Iterator start) const override;
    uint32_t Deserialize (Buffer::Iterator start) override;

private:    
    uint8_t sourceEndpoint; // 0x1
    uint8_t destinationEndpoint; //0x1
    uint16_t m_frameControl;
    uint16_t groupaddress; // 0xffff
    uint16_t clusterID;    // 0x0402 温度
    uint16_t profileID;    // 0x0000
    uint8_t apsCount;     // rand
};


}

#endif