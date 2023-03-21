#include "wsn-neighbor-table.h"

namespace ns3 {

NeighborTable::NeighborTable() {
  // constructor
}

void 
NeighborTable::AddNeighborEntry(NeighborTable::NeighborEntry neighbor) {
  // add a new neighbor entry to the vector
  m_neighbors.push_back(neighbor);
}

std::vector<NeighborTable::NeighborEntry> 
NeighborTable::GetNeighborEntries() const {
  // return the vector of neighbor entries
  return m_neighbors;
}

NeighborTable::NeighborEntry 
NeighborTable::GetNeighborEntry(uint16_t networkAddr) const {
  // find and return the neighbor entry with the given network address
  for (auto& neighbor : m_neighbors) {
    if (neighbor.networkAddr == networkAddr) {
      return neighbor;
    }
  }
  
  // throw an exception if not found
  throw std::runtime_error("No neighbor entry with this network address");
}

bool 
NeighborTable::HasNeighborEntry(uint16_t networkAddr) const {
  // check if there is a neighbor entry with the given network address
  for (auto& neighbor : m_neighbors) {
    if (neighbor.networkAddr == networkAddr) {
      return true;
    }
  }
  return false;
}

void 
NeighborTable::RemoveNeighborEntry(uint16_t networkAddr) {
  // remove the neighbor entry with the given network address from the vector
  for (auto it = m_neighbors.begin(); it != m_neighbors.end(); ++it) {
    if (it->networkAddr == networkAddr) {
      m_neighbors.erase(it);
      break;
    }
  }
}

void 
NeighborTable::SetExtendedAddress(uint16_t networkAddr, uint64_t extendedAddr) {
  // set the extended address of the neighbor entry with the given network address
  for (auto& neighbor : m_neighbors) {
    if (neighbor.networkAddr == networkAddr) {
      neighbor.extendedAddr = extendedAddr;
      break;
    }
  }
}

void 
NeighborTable::SetDeviceType(uint16_t networkAddr, DeviceTypes deviceType) {
   // set the device type of the neighbor entry with the given network address
   for (auto& neighbor : m_neighbors) {
     if (neighbor.networkAddr == networkAddr) {
       neighbor.deviceType = deviceType;
       break;
     }
   }
}

void 
NeighborTable::SetRelationship(uint16_t networkAddr, Relationship relationship) {
   // set the relationship of the neighbor entry with the given network address
   for (auto& neighbor : m_neighbors) {
     if (neighbor.networkAddr == networkAddr) { 
       neighbor.relationship = relationship; 
       break; 
     } 
   } 
} 

void 
NeighborTable::SetRxOnWhenIdle(uint16_t networkAddr, bool rxOnWhenIdle)
{ 
   // set the rx on when idle flag of the neighbor entry with the given 
   //network address 
   for (auto& neighbor : m_neighbors)
    { 
     if (neighbor.networkAddr == networkAddr)
        { 
            neighbor.rxOnWhenIdle = rxOnWhenIdle; 
            break; 
        } 
   } 
} 



void 
NeighborTable::SetLinkQuality(uint16_t networkAddr, uint8_t linkQuality) {
  // loop through the vector and find the matching entry
  for (auto &entry : m_neighbors) {
    if (entry.networkAddr == networkAddr) {
      // update the link quality field
      entry.linkQuality = linkQuality;
      break;
    }
  }
}

uint64_t 
NeighborTable::GetExtendedAddress(uint16_t networkAddr) const {
  // loop through the vector and find the matching entry
  for (const auto &entry : m_neighbors) {
    if (entry.networkAddr == networkAddr) {
      // return the extended address field
      return entry.extendedAddr;
    }
  }
  // return a default value if not found
  return 0;
}

DeviceTypes 
NeighborTable::GetDeviceType(uint16_t networkAddr) const {
  // loop through the vector and find the matching entry
  for (const auto &entry : m_neighbors) {
    if (entry.networkAddr == networkAddr) {
      // return the device type field
      return entry.deviceType;
    }
  }
  // return a default value if not found
  return DeviceTypes::UNKNOWN;
}

Relationship 
NeighborTable::GetRelationship(uint16_t networkAddr) const {
  // loop through the vector and find the matching entry
  for (const auto &entry : m_neighbors) {
    if (entry.networkAddr == networkAddr) {
      // return the relationship field
      return entry.relationship;
    }
  }
  // return a default value if not found
  return Relationship::NO_RELATIONSHIP;
}

bool 
NeighborTable::GetRxOnWhenIdle(uint16_t networkAddr) const {
  // loop through the vector and find the matching entry
  for (const auto &entry : m_neighbors) {
    if (entry.networkAddr == networkAddr) {
      // return the rx on when idle field
      return entry.rxOnWhenIdle;
    }
  }
  // return a default value if not found
  return false;
}

uint8_t 
NeighborTable::GetLinkQuality(uint16_t networkAddr) const {
   // loop through the vector and find the matching entry 
   for (const auto &entry : m_neighbors) { 
     if (entry.networkAddr == networkAddr) { 
       // return the link quality field 
       return entry.linkQuality; 
     } 
   } 
   // return a default value if not found 
   return 0; 
 }

}