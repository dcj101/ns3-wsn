#include "wsn-sensor-tag.h"
#include <ns3/integer.h>

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (WsnSensorTag);

TypeId
WsnSensorTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::WsnSensorTag")
    .SetParent<Tag> ()
    .SetGroupName ("LrWpan")
    .AddConstructor<WsnSensorTag> ()
    .AddAttribute ("WsnSensorTag", "WsnSensorTag",
                   IntegerValue (0),
                   MakeIntegerAccessor (&WsnSensorTag::Get),
                   MakeIntegerChecker<uint8_t> ())
  ;
  return tid;
}

TypeId
WsnSensorTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

WsnSensorTag::WsnSensorTag (void)
  : m_temperature (10.0)
{
}

WsnSensorTag::WsnSensorTag (double temperature)
  : m_temperature (temperature)
{
}

uint32_t
WsnSensorTag::GetSerializedSize (void) const
{
  return sizeof (double);
}

void
WsnSensorTag::Serialize (TagBuffer i) const
{
  i.WriteU8 (m_temperature);
}

void
WsnSensorTag::Deserialize (TagBuffer i)
{
  m_temperature = i.ReadU8 ();
}

void
WsnSensorTag::Print (std::ostream &os) const
{
  os << "m_temperature = " << m_temperature;
}

void
WsnSensorTag::Set (double temperature)
{
  m_temperature = temperature;
}

double
WsnSensorTag::Get (void) const
{
  return m_temperature;
}

}
