#include <ns3/log.h>
#include <ns3/core-module.h>
#include <ns3/lr-wpan-module.h>
#include <ns3/propagation-loss-model.h>
#include <ns3/propagation-delay-model.h>
#include <ns3/simulator.h>
#include <ns3/single-model-spectrum-channel.h>
#include <ns3/constant-position-mobility-model.h>
#include <ns3/packet.h>
#include <ns3/wsn-module.h>
#include <iostream>

using namespace ns3;
using namespace std;

int main()
{
  LogComponentEnableAll (LOG_PREFIX_TIME);
  LogComponentEnableAll (LOG_PREFIX_FUNC);
  LogComponentEnable ("LrWpanMac", LOG_LEVEL_ALL);
  LogComponentEnable ("LrWpanCsmaCa", LOG_LEVEL_ALL);
  LogComponentEnable ("LrWpanNetDevice", LOG_LEVEL_ALL);
  LogComponentEnable ("Node", LOG_LEVEL_ALL);

  // 设置入网分配器的分配规则
  WsnAddressAllocator::Get ()->SetWsnAddressAllocator(5,3,3);

  LrWpanHelper lrWpanHelper;

  // Create 2 nodes, and a NetDevice for each one
  Ptr<Node> n0 = CreateObject <Node> ();
  Ptr<Node> n1 = CreateObject <Node> ();
  // Ptr<Node> n2 = CreateObject <Node> ();

  Ptr<LrWpanNetDevice> dev0 = CreateObject<LrWpanNetDevice> ();
  Ptr<LrWpanNetDevice> dev1 = CreateObject<LrWpanNetDevice> ();
  // Ptr<LrWpanNetDevice> dev2 = CreateObject<LrWpanNetDevice> ();

  string mac480 = WsnAddressAllocator::Get ()->AllocateRandMac48Address();
  string mac481 = WsnAddressAllocator::Get ()->AllocateRandMac48Address();

  dev0->GetMac()->SetExtendedAddress (Mac64Address (mac480.c_str()));
  dev1->GetMac()->SetExtendedAddress (Mac64Address (mac481.c_str()));

  Ptr<WsnNwkProtocol> nwk0 = CreateObject<WsnNwkProtocol>();
  Ptr<WsnNwkProtocol> nwk1 = CreateObject<WsnNwkProtocol>();

  n0->AddDevice (dev0);
  n1->AddDevice (dev1);

  nwk0->Install(n0);
  nwk1->Install(n1);

  Simulator::Schedule(Seconds(0.0),&WsnNwkProtocol::JoinRequest(),
                      nwk0,NODE_TYPE::COOR,NULL);
  // nwk0->JoinRequest(NODE_TYPE::COOR,NULL);
  Simulator::Schedule(Seconds(1.0),&WsnNwkProtocol::JoinRequest(),
                      nwk1,NODE_TYPE::EDGE,nwk1);
  // nwk1->JoinRequest(NODE_TYPE::EDGE,nwk1);

  Simulator::Run ();
  Simulator::Destroy ();

  
//   Ptr<''>

}

