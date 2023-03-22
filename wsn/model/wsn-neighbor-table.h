
#ifndef WSN_NEIGHBOR_H
#define WSN_NEIGHBOR_H

#include <stdint.h>
#include <vector>
#include <stdexcept>

#include "wsn-nwk-short-address.h"
#include "ns3/mac64-address.h"

namespace ns3 {

enum class DeviceTypes {
  COORDINATOR,
  ROUTER,
  END_DEVICE,
  UNKNOWN
};

enum class Relationship {
  PARENT,
  CHILD,
  SIBLING,
  NO_RELATIONSHIP
};

class NeighborTable {
public:
  struct NeighborEntry {
    Mac64Address extendedAddr;
    NwkShortAddress networkAddr;
    DeviceTypes deviceType;
    Relationship relationship;
    bool rxOnWhenIdle;
    uint8_t linkQuality;
  };

  NeighborTable();

  void AddNeighborEntry(NeighborEntry neighbor);
  std::vector<NeighborEntry> GetNeighborEntries() const;
  NeighborEntry GetNeighborEntry(NwkShortAddress networkAddr) const;
  bool HasNeighborEntry(NwkShortAddress networkAddr) const;
  void RemoveNeighborEntry(NwkShortAddress networkAddr);

  void SetExtendedAddress(NwkShortAddress networkAddr, Mac64Address extendedAddr);
  void SetDeviceType(NwkShortAddress networkAddr, DeviceTypes deviceType);
  void SetRelationship(NwkShortAddress networkAddr, Relationship relationship);
  void SetRxOnWhenIdle(NwkShortAddress networkAddr, bool rxOnWhenIdle);
  void SetLinkQuality(NwkShortAddress networkAddr, uint8_t linkQuality);

  Mac64Address GetExtendedAddress(NwkShortAddress networkAddr) const;
  DeviceTypes GetDeviceType(NwkShortAddress networkAddr) const;
  Relationship GetRelationship(NwkShortAddress networkAddr) const;
  bool GetRxOnWhenIdle(NwkShortAddress networkAddr) const;
  uint8_t GetLinkQuality(NwkShortAddress networkAddr) const;

private:
  std::vector<NeighborEntry> m_neighbors;
};

} // namespace ns3

#endif

