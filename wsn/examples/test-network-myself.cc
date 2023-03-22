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

int main()
{
  LogComponentEnableAll (LOG_PREFIX_TIME);
  LogComponentEnableAll (LOG_PREFIX_FUNC);
  // LogComponentEnable ("LrWpanMac", LOG_LEVEL_INFO);
  LogComponentEnable ("LrWpanMac", LOG_LEVEL_ALL);
  // LogComponentEnable ("LrWpanCsmaCa", LOG_LEVEL_INFO);
  LogComponentEnable ("LrWpanCsmaCa", LOG_LEVEL_ALL);

  LogComponentEnable ("LrWpanNetDevice", LOG_LEVEL_ALL);

  LogComponentEnable ("Node", LOG_LEVEL_ALL);



  LrWpanHelper lrWpanHelper;


/*
+0.000000000s Node:Node(0x18797c0)
+0.000000000s Node:Construct(0x18797c0)
+0.000000000s Node:Node(0x1879a70)
+0.000000000s Node:Construct(0x1879a70)
*/

  // Create 2 nodes, and a NetDevice for each one
  Ptr<Node> n0 = CreateObject <Node> ();
  Ptr<Node> n1 = CreateObject <Node> ();
  // Ptr<Node> n2 = CreateObject <Node> ();



  Ptr<LrWpanNetDevice> dev0 = CreateObject<LrWpanNetDevice> ();
  Ptr<LrWpanNetDevice> dev1 = CreateObject<LrWpanNetDevice> ();
  // Ptr<LrWpanNetDevice> dev2 = CreateObject<LrWpanNetDevice> ();

  Ptr<WsnNwkProtocol> nwk0 = CreateObject<WsnNwkProtocol>();
  Ptr<WsnNwkProtocol> nwk1 = CreateObject<WsnNwkProtocol>();

  n0->AddDevice (dev0);
  n1->AddDevice (dev1);

  nwk0->Install(n0);
  nwk1->Install(n1);


  
//   Ptr<''>

}

