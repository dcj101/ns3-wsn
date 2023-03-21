#ifndef WSN_NETWORK_HEADER_H
#define WSN_NETWORK_HEADER_H
#include "ns3/header.h"
#include "ns3/packet.h"
#include "ns3/random-variable-stream.h"
#include <ns3/lr-wpan-module.h>
#include <stdint.h>
#include <vector>
#include <iostream>
#include "wsn-route.h"
#include "wsn-network-pl.h"
#include "wsn-nwk-short-address.h"


namespace ns3
{


class NwkFrame : public Header
{
public:

    enum FrameType
    {
        NWK_FRAME_DATA = 0x00,
        NWK_FRAME_COMMAND = 0x11,
    };
    enum ProtocolVersion
    {
        NWK_FRAME_ZIGBB = 0x00,
    };
    enum FindRoute
    {
        NWK_FRAME_SUPPRESS_ROUTING = 0x00,
        NWK_FRAME_ENABLE_ROUTING = 0x01,
        NWK_FRAME_FORCED_ROUTING = 0x02,
        NWK_FRAME_RESERVE = 0x03,
    };
    enum MultiCastFlag 
    {
        NWK_FRAME_BROADCAST_OR_UNICAST = 0x00,
        NWK_FRAME_MULTICAST = 0x01,
    };  
    enum SecurityLevel 
    {
        NWK_FRAMK_SECURITY_NONE = 0x0,
        NWK_FRAMK_SECURITY_ENC = 0x1
    }; 
    enum SourcerRoutingSubdomain
    {
        NWK_FRAMK_NOT_OPEN = 0x00,
        NWK_FRAMK_IS_OPEN = 0x01,
    };
    enum DestinationIEEEAddressSubfield
    {
        NWK_FRAME_D_NOT_CONTAIN = 0x00,
        NWK_FRAME_D_IS_CONTAIN = 0x01,
    };

    enum SourceIEEEAddressSubfield
    {
        NWK_FRAME_S_NOT_CONTAIN = 0x00,
        NWK_FRAME_S_IS_CONTAIN = 0x01,
    };

    NwkFrame();
    ~NwkFrame();
    static TypeId GetTypeId (void);

    TypeId GetInstanceTypeId (void) const override;
    void Print (std::ostream &os) const override;
    uint32_t GetSerializedSize (void) const override;
    void Serialize (Buffer::Iterator start) const override;
    uint32_t Deserialize (Buffer::Iterator start) override;

    NwkFrame::FrameType GetType(void) const;


private:
    uint16_t m_frameControl;
};

class NwkHeader : public Header
{

public:
    

    NwkHeader();
    
    ~NwkHeader(){}

    // Getter and setter for destAddr
    void SetDestAddr(NwkShortAddress addr) { destAddr = addr; }
    NwkShortAddress GetDestAddr() const { return destAddr; }
    
    // Getter and setter for sourceAddr
    void SetSourceAddr(NwkShortAddress addr) { sourceAddr = addr; }
    NwkShortAddress GetSourceAddr() const { return sourceAddr; }
    
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
    void SetSourceList(const NwkShortAddress & list) { sourceList.push_back(list); }
    std::vector<NwkShortAddress> GetSourceList() const { return sourceList; }

    static TypeId GetTypeId (void);

    TypeId GetInstanceTypeId (void) const override;
    void Print (std::ostream &os) const override;
    uint32_t GetSerializedSize (void) const override;
    void Serialize (Buffer::Iterator start) const override;
    uint32_t Deserialize (Buffer::Iterator start) override;
    
    NwkFrame::FrameType GetType(void) const;

private:
    WsnNwkPayload wsnNwkPayload;
    NwkFrame nwkframe;
    NwkShortAddress destAddr;
    NwkShortAddress sourceAddr;
    uint8_t radius;
    uint8_t seqNum;
    uint64_t destAddrIEEE;
    uint64_t sourceAddrIEEE;
    uint8_t multicastContral;
    uint8_t relayCount;
    uint8_t relayIndex;
    std::vector<NwkShortAddress> sourceList;


};

}

#endif