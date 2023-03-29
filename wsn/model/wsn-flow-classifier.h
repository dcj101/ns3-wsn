#ifndef WSN_FLOAT_CLASSIFIER_H
#define WSN_FLOAT_CLASSIFIER_H

#include "wsn-network.h"
#include "wsn-network-header.h"
#include "ns3/flow-classifier.h"


namespace ns3
{

class WsnFlowClassifier : public FlowClassifier
{
public:
    struct FourTuple
    {
        NwkShortAddress sourceAddress;
        NwkShortAddress destinationAddress;
        uint8_t sourcePoint;
        uint8_t destinationPoint;
    };

    WsnFlowClassifier();
    
    bool Classify (const NwkHeader &ipHeader, Ptr<const Packet> ipPayload,
                 uint32_t *out_flowId, uint32_t *out_packetId);

    FourTuple FindFlow (FlowId flowId) const;

    virtual void SerializeToXmlStream (std::ostream &os, uint16_t indent) const;


private:
  std::map<FourTuple, FlowId> m_flowMap;
  std::map<FlowId, FlowPacketId> m_flowPktIdMap;

};

bool operator < (const WsnFlowClassifier::FourTuple &t1, const WsnFlowClassifier::FourTuple &t2);

bool operator == (const WsnFlowClassifier::FourTuple &t1, const WsnFlowClassifier::FourTuple &t2);

}

#endif