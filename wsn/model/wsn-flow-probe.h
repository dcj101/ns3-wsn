#ifndef WSN_FLOW_PROBE_H
#define WSN_FLOW_PROBE_H

#include "ns3/flow-probe.h"
#include "ns3/flow-monitor.h"
#include "wsn-flow-classifier.h"
#include "wsn-network.h"
#include "wsn-flow-probe-tag.h"

namespace ns3
{

class WsnFlowProbe : public FlowProbe
{
public:
    WsnFlowProbe (Ptr<FlowMonitor> monitor, Ptr<WsnFlowClassifier> classifier, Ptr<Node> node);
    virtual ~WsnFlowProbe ();

    static TypeId GetTypeId (void);


   
protected:

    virtual void DoDispose (void);

private:
    void SendOutgoingLogger (const NwkHeader &ipHeader, Ptr<const Packet> ipPayload);

    void ForwardLogger (const NwkHeader &ipHeader, Ptr<const Packet> ipPayload);

    void ForwardUpLogger (const NwkHeader &ipHeader, Ptr<const Packet> ipPayload);
    
    Ptr<WsnFlowClassifier> m_classifier;

    Ptr<WsnNwkProtocol> m_wsnNwkProtocol;

};

}
#endif