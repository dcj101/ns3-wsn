#ifndef WSN_NWK_SHORT_ADDRESS_H
#define WSN_NWK_SHORT_ADDRESS_H

#include <stdint.h>
#include <ostream>
#include "ns3/attribute.h"
#include "ns3/attribute-helper.h"
#include "ns3/address.h"
#include <cstring>
#include "ns3/log.h"

namespace ns3 {

class Address;

class NwkShortAddress
{
public:
  NwkShortAddress ();

  

  NwkShortAddress(uint16_t);

  NwkShortAddress (const char *str);

  void CopyFrom (const uint8_t buffer[2]);

  void CopyTo (uint8_t buffer[2]) const;

  operator Address () const;

  static NwkShortAddress ConvertFrom (const Address &address);

  static bool IsMatchingType (const Address &address);

  static NwkShortAddress Allocate (void);

  static NwkShortAddress GetBroadcast (void);

  bool IsBroadcast (void) const;

  bool IsMulticast (void) const;

  uint16_t GetAddressU16(void) const;

private:

  Address ConvertTo (void) const;

  static uint8_t GetType (void);

  friend bool operator == (const NwkShortAddress &a, const NwkShortAddress &b);


  friend bool operator != (const NwkShortAddress &a, const NwkShortAddress &b);


  friend bool operator < (const NwkShortAddress &a, const NwkShortAddress &b);

  friend std::ostream& operator<< (std::ostream& os, const NwkShortAddress & address);

  friend std::istream& operator>> (std::istream& is, NwkShortAddress & address);

  uint8_t m_address[2]; 
};

ATTRIBUTE_HELPER_HEADER (NwkShortAddress);

inline bool operator == (const NwkShortAddress &a, const NwkShortAddress &b)
{
  return std::memcmp (a.m_address, b.m_address, 2) == 0;
}
inline bool operator != (const NwkShortAddress &a, const NwkShortAddress &b)
{
  return std::memcmp (a.m_address, b.m_address, 2) != 0;
}
inline bool operator < (const NwkShortAddress &a, const NwkShortAddress &b)
{
  return std::memcmp (a.m_address, b.m_address, 2) < 0;
}

std::ostream& operator<< (std::ostream& os, const NwkShortAddress & address);
std::istream& operator>> (std::istream& is, NwkShortAddress & address);

} 
#endif
