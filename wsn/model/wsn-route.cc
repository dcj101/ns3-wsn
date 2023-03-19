#include "wsn-route.h"
#include <ns3/log.h>

namespace ns3
{

NS_LOG_COMPONENT_DEFINE ("RoutingTable");

TypeId 
StaticRoute::GetType(void)
{
    static TypeId tid = TypeId ("ns3::StaticRoute")
    ;
    return tid;
}

StaticRoute::StaticRoute(NwkShortAddress dest, NwkShortAddress nextHop)
{
    m_dest = dest;
    m_nextHop = nextHop;
}

NwkShortAddress 
StaticRoute::GetDestination() const
{
    return m_dest;
}

NwkShortAddress 
StaticRoute::GetNextHop() const
{
    return m_nextHop;
}


//------------------RoutingTable---------------//

TypeId 
RoutingTable::GetType(void)
{
    static TypeId tid = TypeId ("ns3::RoutingTable")
                        ;
    return tid;
}

RoutingTable::RoutingTable()
{
    m_routes.clear();
}

void 
RoutingTable::AddRoute(const StaticRoute& route)
{
    m_routes[route.GetDestination()] = route.GetNextHop();
}

void 
RoutingTable::RemoveRoute(NwkShortAddress dest)
{
    if(m_routes.count(dest)) m_routes.erase(dest);
}

NwkShortAddress 
RoutingTable::Lookup(NwkShortAddress dest) 
{
    if(m_routes.count(dest)) return m_routes[dest];
    else return NwkShortAddress();
}

void 
RoutingTable::Print() const
{
    for(std::map<NwkShortAddress,NwkShortAddress>::const_iterator begin = m_routes.begin(); 
                            begin != m_routes.end(); ++ begin)
    {
        std::cout << begin->first << " " << begin->second << "\n"; 
    }
}

}