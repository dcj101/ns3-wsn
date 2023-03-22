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
  LogComponentEnable ("WsnNwkProtocol", LOG_LEVEL_ALL);
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

  string mac640 = WsnAddressAllocator::Get ()->AnalysisMac48AddresstoEUI64(mac480);
  string mac641 = WsnAddressAllocator::Get ()->AnalysisMac48AddresstoEUI64(mac481);

  dev0->GetMac()->SetExtendedAddress (Mac64Address (mac640.c_str()));
  dev1->GetMac()->SetExtendedAddress (Mac64Address (mac641.c_str()));

  Ptr<SingleModelSpectrumChannel> channel = CreateObject<SingleModelSpectrumChannel> ();
  // 距离传输模型
  Ptr<LogDistancePropagationLossModel> propModel = CreateObject<LogDistancePropagationLossModel> ();
  // 介质传输模型 
  Ptr<ConstantSpeedPropagationDelayModel> delayModel = CreateObject<ConstantSpeedPropagationDelayModel> ();
  channel->AddPropagationLossModel (propModel);
  channel->SetPropagationDelayModel (delayModel);

  dev0->SetChannel (channel);
  dev1->SetChannel (channel);
  // dev2->SetChannel (channel);

  Ptr<WsnNwkProtocol> nwk0 = CreateObject<WsnNwkProtocol>(NODE_TYPE::COOR);
  Ptr<WsnNwkProtocol> nwk1 = CreateObject<WsnNwkProtocol>(NODE_TYPE::EDGE);

  nwk0->Assign(dev0);
  nwk1->Assign(dev1);

  n0->AddDevice (dev0);
  n1->AddDevice (dev1);

  nwk0->Install(n0);
  nwk1->Install(n1);



  Simulator::Schedule(Seconds(0.5),&WsnNwkProtocol::JoinRequest,
                      nwk0,nwk1);
  // nwk0->JoinRequest(NODE_TYPE::COOR,NULL);
  Simulator::Schedule(Seconds(2.0),&WsnNwkProtocol::JoinRequest,
                      nwk1,nwk0);
  // nwk1->JoinRequest(NODE_TYPE::EDGE,nwk1);

  Simulator::Run ();
  Simulator::Destroy ();

  return 0;

}

