#ifndef WSN_NETWORK_HEADER_H
#define WSN_NETWORK_HEADER_H
#include "ns3/header.h"
#include "ns3/packet.h"
#include <ns3/lr-wpan-module.h>
#include <stdint.h>
#include <vector>
#include <iostream>
#include "wsn-route.h"


namespace ns3
{


class NwkFrame : public Header
{
public:

    enum FrameType
    {
        DATA = 0x00,
        COMMAND = 0x11,
    };
    enum ProtocolVersion
    {
        ZIGBB = 2006
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
        D_NOT_CONTAIN = 0x00,
        D_IS_CONTAIN = 0x01,
    };

    enum SourceIEEEAddressSubfield
    {
        S_NOT_CONTAIN = 0x00,
        S_IS_CONTAIN = 0x01,
    };

    NwkFrame();
    ~NwkFrame();
    static TypeId GetTypeId (void);

    TypeId GetInstanceTypeId (void) const override;
    void Print (std::ostream &os) const override;
    uint32_t GetSerializedSize (void) const override;
    void Serialize (Buffer::Iterator start) const override;
    uint32_t Deserialize (Buffer::Iterator start) override;

    void setFrameType(FrameType frametype);
    FrameType getFrameType() const;

    void setProtocolVersion();


private:
    uint16_t m_frameControl;
};

class NwkHeader : public Header
{

public:
    ~NwkHeader(){}

    NwkHeader() : destAddr(0), sourceAddr(0), radius(0), seqNum(0), destAddrIEEE(0),
                  sourceAddrIEEE(0), multicastContral(0), relayCount(0), relayIndex(0) {}
    
    // Getter and setter for destAddr
    void SetDestAddr(uint16_t addr) { destAddr = addr; }
    uint16_t GetDestAddr() const { return destAddr; }
    
    // Getter and setter for sourceAddr
    void SetSourceAddr(uint16_t addr) { sourceAddr = addr; }
    uint16_t GetSourceAddr() const { return sourceAddr; }
    
    // Getter and setter for radius
    void SetRadius(uint8_t rad) { radius = rad; }
    uint8_t GetRadius() const { return radius; }
    
    // Getter and setter for seqNum
    void SetSeqNum(uint8_t seq) { seqNum = seq; }
    uint8_t GetSeqNum() const { return seqNum; }
    
    // Getter and setter for destAddrIEEE
    void SetDestAddrIEEE(uint16_t addr) { destAddrIEEE = addr; }
    uint16_t GetDestAddrIEEE() const { return destAddrIEEE; }
    
    // Getter and setter for sourceAddrIEEE
    void SetSourceAddrIEEE(uint16_t addr) { sourceAddrIEEE = addr; }
    uint16_t GetSourceAddrIEEE() const { return sourceAddrIEEE; }
    
    // Getter and setter for multicastContral
    void SetMulticastContral(uint8_t control) { multicastContral = control; }
    uint8_t GetMulticastContral() const { return multicastContral; }
    
    // Getter and setter for relayCount
    void SetRelayCount(uint8_t count) { relayCount = count; }
    uint8_t GetRelayCount() const { return relayCount; }
    
    // Getter and setter for relayIndex
    void SetRelayIndex(uint8_t index) { relayIndex = index; }
    uint8_t GetRelayIndex() const { return relayIndex; }
    
    // Getter and setter for sourceList
    void SetSourceList(const std::vector<uint16_t>& list) { sourceList = list; }
    std::vector<uint16_t> GetSourceList() const { return sourceList; }

    static TypeId GetTypeId (void);

    TypeId GetInstanceTypeId (void) const override;
    void Print (std::ostream &os) const override;
    uint32_t GetSerializedSize (void) const override;
    void Serialize (Buffer::Iterator start) const override;
    uint32_t Deserialize (Buffer::Iterator start) override;
    
    void DataIndication (McpsDataIndicationParams params, Ptr<Packet> p);

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
    std::vector<uint16_t> sourceList;


};

}

#endif