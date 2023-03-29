
#include "wsn-flow-monitor-helper.h"
#include "ns3/flow-monitor.h"
#include "ns3/wsn-flow-classifier.h"
#include "ns3/wsn-flow-probe.h"
#include "ns3/wsn-network.h"
#include "ns3/node.h"
#include "ns3/node-list.h"
#include <iostream>


namespace ns3 {

WsnFlowMonitorHelper::WsnFlowMonitorHelper ()
{
  m_monitorFactory.SetTypeId ("ns3::FlowMonitor");
  // m_flowMonitor = m_monitorFactory.Create<FlowMonitor> ();

}

WsnFlowMonitorHelper::~WsnFlowMonitorHelper ()
{
  if (m_flowMonitor)
    {
      m_flowMonitor->Dispose ();
      m_flowMonitor = 0;
      m_flowClassifierWsn = 0;
    }
}

void 
WsnFlowMonitorHelper::SetMonitorAttribute (std::string n1, const AttributeValue &v1)
{
  m_monitorFactory.Set (n1, v1);
}


Ptr<FlowMonitor>
WsnFlowMonitorHelper::GetMonitor ()
{
  if (!m_flowMonitor)
    {
      m_flowMonitor = m_monitorFactory.Create<FlowMonitor> ();
      m_flowClassifierWsn = Create<WsnFlowClassifier> ();
      m_flowMonitor->AddFlowClassifier (m_flowClassifierWsn);
    }
  return m_flowMonitor;
}


Ptr<FlowClassifier>
WsnFlowMonitorHelper::GetClassifier ()
{
  if (!m_flowClassifierWsn)
    {
      m_flowClassifierWsn = Create<WsnFlowClassifier> ();
    }
  return m_flowClassifierWsn;
}



Ptr<FlowMonitor>
WsnFlowMonitorHelper::Install (Ptr<Node> node)
{
  Ptr<FlowMonitor> monitor = GetMonitor ();
  Ptr<FlowClassifier> classifier = GetClassifier ();
  Ptr<WsnNwkProtocol> wsnNwkProtocol = node->GetObject<WsnNwkProtocol> ();
  if (wsnNwkProtocol)
    {
      std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n";
      Ptr<WsnFlowProbe> probe = Create<WsnFlowProbe> (monitor,
                                                        DynamicCast<WsnFlowClassifier> (classifier),
                                                        node);
    }
  return m_flowMonitor;
}


Ptr<FlowMonitor>
WsnFlowMonitorHelper::Install (NodeContainer nodes)
{
  for (NodeContainer::Iterator i = nodes.Begin (); i != nodes.End (); ++i)
    {
      std::cout << "@@@@@@@@@@@@@@\n";
      Ptr<Node> node = *i;
      if (node->GetObject<WsnNwkProtocol> ())
        {
          Install (node);
        }
    }
  return m_flowMonitor;
}

Ptr<FlowMonitor>
WsnFlowMonitorHelper::InstallAll ()
{
      std::cout << "@@@@@@@@@@@@@@@@@@\n";
  for (NodeList::Iterator i = NodeList::Begin (); i != NodeList::End (); ++i)
    {
      Ptr<Node> node = *i;
      if (node->GetObject<WsnNwkProtocol> ())
        {
          Install (node);
        }
    }
  return m_flowMonitor;
}

void
WsnFlowMonitorHelper::SerializeToXmlStream (std::ostream &os, uint16_t indent, bool enableHistograms, bool enableProbes)
{
  if (m_flowMonitor)
    {
      m_flowMonitor->SerializeToXmlStream (os, indent, enableHistograms, enableProbes);
    }
}

std::string
WsnFlowMonitorHelper::SerializeToXmlString (uint16_t indent, bool enableHistograms, bool enableProbes)
{
  std::ostringstream os;
  if (m_flowMonitor)
    {
      m_flowMonitor->SerializeToXmlStream (os, indent, enableHistograms, enableProbes);
    }
  return os.str ();
}

void
WsnFlowMonitorHelper::SerializeToXmlFile (std::string fileName, bool enableHistograms, bool enableProbes)
{
  if (m_flowMonitor)
    {
      m_flowMonitor->SerializeToXmlFile (fileName, enableHistograms, enableProbes);
    }
}


} // namespace ns3
