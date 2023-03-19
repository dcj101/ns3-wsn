#ifndef WSN_SENSOR_INFO_H
#define WSN_SENSOR_INFO_H

#include <stdint.h>
#include "ns3/node.h"

namespace ns3
{


 class WsnSensorTag : public Tag
  {
  public:

    static TypeId GetTypeId (void);
    
    virtual TypeId GetInstanceTypeId (void) const;
    

    WsnSensorTag (void);
    
    WsnSensorTag (double temperature);
    
    virtual uint32_t GetSerializedSize (void) const;
    virtual void Serialize (TagBuffer i) const;
    virtual void Deserialize (TagBuffer i);
    virtual void Print (std::ostream &os) const;
    
    void Set (double temperature);
    
    double Get (void) const;
  private:
  
    double m_temperature;
  };

}


#endif