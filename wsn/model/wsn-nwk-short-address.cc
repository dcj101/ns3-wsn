#include "wsn-nwk-short-address.h"
#include "ns3/address.h"
#include "ns3/assert.h"
#include "ns3/log.h"
#include <iomanip>
#include <iostream>
#include <cstring>

namespace ns3 
{

NS_LOG_COMPONENT_DEFINE ("NwkShortAddress");

ATTRIBUTE_HELPER_CPP (NwkShortAddress);

#define ASCII_a (0x41)
#define ASCII_z (0x5a)
#define ASCII_A (0x61)
#define ASCII_Z (0x7a)
#define ASCII_COLON (0x3a)
#define ASCII_ZERO (0x30)

static char
AsciiToLowCase (char c)
{
  if (c >= ASCII_a && c <= ASCII_z) {
      return c;
    } else if (c >= ASCII_A && c <= ASCII_Z) {
      return c + (ASCII_a - ASCII_A);
    } else {
      return c;
    }
}


NwkShortAddress::NwkShortAddress ()
{
  NS_LOG_FUNCTION (this);
  std::memset (m_address, 0, 2);
}

NwkShortAddress::NwkShortAddress (uint16_t addr)
{
  NS_LOG_FUNCTION (this);
  m_address[0] = (addr >> 8) & 0xff;
  m_address[1] = (addr >> 0) & 0xff;
}

NwkShortAddress::NwkShortAddress (const char *str)
{
  NS_LOG_FUNCTION (this << str);
  int i = 0;
  while (*str != 0 && i < 2)
    {
      uint8_t byte = 0;
      while (*str != ASCII_COLON && *str != 0)
        {
          byte <<= 4;
          char low = AsciiToLowCase (*str);
          if (low >= ASCII_a)
            {
              byte |= low - ASCII_a + 10;
            }
          else
            {
              byte |= low - ASCII_ZERO;
            }
          str++;
        }
      m_address[i] = byte;
      i++;
      if (*str == 0)
        {
          break;
        }
      str++;
    }
  NS_ASSERT (i == 2);
}

void
NwkShortAddress::CopyFrom (const uint8_t buffer[2])
{
  NS_LOG_FUNCTION (this << &buffer);
  std::memcpy (m_address, buffer, 2);
}
void
NwkShortAddress::CopyTo (uint8_t buffer[2]) const
{
  NS_LOG_FUNCTION (this << &buffer);
  std::memcpy (buffer, m_address, 2);
}

bool
NwkShortAddress::IsMatchingType (const Address &address)
{
  NS_LOG_FUNCTION (&address);
  return address.CheckCompatible (GetType (), 2);
}

NwkShortAddress::operator Address () const
{
  return ConvertTo ();
}

NwkShortAddress
NwkShortAddress::ConvertFrom (const Address &address)
{
  NS_LOG_FUNCTION (address);
  NS_ASSERT (address.CheckCompatible (GetType (), 2));
  NwkShortAddress retval;
  address.CopyTo (retval.m_address);
  return retval;
}
Address
NwkShortAddress::ConvertTo (void) const
{
  NS_LOG_FUNCTION (this);
  return Address (GetType (), m_address, 2);
}
// 按顺序生成地址
NwkShortAddress
NwkShortAddress::Allocate (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  static uint64_t id = 0;
  id++;
  NwkShortAddress address;
  address.m_address[0] = (id >> 8) & 0xff;
  address.m_address[1] = (id >> 0) & 0xff;
  return address;
}

uint8_t
NwkShortAddress::GetType (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  static uint8_t type = Address::Register ();
  return type;
}
// 获取广播地址
NwkShortAddress
NwkShortAddress::GetBroadcast (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  static NwkShortAddress broadcast = NwkShortAddress ("ff:ff");
  return broadcast;
}

bool
NwkShortAddress::IsBroadcast (void) const
{
  NS_LOG_FUNCTION (this);
  if (m_address[0] == 0xff && m_address[1] == 0xff)
    {
      return true;
    }
  return false;
}

bool
NwkShortAddress::IsMulticast (void) const
{
  NS_LOG_FUNCTION (this);
  uint8_t val = m_address[0];
  val >>= 5;
  if (val == 0x4)
    {
      return true;
    }
  return false;
}

uint16_t
NwkShortAddress::GetAddressU16(void) const
{
  uint16_t address = 0;
  address  = (m_address[0] << 8);
  address |= (m_address[1] << 0);
  return address;
}

std::ostream & operator<< (std::ostream& os, const NwkShortAddress & address)
{
  uint8_t ad[2];
  address.CopyTo (ad);

  os.setf (std::ios::hex, std::ios::basefield);
  os.fill ('0');
  for (uint8_t i = 0; i < 1; i++)
    {
      os << std::setw (2) << (uint32_t)ad[i] << ":";
    }

  os << std::setw (2) << (uint32_t)ad[1];
  os.setf (std::ios::dec, std::ios::basefield);
  os.fill (' ');
  return os;
}

std::istream& operator>> (std::istream& is, NwkShortAddress & address)
{
  std::string v;
  is >> v;

  std::string::size_type col = 0;
  for (uint8_t i = 0; i < 2; ++i)
    {
      std::string tmp;
      std::string::size_type next;
      next = v.find (":", col);
      if (next == std::string::npos)
        {
          tmp = v.substr (col, v.size ()-col);
          address.m_address[i] = strtoul (tmp.c_str(), 0, 16);
          break;
        }
      else
        {
          tmp = v.substr (col, next-col);
          address.m_address[i] = strtoul (tmp.c_str(), 0, 16);
          col = next + 1;
        }
    }
  return is;
}

}
