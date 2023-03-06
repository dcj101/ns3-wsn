#include "wsn-network-pl.h"

namespace ns3
{

TypeId 
WsnPayLoad::GetTypeId (void)
{
    static TypeId tid = TypeId ("ns3::WsnPayLoad")
                        .SetParent<Tag> ()
                        .AddConstructor<Tag> ()
    ;
  return tid;
}

TypeId 
WsnPayLoad::GetInstanceTypeId (void) const
{
    return GetTypeId ();
}


uint32_t 
WsnPayLoad::GetSerializedSize (void) const
{

    return 0;
}

void 
WsnPayLoad::Serialize (TagBuffer start) const
{
    if(isCommand)
    {
        start.WriteU8(nwkCommandIdentifier);
        start.WriteU8(commandOption);
        start.WriteU8(routeRequestIdentifier);
        start.WriteU16(responseAddr);
        start.WriteU8(pathCast);
        start.WriteU16(originatorAddr);
        start.WriteU8(errorCode);
        start.WriteU8(replayCount);
        start.WriteU16(replayList);
        for(vector<uint16_t>::iterator it = replayList.begin(); it != replayList.end(); it ++)
        {
            start.WriteU16(*it);
        }
        start.WriteU8(capabilityInformation);
        start.WriteU16(shortAddr);
        start.WriteU8(rejoinStatus);
        for(vector< pair<uint16_t,uint8_t> >::iterator it = linkStatusList.begin(); it != linkStatusList.end(); it ++)
        {
            start.WriteU16((*it).first);
            start.WriteU8((*it).second);
        }
    }

    
}

void 
WsnPayLoad::Deserialize (TagBuffer start)
{
    if(isCommand)
    {
        start.ReadU8(nwkCommandIdentifier);
        start.ReadU8(commandOption);
        start.ReadU8(routeRequestIdentifier);
        start.ReadU16(responseAddr);
        start.ReadU8(pathCast);
        start.ReadU16(originatorAddr);
        start.ReadU8(errorCode);
        start.ReadU8(replayCount);
        start.ReadU16(replayList);
        for(vector<uint16_t>::iterator it = replayList.begin(); it != replayList.end(); it ++)
        {
            start.ReadU16(*it);
        }
        start.ReadU8(capabilityInformation);
        start.ReadU16(shortAddr);
        start.ReadU8(rejoinStatus);
        for(vector< pair<uint16_t,uint8_t> >::iterator it = linkStatusList.begin(); it != linkStatusList.end(); it ++)
        {
            start.ReadU16((*it).first);
            start.ReadU8((*it).second);
        }
    }
}


}