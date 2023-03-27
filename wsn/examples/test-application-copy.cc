#include "ns3/core-module.h"
#include "ns3/wsn-address-allocator.h"
#include "ns3/log.h"
#include "ns3/ptr.h"
#include "ns3/wsn-application.h"
#include <iostream>
using namespace ns3;
NS_LOG_COMPONENT_DEFINE ("testExample");


int main () {
    LogComponentEnable ("testExample", LogLevel (LOG_PREFIX_TIME | LOG_PREFIX_NODE | LOG_LEVEL_INFO));
    // 定义 ZigbeeAddressAllocator 对象
    AppHeader wsnapp;
    WsnSensorTag sensor;
    wsnapp.SetSourceEndpoint(11);
    wsnapp.SetDestinationEndpoint(13);
    Ptr<Packet> packet = Create<Packet>(10);
    packet->AddHeader(wsnapp);
    packet->AddPacketTag(sensor);
    uint8_t data[2]={0};
    packet->CopyData(data,2);
    NS_LOG_INFO((unsigned)data[0] << "---" << (unsigned)data[1]);
    return 0;
}
