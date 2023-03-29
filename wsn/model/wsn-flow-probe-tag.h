#ifndef WSN_FLOW_TAG_H
#define WSN_FLOW_TAG_H
#include "wsn-nwk-short-address.h"
#include "ns3/type-id.h"
#include "ns3/flow-probe.h"
#include "ns3/flow-monitor.h"
#include "wsn-flow-classifier.h"
#include "wsn-network.h"

namespace ns3
{

class WsnFlowProbeTag : public Tag
{
public:

  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (TagBuffer buf) const;
  virtual void Deserialize (TagBuffer buf);
  virtual void Print (std::ostream &os) const;
  WsnFlowProbeTag ();

  WsnFlowProbeTag (uint32_t flowId, uint32_t packetId, uint32_t packetSize, NwkShortAddress src, NwkShortAddress dst);

  void SetFlowId (uint32_t flowId);

  void SetPacketId (uint32_t packetId);

  void SetPacketSize (uint32_t packetSize);

  uint32_t GetFlowId (void) const;

  uint32_t GetPacketId (void) const;

  uint32_t GetPacketSize (void) const;

  bool IsSrcDstValid (NwkShortAddress src, NwkShortAddress dst) const;
private:
  uint32_t m_flowId;      //!< flow identifier
  uint32_t m_packetId;    //!< packet identifier
  uint32_t m_packetSize;  //!< packet size
  NwkShortAddress m_src;      //!< IP source
  NwkShortAddress m_dst;      //!< IP destination
};

}

#endif