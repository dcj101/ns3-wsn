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
NS_LOG_COMPONENT_DEFINE ("testexample");

void Test(Ptr<WsnNwkProtocol> from, Ptr<WsnNwkProtocol> to)
{
    Simulator::ScheduleWithContext(1,Seconds(20.0),&WsnNwkProtocol::Send,
                      from,from->GetNwkShortAddress(),to->GetNwkShortAddress(),
                      Create<Packet>(7),
                      NwkHeader::NWK_FRAME_DATA);
    Simulator::ScheduleWithContext(2,Seconds(20.0),&WsnNwkProtocol::Send,
                      to,to->GetNwkShortAddress(),from->GetNwkShortAddress(),
                      Create<Packet>(11),
                      NwkHeader::NWK_FRAME_DATA);
}

int main()
{
  // LogComponentEnable ("testexample", LogLevel (LOG_PREFIX_TIME | LOG_PREFIX_NODE | LOG_LEVEL_INFO));
  LogComponentEnableAll (LOG_PREFIX_TIME);
  LogComponentEnableAll (LOG_PREFIX_FUNC);
  // LogComponentEnable ("LrWpanMac", LOG_LEVEL_ALL);
  // LogComponentEnable ("LrWpanPhy", LOG_LEVEL_ALL);
  // LogComponentEnable ("LrWpanCsmaCa", LOG_LEVEL_ALL);
  // LogComponentEnable ("LrWpanNetDevice", LOG_LEVEL_ALL);
  LogComponentEnable ("WsnNwkProtocol", LOG_LEVEL_ALL);
  // LogComponentEnable ("NwkHeader", LOG_LEVEL_ALL);
  // LogComponentEnable ("WsnAddressAllocator", LOG_LEVEL_ALL);
  // LogComponentEnable ("Node", LOG_LEVEL_ALL);

  // 设置入网分配器的分配规则
  WsnAddressAllocator::Get ()->SetWsnAddressAllocator(5,3,3);

  LrWpanHelper lrWpanHelper;

  // Create 2 nodes, and a NetDevice for each one
  Ptr<Node> n0 = CreateObject <Node> ();
  Ptr<Node> n1 = CreateObject <Node> ();
  Ptr<Node> n2 = CreateObject <Node> ();
  Ptr<Node> n3 = CreateObject <Node> ();
  Ptr<Node> n4 = CreateObject <Node> ();

  Ptr<LrWpanNetDevice> dev0 = CreateObject<LrWpanNetDevice> ();
  Ptr<LrWpanNetDevice> dev1 = CreateObject<LrWpanNetDevice> ();
  Ptr<LrWpanNetDevice> dev2 = CreateObject<LrWpanNetDevice> ();
  Ptr<LrWpanNetDevice> dev3 = CreateObject<LrWpanNetDevice> ();
  Ptr<LrWpanNetDevice> dev4 = CreateObject<LrWpanNetDevice> ();

  string mac480 = WsnAddressAllocator::Get ()->AllocateRandMac48Address();
  string mac481 = WsnAddressAllocator::Get ()->AllocateRandMac48Address();
  string mac482 = WsnAddressAllocator::Get ()->AllocateRandMac48Address();
  string mac483 = WsnAddressAllocator::Get ()->AllocateRandMac48Address();
  string mac484 = WsnAddressAllocator::Get ()->AllocateRandMac48Address();


  string mac640 = WsnAddressAllocator::Get ()->AnalysisMac48AddresstoEUI64(mac480);
  std::cout << mac640 << "\n";
  string mac641 = WsnAddressAllocator::Get ()->AnalysisMac48AddresstoEUI64(mac481);
  std::cout << mac641 << "\n";
  string mac642 = WsnAddressAllocator::Get ()->AnalysisMac48AddresstoEUI64(mac482);
  std::cout << mac642 << "\n";
  string mac643 = WsnAddressAllocator::Get ()->AnalysisMac48AddresstoEUI64(mac483);
  std::cout << mac643 << "\n";
  string mac644 = WsnAddressAllocator::Get ()->AnalysisMac48AddresstoEUI64(mac484);
  std::cout << mac644 << "\n";


  dev0->GetMac()->SetExtendedAddress (Mac64Address (mac640.c_str()));
  dev1->GetMac()->SetExtendedAddress (Mac64Address (mac641.c_str()));
  dev2->GetMac()->SetExtendedAddress (Mac64Address (mac642.c_str()));
  dev3->GetMac()->SetExtendedAddress (Mac64Address (mac643.c_str()));
  dev4->GetMac()->SetExtendedAddress (Mac64Address (mac644.c_str()));

  Ptr<SingleModelSpectrumChannel> channel = CreateObject<SingleModelSpectrumChannel> ();
  // 距离传输模型
  Ptr<LogDistancePropagationLossModel> propModel = CreateObject<LogDistancePropagationLossModel> ();
  // 介质传输模型 
  Ptr<ConstantSpeedPropagationDelayModel> delayModel = CreateObject<ConstantSpeedPropagationDelayModel> ();

  channel->AddPropagationLossModel (propModel);
  channel->SetPropagationDelayModel (delayModel);

  dev0->SetChannel (channel);
  dev1->SetChannel (channel);
  dev2->SetChannel (channel);
  dev3->SetChannel (channel);
  dev4->SetChannel (channel);

  Ptr<WsnNwkProtocol> nwk0 = CreateObject<WsnNwkProtocol>(NODE_TYPE::COOR);
  Ptr<WsnNwkProtocol> nwk1 = CreateObject<WsnNwkProtocol>(NODE_TYPE::EDGE);
  Ptr<WsnNwkProtocol> nwk2 = CreateObject<WsnNwkProtocol>(NODE_TYPE::EDGE);
  Ptr<WsnNwkProtocol> nwk3 = CreateObject<WsnNwkProtocol>(NODE_TYPE::ROUTE);
  Ptr<WsnNwkProtocol> nwk4 = CreateObject<WsnNwkProtocol>(NODE_TYPE::EDGE);

  nwk0->Assign(dev0);
  nwk1->Assign(dev1);
  nwk2->Assign(dev2);
  nwk3->Assign(dev3);
  nwk4->Assign(dev4);

  n0->AddDevice (dev0);
  n1->AddDevice (dev1);
  n2->AddDevice (dev2);
  n3->AddDevice (dev3);
  n4->AddDevice (dev4);


  // Ptr<ConstantPositionMobilityModel> sender0Mobility = CreateObject<ConstantPositionMobilityModel> ();
  // sender0Mobility->SetPosition (Vector (0,0,0));
  // dev0->GetPhy ()->SetMobility (sender0Mobility);

  // Ptr<ConstantPositionMobilityModel> sender1Mobility = CreateObject<ConstantPositionMobilityModel> ();
  // sender1Mobility->SetPosition (Vector (0,10,0)); //10 m distance
  // dev1->GetPhy ()->SetMobility (sender1Mobility);

  // Ptr<ConstantPositionMobilityModel> sender2Mobility = CreateObject<ConstantPositionMobilityModel> ();
  // sender2Mobility->SetPosition (Vector (10,0,0));
  // dev2->GetPhy ()->SetMobility (sender2Mobility);

  // Ptr<ConstantPositionMobilityModel> sender3Mobility = CreateObject<ConstantPositionMobilityModel> ();
  // sender3Mobility->SetPosition (Vector (0,0,10)); //10 m distance
  // dev3->GetPhy ()->SetMobility (sender3Mobility);

  // Ptr<ConstantPositionMobilityModel> sender4Mobility = CreateObject<ConstantPositionMobilityModel> ();
  // sender4Mobility->SetPosition (Vector (0,10,10)); //10 m distance
  // dev4->GetPhy ()->SetMobility (sender4Mobility);

  nwk0->Install(n0);
  nwk1->Install(n1);
  nwk2->Install(n2);
  nwk3->Install(n3);
  nwk4->Install(n4);



  Simulator::Schedule(Seconds(0.0),&WsnNwkProtocol::JoinRequest,
                      nwk0,nwk1);
  // nwk0->JoinRequest(NODE_TYPE::COOR,NULL);
  Simulator::Schedule(Seconds(5.0),&WsnNwkProtocol::JoinRequest,
                      nwk3,nwk0);
  // nwk1->JoinRequest(NODE_TYPE::EDGE,nwk1);

  Simulator::Schedule(Seconds(10.0),&WsnNwkProtocol::JoinRequest,
                      nwk1,nwk0);

  Simulator::Schedule(Seconds(15.0),&WsnNwkProtocol::JoinRequest,
                      nwk2,nwk3);

  Simulator::Schedule(Seconds(20.0),&WsnNwkProtocol::JoinRequest,
                      nwk4,nwk3);        

  Simulator::Schedule(Seconds(25.0),&Test,nwk2,nwk1);
  Simulator::Schedule(Seconds(30.0),&Test,nwk4,nwk1);
  Simulator::Stop(Seconds(600));
  Simulator::Run ();
  Simulator::Destroy ();

  return 0;

}

