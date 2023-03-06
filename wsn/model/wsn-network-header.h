#ifndef WSN_NETWORK_HEADER_H
#define WSN_NETWORK_HEADER_H
#include "ns3/header.h"
#include <stdint.h>
#include <vector>

namespace ns3
{



class NwkFrame
{
public:

    enum FrameType
    {
        DATA = 0x00,
        COMMAND = 0x11,
    };

    enum ProtocolVersion
    {
        ZIGBB = "2006"
    };

    enum FindRoute
    {
        SUPPRESS_ROUTING = 0x00,
        ENABLE_ROUTING = 0x01,
        FORCED_ROUTING = 0x02,
        RESERVE = 0x03,
    };

    enum MultiCastFlag 
    {
        BROADCAST_OR_UNICAST = 0x00,
        MULTICAST = 0x01,
    };  

    enum SecurityLevel 
    {
        SECURITY_NONE = 0x0,
        SECURITY_ENC = 0x1
    }; 

    enum SourcerRoutingSubdomain
    {
        NOT_OPEN = 0x00,
        IS_OPEN = 0x01,
    };

    enum DestinationIEEEAddressSubfield
    {
        NOT_CONTAIN = 0x00,
        IS_CONTAIN = 0x01,
    };

    enum SourceIEEEAddressSubfield
    {
        NOT_CONTAIN = 0x00,
        IS_CONTAIN = 0x01,
    }

    NwkFrame();

    void setFrameType(FrameType frametype);
    FrameType getFrameType() const;

    void setProtocolVersion();


private:
    uint16_t m_frameControl;
};

class NwkHeader : public Header
{

public:
    static TypeId GetTypeId (void);

    TypeId GetInstanceTypeId (void) const override;
    void Print (std::ostream &os) const override;
    uint32_t GetSerializedSize (void) const override;
    void Serialize (Buffer::Iterator start) const override;
    uint32_t Deserialize (Buffer::Iterator start) override;
    
private:
    NwkFrame nwkframe;
    uint16_t destAddr;
    uint16_t sourceAddr;
    uint8_t radius;
    uint8_t seqNum;
    uint16_t destAddrIEEE;
    uint16_t sourceAddrIEEE;
    uint8_t multicastContral;
    uint8_t relayCount;
    uint8_t relayIndex;
    vector<uint16_t> sourceList;


};

}

#endif