#ifndef WSN_HELPER_H
#define WSN_HELPER_H

#include <ns3/node-container.h>
#include <ns3/lr-wpan-phy.h>
#include <ns3/lr-wpan-mac.h>
#include <ns3/trace-helper.h>

namespace ns3 {

class SpectrumChannel;
class MobilityModel;

class WsnHelper : public PcapHelperForDevice,
                     public AsciiTraceHelperForDevice
{
public:

  WsnHelper (void);

  WsnHelper (bool useMultiModelSpectrumChannel);

  virtual ~WsnHelper (void);

  Ptr<SpectrumChannel> GetChannel (void);

  void SetChannel (Ptr<SpectrumChannel> channel);

  void SetChannel (std::string channelName);

  void AddMobility (Ptr<LrWpanPhy> phy, Ptr<MobilityModel> m);

  NetDeviceContainer Install (NodeContainer c);

  void AssociateToPan (NetDeviceContainer c, uint16_t panId);


  void AssociateToBeaconPan (NetDeviceContainer c, uint16_t panId, Mac16Address coor, uint8_t bcnOrd, uint8_t sfrmOrd);

  void EnableLogComponents (void);

  static std::string LrWpanPhyEnumerationPrinter (LrWpanPhyEnumeration e);

  static std::string LrWpanMacStatePrinter (LrWpanMacState e);

  int64_t AssignStreams (NetDeviceContainer c, int64_t stream);

private:

  WsnHelper (WsnHelper const &);

  WsnHelper& operator= (WsnHelper const &);

  virtual void EnablePcapInternal (std::string prefix, Ptr<NetDevice> nd, bool promiscuous, bool explicitFilename);

  virtual void EnableAsciiInternal (Ptr<OutputStreamWrapper> stream,
                                    std::string prefix,
                                    Ptr<NetDevice> nd,
                                    bool explicitFilename);

private:
  Ptr<SpectrumChannel> m_channel; 
  
};

}

#endif
