#include <stdint.h>
#include <vector>
#include <stdexcept>
#ifndef WSN_NEIGHBOR_H
#define WSN_NEIGHBOR_H

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
    uint64_t extendedAddr;
    uint16_t networkAddr;
    DeviceTypes deviceType;
    Relationship relationship;
    bool rxOnWhenIdle;
    uint8_t linkQuality;
  };

  NeighborTable();

  void AddNeighborEntry(NeighborEntry neighbor);
  std::vector<NeighborEntry> GetNeighborEntries() const;
  NeighborEntry GetNeighborEntry(uint16_t networkAddr) const;
  bool HasNeighborEntry(uint16_t networkAddr) const;
  void RemoveNeighborEntry(uint16_t networkAddr);

  void SetExtendedAddress(uint16_t networkAddr, uint64_t extendedAddr);
  void SetDeviceType(uint16_t networkAddr, DeviceTypes deviceType);
  void SetRelationship(uint16_t networkAddr, Relationship relationship);
  void SetRxOnWhenIdle(uint16_t networkAddr, bool rxOnWhenIdle);
  void SetLinkQuality(uint16_t networkAddr, uint8_t linkQuality);

  uint64_t GetExtendedAddress(uint16_t networkAddr) const;
  DeviceTypes GetDeviceType(uint16_t networkAddr) const;
  Relationship GetRelationship(uint16_t networkAddr) const;
  bool GetRxOnWhenIdle(uint16_t networkAddr) const;
  uint8_t GetLinkQuality(uint16_t networkAddr) const;

private:
  std::vector<NeighborEntry> m_neighbors;
};

} // namespace ns3

#endif

