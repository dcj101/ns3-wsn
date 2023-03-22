#include "wsn-network-pl.h"
#include <ns3/log.h>

namespace ns3
{

NS_LOG_COMPONENT_DEFINE ("WsnNwkPayload");

NS_OBJECT_ENSURE_REGISTERED (WsnNwkPayload);

TypeId 
WsnNwkPayload::GetTypeId (void)
{
    static TypeId tid = TypeId ("ns3::WsnNwkPayload")
                        .SetParent<Tag> ()
                        .AddConstructor<WsnNwkPayload> ()
    ;
  return tid;
}

TypeId 
WsnNwkPayload::GetInstanceTypeId (void) const
{
    return GetTypeId ();
}


uint32_t 
WsnNwkPayload::GetSerializedSize (void) const
{
    return 0;
}

void 
WsnNwkPayload::Print (std::ostream &os) const
{
    os << "nwkCommandIdentifier: " << static_cast<int>(nwkCommandIdentifier) << "\n";
    os << "commandOption: " << static_cast<int>(commandOption) << "\n";
    os << "routeRequestIdentifier: " << static_cast<int>(routeRequestIdentifier) << "\n";
    os << "responseAddr: " << responseAddr << "\n";
    os << "pathCast: " << static_cast<int>(pathCast) << "\n";
    os << "originatorAddr: " << originatorAddr << "\n";
    os << "errorCode: " << static_cast<int>(errorCode) << "\n";
    os << "replayCount: " << static_cast<int>(replayCount) << "\n";
    os << "replayList: ";
    for (uint16_t addr : replayList) {
        os << addr << " ";
    }
    os << "\n";
    os << "capabilityInformation: " << static_cast<int>(capabilityInformation) << "\n";
    os << "shortAddr: " << shortAddr << "\n";
    os << "rejoinStatus: " << static_cast<int>(rejoinStatus) << "\n";
    os << "linkStatusOp: " << static_cast<int>(linkStatusOp) << "\n";
    os << "linkStatusList: ";
    for (auto& pair : linkStatusList) {
        os << "(" << pair.first << ", " << static_cast<int>(pair.second) << ") ";
    }
    os << "\n";
}

void 
WsnNwkPayload::Serialize (TagBuffer start) const
{
    start.WriteU8(nwkCommandIdentifier);
    start.WriteU8(commandOption);
    start.WriteU8(routeRequestIdentifier);
    start.WriteU16(responseAddr);
    start.WriteU8(pathCast);
    start.WriteU16(originatorAddr);
    start.WriteU8(errorCode);
    start.WriteU8(replayCount);
    
    for(std::vector<uint16_t>::const_iterator it = replayList.begin(); it != replayList.end(); it ++)
    {
        start.WriteU16(*it);
    }
    start.WriteU8(capabilityInformation);
    start.WriteU16(shortAddr);
    start.WriteU8(rejoinStatus);
    start.WriteU8(linkStatusOp);
    for(std::vector< std::pair<uint16_t,uint8_t> >::const_iterator it = linkStatusList.begin(); it != linkStatusList.end(); it ++)
    {
        start.WriteU16((*it).first);
        start.WriteU8((*it).second);
    }
}

void 
WsnNwkPayload::Deserialize (TagBuffer start)
{
    nwkCommandIdentifier = start.ReadU8();
    commandOption = start.ReadU8();
    routeRequestIdentifier = start.ReadU8();
    responseAddr = start.ReadU16();
    pathCast = start.ReadU8();
    originatorAddr = start.ReadU16();
    errorCode = start.ReadU8();
    replayCount = start.ReadU8();

    for(int i = 0; i < replayCount; ++ i) 
    {
        replayList.push_back(static_cast<uint16_t>(start.ReadU16()));
    }
    capabilityInformation = start.ReadU8();
    shortAddr = start.ReadU16();
    rejoinStatus = start.ReadU8();
    linkStatusOp = start.ReadU8();

    for(int i = 0; i < (linkStatusOp & 0x1F); ++ i) 
    {
        uint16_t NwkAddr = start.ReadU16();
        uint8_t LinkStatus = start.ReadU8();
        linkStatusList.push_back(std::make_pair(NwkAddr,LinkStatus));
    }
}


}