#ifndef WSN_NETWORK_PL_H
#define WSN_NETWORK_PL_H
#include <utility>
#include <vector>
#include "ns3/packet.h"

namespace ns3
{

class WsnNwkPayload : public Tag
{
public:

    enum NWKCommandIdentifier
    {
        WSN_PL_ROUTE_REQUEST = 0x01,
        WSN_PL_ROUTE_REPLY,
        WSN_PL_NETWORK_STATUS,
        WSN_PL_LEAVE,
        WSN_PL_ROUTE_RECORD,
        WSN_PL_REJOIN_RECORD,
        WSN_PL_REJOIN_REQUEST,
        WSN_PL_REJOIN_RESPONSE,
        WSN_PL_LINK_STATUS,
        WSN_PL_NETWORK_REPORT,
        WSN_PL_NETWORK_UPDATE,
        WSN_PL_END_DEVICE_TIMEOUT_REQUEST,
        WSN_PL_END_DEVICE_TIMEOUT_RESPONSE,
        WSN_PL_LINK_POWER_DELTA,
    };

    enum ManyToOne
    {
        WSN_PL_NOT_MATCH = 0x00,
        WSN_PL_MATCH,
        WSN_PL_MATCH_BUT_SENDER_NOT_MATCH,
        WSN_PL_RESERVE,
    };

    enum ErrorCode
    {
        WSN_PL_NO_ROUTE_AVAILABLE = 0x00,
        WSN_PL_TREE_LINK_FAILURE,
        WSN_PL_NOT_TREE_LINK_FAILURE,
        WSN_PL_LOW_BATTERY_LEVEL,
        WSN_PL_NO_ROUTING_CAPACITY,
        WSN_PL_NO_INDIRECT_CAPACITY,
        WSN_PL_INDIRECT_TRANSACTION_EXPIRY,
        WSN_PL_TARGET_DEVICE_UNAVAILABLE,
        WSN_PL_TARGET_ADDRESS_UNALLOCATED,
        WSN_PL_PARENT_LINK_FAILURE,
        WSN_PL_VALIDATE_ROUTE,
        WSN_PL_SOURCE_ROUTE_FAILURE,
        WSN_PL_MANY_TO_ONT_ROUTE_FAILURE,
        WSN_PL_ADDRESS_CONFILICT,
        WSN_PL_VERIFY_ADDRESS,
        WSN_PL_PAN_IDENTIFIER_UPDATE,
        WSN_PL_NETWORK_ADDRESS_UPDATE,
        WSN_PL_BAD_FRAME_COUNTER,
        WSN_PL_BED_KEY_SQE_NUM,
        WSN_PL_UNKONW_COMMAND,
    };

    WsnNwkPayload(){}
    static TypeId GetTypeId (void);
    virtual TypeId GetInstanceTypeId (void) const;
    virtual uint32_t GetSerializedSize (void) const;
    virtual void Serialize (TagBuffer start) const;
    virtual void Deserialize (TagBuffer start);
    virtual void Print (std::ostream &os) const;
private:


    uint8_t nwkCommandIdentifier;
    uint8_t commandOption;
    // Route Replay
    uint8_t routeRequestIdentifier;
    uint16_t responseAddr;
    uint8_t pathCast;
    //Route reply add originatorAddr compare Route Replay
    uint16_t originatorAddr;


    // Network Status
    uint8_t errorCode;

    //Route Record
    uint8_t replayCount;
    std::vector<uint16_t> replayList;

    // Rejoin Request
    uint8_t capabilityInformation;

    //Rejoin response
    uint16_t shortAddr;
    uint8_t  rejoinStatus;

    // link status 
    uint8_t linkStatusOp;
    // pair <neighbor network addr, link status>
    std::vector< std::pair<uint16_t,uint8_t> > linkStatusList;

    // Network report

};

}

#endif