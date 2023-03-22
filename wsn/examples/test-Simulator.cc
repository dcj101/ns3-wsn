#include <ns3/log.h>
#include <ns3/core-module.h>
#include <ns3/lr-wpan-module.h>
#include <ns3/propagation-loss-model.h>
#include <ns3/propagation-delay-model.h>
#include <ns3/simulator.h>
#include <ns3/single-model-spectrum-channel.h>
#include <ns3/constant-position-mobility-model.h>
#include <ns3/packet.h>
#include <iostream>
using namespace ns3;

void C()
{
    NS_LOG_UNCOND (Simulator::Now ().GetSeconds () << " = nowtime is in C");
}

void B()
{
    NS_LOG_UNCOND (Simulator::Now ().GetSeconds () << " = nowtime is in B");
}

void A()
{
    NS_LOG_UNCOND (Simulator::Now ().GetSeconds () << " = nowtime is in A");
    // while(1);
    Simulator::Schedule(Seconds(2.0),&B);
    // Simulator::Stop(Seconds(3.1));
    Simulator::Schedule(Seconds(3.0),&C);
}

int main(int argc, char *argv[])
{
    LogComponentEnableAll (LOG_PREFIX_TIME);
    LogComponentEnableAll (LOG_PREFIX_FUNC);
    NS_LOG_UNCOND (Simulator::Now ().GetSeconds () << " = nowtime");

    Simulator::Schedule(Seconds(11.0),&A);
    Simulator::Schedule(Seconds(11.1),&A);
    Simulator::Stop(Seconds(200.0));
    Simulator::Run ();
    Simulator::Destroy ();

    return 0;
}