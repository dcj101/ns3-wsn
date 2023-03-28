#include "wsn-flow-classifier.h"
#include "ns3/log.h"

namespace ns3
{
NS_LOG_COMPONENT_DEFINE ("WsnFlowClassifier");


bool operator < (const WsnFlowClassifier::FourTuple &t1,
                 const WsnFlowClassifier::FourTuple &t2)
{
  if (t1.sourceAddress < t2.sourceAddress)
    {
      return true;
    }
  if (t1.sourceAddress != t2.sourceAddress)
    {
      return false;
    }

  if (t1.destinationAddress < t2.destinationAddress)
    {
      return true;
    }
  if (t1.destinationAddress != t2.destinationAddress)
    {
      return false;
    }

  if (t1.sourcePoint < t2.sourcePoint)
    {
      return true;
    }
  if (t1.sourcePoint != t2.sourcePoint)
    {
      return false;
    }

  if (t1.destinationPoint < t2.destinationPoint)
    {
      return true;
    }
  if (t1.destinationPoint != t2.destinationPoint)
    {
      return false;
    }

  return false;
}

bool operator == (const WsnFlowClassifier::FourTuple &t1,
                  const WsnFlowClassifier::FourTuple &t2)
{
  return (t1.sourceAddress      == t2.sourceAddress &&
          t1.destinationAddress == t2.destinationAddress &&
          t1.sourcePoint         == t2.sourcePoint &&
          t1.destinationPoint    == t2.destinationPoint);
}

WsnFlowClassifier::WsnFlowClassifier ()
{
}

bool
WsnFlowClassifier::Classify (const NwkHeader &ipHeader, Ptr<const Packet> ipPayload,
                              uint32_t *out_flowId, uint32_t *out_packetId)
{
  FourTuple tuple;
  tuple.sourceAddress = ipHeader.GetSourceAddr ();
  tuple.destinationAddress = ipHeader.GetDestAddr ();

  uint8_t data[2];
  ipPayload->CopyData (data, 2);

  tuple.sourcePoint = data[0];
  tuple.destinationPoint = data[1];

  std::pair<std::map<FourTuple, FlowId>::iterator, bool> insert
    = m_flowMap.insert (std::pair<FourTuple, FlowId> (tuple, 0));

  if (insert.second)
    {
      FlowId newFlowId = GetNewFlowId ();
      insert.first->second = newFlowId;
      m_flowPktIdMap[newFlowId] = 0;
    }
  else
    {
      m_flowPktIdMap[insert.first->second] ++;
    }

  *out_flowId = insert.first->second;
  *out_packetId = m_flowPktIdMap[*out_flowId];
  return true;
}


WsnFlowClassifier::FourTuple
WsnFlowClassifier::FindFlow (FlowId flowId) const
{
  for (std::map<FourTuple, FlowId>::const_iterator
       iter = m_flowMap.begin (); iter != m_flowMap.end (); iter++)
    {
      if (iter->second == flowId)
        {
          return iter->first;
        }
    }
  NS_FATAL_ERROR ("Could not find the flow with ID " << flowId);
  FourTuple retval = { NwkShortAddress((uint16_t)0), NwkShortAddress((uint16_t)0), 0, 0 };
  return retval;
}

void
WsnFlowClassifier::SerializeToXmlStream (std::ostream &os, uint16_t indent) const
{
  Indent (os, indent); os << "<WsnFlowClassifier>\n";

  indent += 2;
  for (std::map<FourTuple, FlowId>::const_iterator
       iter = m_flowMap.begin (); iter != m_flowMap.end (); iter++)
    {
      Indent (os, indent);
      os << "<Flow flowId=\"" << iter->second << "\""
         << " sourceAddress=\"" << iter->first.sourceAddress << "\""
         << " destinationAddress=\"" << iter->first.destinationAddress << "\""
         << " sourcePoint=\"" << iter->first.sourcePoint << "\""
         << " destinationPoint=\"" << iter->first.destinationPoint << "\">\n";

      Indent (os, indent); os << "</Flow>\n";
    }
  indent -= 2;
  Indent (os, indent); os << "</WsnFlowClassifier>\n";
}

}