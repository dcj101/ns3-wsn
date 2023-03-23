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

class NwkHeader : public Header
{

public:

    NwkHeader();
    
    ~NwkHeader(){}

    enum FrameType
    {
        NWK_FRAME_DATA = 0x00,
        NWK_FRAME_COMMAND = 0x3,
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

    // Getter and setter for destAddr
    void SetDestAddr(NwkShortAddress addr) { m_destAddr = addr; }
    NwkShortAddress GetDestAddr() const { return m_destAddr; }
    
    // Getter and setter for sourceAddr
    void SetSourceAddr(NwkShortAddress addr) { m_sourceAddr = addr; }
    NwkShortAddress GetSourceAddr() const { return m_sourceAddr; }
    
    // Getter and setter for radius
    void SetRadius(uint8_t rad) { m_radius = rad; }
    uint8_t GetRadius() const { return m_radius; }
    
    // Getter and setter for seqNum
    void SetSeqNum(uint8_t seq) { m_seqNum = seq; }
    uint8_t GetSeqNum() const { return m_seqNum; }
    
    // Getter and setter for destAddrIEEE
    void SetDestAddrIEEE(uint64_t addr) { m_destAddrIEEE = addr; }
    uint16_t GetDestAddrIEEE() const { return m_destAddrIEEE; }
    
    // Getter and setter for sourceAddrIEEE
    void SetSourceAddrIEEE(uint64_t addr) { m_sourceAddrIEEE = addr; }
    uint16_t GetSourceAddrIEEE() const { return m_sourceAddrIEEE; }
    
    // Getter and setter for multicastContral
    void SetMulticastContral(uint8_t control) { m_multicastContral = control; }
    uint8_t GetMulticastContral() const { return m_multicastContral; }
    
    // Getter and setter for relayCount
    void SetRelayCount(uint8_t count) { m_relayCount = count; }
    uint8_t GetRelayCount() const { return m_relayCount; }
    
    // Getter and setter for relayIndex
    void SetRelayIndex(uint8_t index) { m_relayIndex = index; }
    uint8_t GetRelayIndex() const { return m_relayIndex; }
    
    // Getter and setter for sourceList
    void SetSourceList(const NwkShortAddress & list) { m_sourceList.push_back(list); }
    std::vector<NwkShortAddress> GetSourceList() const { return m_sourceList; }

    static TypeId GetTypeId (void);

    TypeId GetInstanceTypeId (void) const override;
    void Print (std::ostream &os) const override;
    uint32_t GetSerializedSize (void) const override;
    void Serialize (Buffer::Iterator start) const override;
    uint32_t Deserialize (Buffer::Iterator start) override;
    
    void setType(FrameType frametype);
    FrameType GetType(void) const;

private:
    WsnNwkPayload m_wsnNwkPayload;
    uint16_t m_frameControl;
    NwkShortAddress m_destAddr;
    NwkShortAddress m_sourceAddr;
    uint8_t m_radius;
    uint8_t m_seqNum;
    uint64_t m_destAddrIEEE;
    uint64_t m_sourceAddrIEEE;
    uint8_t m_multicastContral;
    uint8_t m_relayCount;
    uint8_t m_relayIndex;
    std::vector<NwkShortAddress> m_sourceList;


};

}

#endif