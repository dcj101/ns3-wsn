
#include "wsn-flow-probe.h"
#include "ns3/log.h"

namespace ns3
{


NS_LOG_COMPONENT_DEFINE ("WsnFlowProbe");

// ------------------------------------
// ------------------------------------
// -------WsnFlowProbe-------------
// -------WsnFlowProbe------------------
// ------------------------------------
// ------------------------------------
// ------------------------------------

WsnFlowProbe::~WsnFlowProbe ()
{

}

WsnFlowProbe::WsnFlowProbe (Ptr<FlowMonitor> monitor,
                              Ptr<WsnFlowClassifier> classifier,
                              Ptr<Node> node)
  : FlowProbe (monitor),
    m_classifier (classifier)
{
  NS_LOG_FUNCTION (this << node->GetId ());

  m_wsnNwkProtocol = node->GetObject<WsnNwkProtocol> ();
  std::cout << m_wsnNwkProtocol << "\n";
  if (!m_wsnNwkProtocol->TraceConnectWithoutContext ("SendTrace",
                                           MakeCallback (&WsnFlowProbe::SendOutgoingLogger, Ptr<WsnFlowProbe> (this))))
    {
      NS_FATAL_ERROR ("trace fail");
    }
  if (!m_wsnNwkProtocol->TraceConnectWithoutContext ("UnicastForwardTrace",
                                           MakeCallback (&WsnFlowProbe::ForwardLogger, Ptr<WsnFlowProbe> (this))))
    {
      NS_FATAL_ERROR ("trace fail");
    }
  if (!m_wsnNwkProtocol->TraceConnectWithoutContext ("LocalDeliverTrace",
                                           MakeCallback (&WsnFlowProbe::ForwardUpLogger, Ptr<WsnFlowProbe> (this))))
    {
      NS_FATAL_ERROR ("trace fail");
    }
}


void
WsnFlowProbe::DoDispose ()
{
  m_wsnNwkProtocol = 0;
  m_classifier = 0;
  FlowProbe::DoDispose ();
}


void
WsnFlowProbe::SendOutgoingLogger (const NwkHeader &ipHeader, Ptr<const Packet> ipPayload)
{
  NS_LOG_FUNCTION(this << " ------- >>>");
  FlowId flowId;
  FlowPacketId packetId;
  WsnFlowProbeTag fTag;
  bool found = ipPayload->FindFirstMatchingByteTag (fTag);
  NS_LOG_FUNCTION(this << " ------- >>>" << found);

  if (found)
    {
      return;
    }

  if (m_classifier->Classify (ipHeader, ipPayload, &flowId, &packetId))
    {
      uint32_t size = (ipPayload->GetSize () + ipHeader.GetSerializedSize ());
      NS_LOG_DEBUG ("ReportFirstTx ("<<this<<", "<<flowId<<", "<<packetId<<", "<<size<<"); "
                                     << ipHeader << *ipPayload);

      m_flowMonitor->ReportFirstTx (this, flowId, packetId, size);

      WsnFlowProbeTag fTag (flowId, packetId, size, ipHeader.GetSourceAddr (), ipHeader.GetDestAddr ());
      ipPayload->AddByteTag (fTag);
    }
}


void
WsnFlowProbe::ForwardLogger (const NwkHeader &ipHeader, Ptr<const Packet> ipPayload)
{
  NS_LOG_FUNCTION(this << " ------- >>>");
  WsnFlowProbeTag fTag;
  bool found = ipPayload->FindFirstMatchingByteTag (fTag);
  NS_LOG_FUNCTION(this << " --------------------------------- >>>" << found);

    if (found)
    {
        if (!fTag.IsSrcDstValid (ipHeader.GetSourceAddr (), ipHeader.GetDestAddr ()))
        {
          NS_LOG_LOGIC ("Not reporting encapsulated packet");
          return;
        }

        FlowId flowId = fTag.GetFlowId ();
        FlowPacketId packetId = fTag.GetPacketId ();

      uint32_t size = (ipPayload->GetSize () + ipHeader.GetSerializedSize ());
      NS_LOG_DEBUG ("ReportForwarding ("<<this<<", "<<flowId<<", "<<packetId<<", "<<size<<");");
      m_flowMonitor->ReportForwarding (this, flowId, packetId, size);
    }
}

void
WsnFlowProbe::ForwardUpLogger (const NwkHeader &ipHeader, Ptr<const Packet> ipPayload)
{
  WsnFlowProbeTag fTag;
  bool found = ipPayload->FindFirstMatchingByteTag (fTag);
  NS_LOG_FUNCTION(this << " found" << found);
  if (found)
    {
      if (!fTag.IsSrcDstValid (ipHeader.GetSourceAddr (), ipHeader.GetDestAddr ()))
        {
          NS_LOG_LOGIC ("Not reporting encapsulated packet");
          return;
        }

      FlowId flowId = fTag.GetFlowId ();
      FlowPacketId packetId = fTag.GetPacketId ();

      uint32_t size = (ipPayload->GetSize () + ipHeader.GetSerializedSize ());
      NS_LOG_DEBUG ("ReportLastRx ("<<this<<", "<<flowId<<", "<<packetId<<", "<<size<<"); "
                                     << ipHeader << *ipPayload);
      m_flowMonitor->ReportLastRx (this, flowId, packetId, size);
    }
}


}   