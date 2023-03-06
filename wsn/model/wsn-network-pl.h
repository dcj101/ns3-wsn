#ifndef WSN_NETWORK_PL_H
#define WSN_NETWORK_PL_H
#include <utility>
#include <vector>
#include "ns3/packet.h"

namespace ns3
{

class WsnPayLoad : public Tag
{
public:
    enum NWKCommandIdentifier
    {
        ROUTE_REQUEST = 0x01,
        ROUTE_REPLY,
        NETWORK_STATUS,
        LEAVE,
        ROUTE_RECORD,
        REJOIN_RECORD,
        REJOIN_REQUEST,
        REJOIN_RESPONSE,
        LINK_STATUS,
        NETWORK_REPORT,
        NETWORK_UPDATE,
        END_DEVICE_TIMEOUT_REQUEST,
        END_DEVICE_TIMEOUT_RESPONSE,
        LINK_POWER_DELTA,
    };

    enum ManyToOne
    {
        NOT_MATCH = 0x00,
        MATCH,
        MATCH_BUT_SENDER_NOT_MATCH,
        RESERVE,
    };

    enum ErrorCode
    {
        NO_ROUTE_AVAILABLE = 0x00,
        TREE_LINK_FAILURE,
        NOT_TREE_LINK_FAILURE,
        LOW_BATTERY_LEVEL,
        NO_ROUTING_CAPACITY,
        NO_INDIRECT_CAPACITY,
        INDIRECT_TRANSACTION_EXPIRY,
        TARGET_DEVICE_UNAVAILABLE,
        TARGET_ADDRESS_UNALLOCATED,
        PARENT_LINK_FAILURE,
        VALIDATE_ROUTE,
        SOURCE_ROUTE_FAILURE,
        MANY_TO_ONT_ROUTE_FAILURE,
        ADDRESS_CONFILICT,
        VERIFY_ADDRESS,
        PAN_IDENTIFIER_UPDATE,
        NETWORK_ADDRESS_UPDATE,
        BAD_FRAME_COUNTER,
        BED_KEY_SQE_NUM,
        UNKONW_COMMAND,
    };

    void addApp();

    static TypeId GetTypeId (void);
    virtual TypeId GetInstanceTypeId (void) const;
    virtual uint32_t GetSerializedSize (void) const;
    virtual void Serialize (TagBuffer start) const;
    virtual void Deserialize (TagBuffer start);

private:

    bool isCommand;

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
    vector<uint16_t> replayList;

    // Rejoin Request
    uint8_t capabilityInformation

    //Rejoin response
    uint16_t shortAddr;
    uint8_t  rejoinStatus;

    // link status 
    // pair <neighbor network addr, link status>
    vector< pair<uint16_t,uint8_t> > linkStatusList;

    // Network report

};

}

#endif