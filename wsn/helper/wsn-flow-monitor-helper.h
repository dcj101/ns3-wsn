
#ifndef WSN_FLOW_MONITOR_HELPER_H
#define WSN_FLOW_MONITOR_HELPER_H

#include "ns3/node-container.h"
#include "ns3/object-factory.h"
#include "ns3/flow-monitor.h"
#include "ns3/flow-classifier.h"
#include <string>

namespace ns3 {

class AttributeValue;
class WsnFlowClassifier;

class WsnFlowMonitorHelper
{
public:

  WsnFlowMonitorHelper ();
  ~WsnFlowMonitorHelper ();

  void SetMonitorAttribute (std::string n1, const AttributeValue &v1);

  Ptr<FlowMonitor> Install (NodeContainer nodes);

  Ptr<FlowMonitor> Install (Ptr<Node> node);

  Ptr<FlowMonitor> InstallAll ();

  Ptr<FlowMonitor> GetMonitor ();

  Ptr<FlowClassifier> GetClassifier ();

  Ptr<FlowClassifier> GetClassifier6 ();

  void SerializeToXmlStream (std::ostream &os, uint16_t indent, bool enableHistograms, bool enableProbes);

  std::string SerializeToXmlString (uint16_t indent, bool enableHistograms, bool enableProbes);

  void SerializeToXmlFile (std::string fileName, bool enableHistograms, bool enableProbes);

private:

  WsnFlowMonitorHelper (const WsnFlowMonitorHelper&);

  WsnFlowMonitorHelper& operator= (const WsnFlowMonitorHelper&);

  ObjectFactory m_monitorFactory;        //!< Object factory
  Ptr<FlowMonitor> m_flowMonitor;        //!< the FlowMonitor object
  Ptr<FlowClassifier> m_flowClassifierWsn; //!< the FlowClassifier object for IPv4
};

} // namespace ns3


#endif 
