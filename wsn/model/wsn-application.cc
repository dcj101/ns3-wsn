#include "wsn-application.h"

namespace ns3
{
TypeId 
WsnApplication::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns::WsnApplication")
    .SetParent<ns3::Application> ()
    .AddConstructor<WsnApplication> ()
    .AddAttribute ("StartTime", "Time at which the application will start",
                  TimeValue (Seconds (0.0)),
                  MakeTimeAccessor (&WsnApplication::m_startTime),
                  MakeTimeChecker ())
    .AddAttribute ("StopTime", "Time at which the application will stop",
                  TimeValue (TimeStep (0)),
                  MakeTimeAccessor (&WsnApplication::m_stopTime),
                  MakeTimeChecker ())
    ;
    return tid;

}

WsnApplication::WsnApplication()
{
  
}

WsnApplication::~WsnApplication()
{
}




void 
WsnApplication::StartApplication() 
{
  Simulator::Schedule (Seconds (1), &SendSensorNodeInfo);
}

void 
WsnApplication::StopApplication()
{

}

void 
WsnApplication::SendSensorNodeInfo()
{

  Simulator::Schedule (Seconds (1), &SendSensorNodeInfo);
}

}