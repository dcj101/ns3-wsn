#ifndef WSN_SENSOR_INFO_H
#define WSN_SENSOR_INFO_H

#include <stdint.h>
#include "ns3/node.h"

namespace ns3
{

struct MVector2D
{
    double x;
    double y;
    MVector2D(double x_,double y_):x(x_),y(y_){}
    MVector2D():x(0),y(0){}
};

class SensorNode : public Node
{
private:

    uint16_t m_id;
    MVector2D m_position;
    uint8_t m_nodeType;
    double m_sensorData;

public:
    static TypeId GetTypeId (void);

    uint16_t GetId() const;
    void SetId(uint16_t id);

    MVector2D GetPosition() const;
    void SetPosition(MVector2D position);

    uint8_t GetNodeType() const;
    void SetNodeType(uint8_t nodeType);

    double GetSensorData() const;
    void SetSensorData(double sensorData);
};

}


#endif