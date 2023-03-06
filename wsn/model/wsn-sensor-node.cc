#include "wsn-sensor-node.h"

namespace ns3 
{
    
TypeId 
SensorNode::GetTypeId (void)
{
  
  static TypeId tid = TypeId ("ns3::SensorNode")
    .SetParent<Node> ()
    .AddConstructor<Node> ()
  ;
  return tid;
}


uint16_t 
SensorNode::GetId() const 
{
  return m_id;
}

void 
SensorNode::SetId(uint16_t id) 
{
  m_id = id;
}

MVector2D 
SensorNode::GetPosition() const 
{
  return m_position;
}

void 
SensorNode::SetPosition(MVector2D position) 
{
  m_position = position;
}

uint8_t 
SensorNode::GetNodeType() const 
{
  return m_nodeType;
}

void 
SensorNode::SetNodeType(uint8_t nodeType) 
{
  m_nodeType = nodeType;
}

double 
SensorNode::GetSensorData() const 
{
  return m_sensorData;
}

void 
SensorNode::SetSensorData(double sensorData) 
{
  m_sensorData = sensorData;
}

}