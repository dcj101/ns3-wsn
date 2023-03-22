#ifndef WSN_NETWORK_H
#define WSN_NETWORK_H
#include <vector>

#include "ns3/object.h"
#include "ns3/object-factory.h"
#include "ns3/net-device.h"
#include "ns3/traced-callback.h"
#include "ns3/lr-wpan-mac.h"
#include "ns3/type-id.h"
#include "ns3/ptr.h"
#include "ns3/mac48-address.h"

#include "wsn-address-allocator.h"
#include "wsn-neighbor-table.h"
#include "wsn-network-header.h"
#include "wsn-nwk-short-address.h"
#include "wsn-route.h"

#include <utility>

namespace ns3
{


enum NODE_TYPE
{
    EDGE  = 0x00,
    ROUTE = 0x01,
    COOR  = 0x02, 
};

class WsnNwkProtocol : public Object
{
    public:

    WsnNwkProtocol();

    WsnNwkProtocol(NODE_TYPE type);

    static TypeId GetTypeId (void);
    
    void Send(NwkShortAddress sourceaddr,NwkShortAddress dstAddr, Ptr<Packet> packet, NwkHeader::FrameType ftype);

    void BuildRtable(std::vector<StaticRoute> &rtable);

    void Install(Ptr<Node> node);

    void Assign(Ptr<LrWpanNetDevice> netDevice);
    
    void SetNode(Ptr<Node> node);

    void SetAck(bool ack);

    uint8_t GetPanID();

    uint8_t GetDepth();

    NwkShortAddress GetNwkShortAddress();

    Ptr<LrWpanNetDevice> GetLrWpanNetDevice();

    RoutingTable* GetRoutingTable();

    NeighborTable* GetNeighborTable();

    void SetCallbackSet();

    void JoinRequest(Ptr<WsnNwkProtocol> parents);

    void BeaconIndication (MlmeBeaconNotifyIndicationParams params, Ptr<Packet> p);

    void DataIndication (McpsDataIndicationParams params, Ptr<Packet> p);

    void TransEndIndication (McpsDataConfirmParams params);

    void DataIndicationCoordinator (McpsDataIndicationParams params, Ptr<Packet> p);

    void StartConfirm (MlmeStartConfirmParams params);



    protected:
    
    virtual void NotifyNewAggregate (void);
        
    virtual void DoDispose (void);

    virtual void DoInitialize (void);
    

    private:

    void CreateAndAggregateObjectFromTypeId(Ptr<Node> node, const std::string typeId);


    Ptr<LrWpanNetDevice> m_netDevice;

    Ptr<Node> m_node;

    NwkShortAddress m_addr;

    NwkShortAddress m_route; // 终端节点的父亲

    RoutingTable m_rtable;

    NeighborTable m_ntable;

    NODE_TYPE m_nodeType;

    uint8_t m_panId;

    uint8_t m_depth;

    bool m_ack;

    MlmeStartConfirmCallback m_MlmeStartConfirmCallback;

    McpsDataConfirmCallback m_McpsDataConfirmCallback;
    
    MlmeBeaconNotifyIndicationCallback m_MlmeBeaconNotifyIndicationCallback;
    
    McpsDataIndicationCallback m_McpsDataIndicationCallback;
};  

}

#endif