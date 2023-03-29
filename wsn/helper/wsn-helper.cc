#include "wsn-helper.h"
#include <ns3/lr-wpan-csmaca.h>
#include <ns3/lr-wpan-error-model.h>
#include <ns3/lr-wpan-net-device.h>
#include <ns3/mobility-model.h>
#include <ns3/single-model-spectrum-channel.h>
#include <ns3/multi-model-spectrum-channel.h>
#include <ns3/propagation-loss-model.h>
#include <ns3/propagation-delay-model.h>
#include <ns3/log.h>
#include "ns3/names.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("WsnHelper");

static void
AsciiLrWpanMacTransmitSinkWithContext (
  Ptr<OutputStreamWrapper> stream,
  std::string context,
  Ptr<const Packet> p)
{
  *stream->GetStream () << "t " << Simulator::Now ().As (Time::S) << " " << context << " " << *p << std::endl;
}

static void
AsciiLrWpanMacTransmitSinkWithoutContext (
  Ptr<OutputStreamWrapper> stream,
  Ptr<const Packet> p)
{
  *stream->GetStream () << "t " << Simulator::Now ().As (Time::S) << " " << *p << std::endl;
}
// 这里设置了默认的损耗模型
WsnHelper::WsnHelper (void)
{
  m_channel = CreateObject<SingleModelSpectrumChannel> ();

  Ptr<LogDistancePropagationLossModel> lossModel = CreateObject<LogDistancePropagationLossModel> ();
  m_channel->AddPropagationLossModel (lossModel);

  Ptr<ConstantSpeedPropagationDelayModel> delayModel = CreateObject<ConstantSpeedPropagationDelayModel> ();
  m_channel->SetPropagationDelayModel (delayModel);
}

WsnHelper::WsnHelper (bool useMultiModelSpectrumChannel)
{
  if (useMultiModelSpectrumChannel)
    {
      m_channel = CreateObject<MultiModelSpectrumChannel> ();
    }
  else
    {
      m_channel = CreateObject<SingleModelSpectrumChannel> ();
    }
  Ptr<LogDistancePropagationLossModel> lossModel = CreateObject<LogDistancePropagationLossModel> ();
  m_channel->AddPropagationLossModel (lossModel);

  Ptr<ConstantSpeedPropagationDelayModel> delayModel = CreateObject<ConstantSpeedPropagationDelayModel> ();
  m_channel->SetPropagationDelayModel (delayModel);
}

WsnHelper::~WsnHelper (void)
{
  m_channel->Dispose ();
  m_channel = 0;
}

// 打开日志
void
WsnHelper::EnableLogComponents (void)
{
  LogComponentEnableAll (LOG_PREFIX_TIME);
  LogComponentEnableAll (LOG_PREFIX_FUNC);
  LogComponentEnable ("LrWpanCsmaCa", LOG_LEVEL_ALL);
  LogComponentEnable ("LrWpanErrorModel", LOG_LEVEL_ALL);
  LogComponentEnable ("LrWpanInterferenceHelper", LOG_LEVEL_ALL);
  LogComponentEnable ("LrWpanMac", LOG_LEVEL_ALL);
  LogComponentEnable ("LrWpanNetDevice", LOG_LEVEL_ALL);
  LogComponentEnable ("LrWpanPhy", LOG_LEVEL_ALL);
  LogComponentEnable ("LrWpanSpectrumSignalParameters", LOG_LEVEL_ALL);
  LogComponentEnable ("LrWpanSpectrumValueHelper", LOG_LEVEL_ALL);
}
// 打开PHY的状态
std::string
WsnHelper::LrWpanPhyEnumerationPrinter (LrWpanPhyEnumeration e)
{
  switch (e)
    {
    case IEEE_802_15_4_PHY_BUSY:
      return std::string ("BUSY");
    case IEEE_802_15_4_PHY_BUSY_RX:
      return std::string ("BUSY_RX");
    case IEEE_802_15_4_PHY_BUSY_TX:
      return std::string ("BUSY_TX");
    case IEEE_802_15_4_PHY_FORCE_TRX_OFF:
      return std::string ("FORCE_TRX_OFF");
    case IEEE_802_15_4_PHY_IDLE:
      return std::string ("IDLE");
    case IEEE_802_15_4_PHY_INVALID_PARAMETER:
      return std::string ("INVALID_PARAMETER");
    case IEEE_802_15_4_PHY_RX_ON:
      return std::string ("RX_ON");
    case IEEE_802_15_4_PHY_SUCCESS:
      return std::string ("SUCCESS");
    case IEEE_802_15_4_PHY_TRX_OFF:
      return std::string ("TRX_OFF");
    case IEEE_802_15_4_PHY_TX_ON:
      return std::string ("TX_ON");
    case IEEE_802_15_4_PHY_UNSUPPORTED_ATTRIBUTE:
      return std::string ("UNSUPPORTED_ATTRIBUTE");
    case IEEE_802_15_4_PHY_READ_ONLY:
      return std::string ("READ_ONLY");
    case IEEE_802_15_4_PHY_UNSPECIFIED:
      return std::string ("UNSPECIFIED");
    default:
      return std::string ("INVALID");
    }
}

// MAC状态

std::string
WsnHelper::LrWpanMacStatePrinter (LrWpanMacState e)
{
  switch (e)
    {
    case MAC_IDLE:
      return std::string ("MAC_IDLE");
    case CHANNEL_ACCESS_FAILURE:
      return std::string ("CHANNEL_ACCESS_FAILURE");
    case CHANNEL_IDLE:
      return std::string ("CHANNEL_IDLE");
    case SET_PHY_TX_ON:
      return std::string ("SET_PHY_TX_ON");
    default:
      return std::string ("INVALID");
    }
}

//添加移动模型

void
WsnHelper::AddMobility (Ptr<LrWpanPhy> phy, Ptr<MobilityModel> m)
{
  phy->SetMobility (m);
}

//添加设备集合

NetDeviceContainer
WsnHelper::Install (NodeContainer c)
{
  NetDeviceContainer devices;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); i++)
    {
      Ptr<Node> node = *i;

      Ptr<LrWpanNetDevice> netDevice = CreateObject<LrWpanNetDevice> ();
      netDevice->SetChannel (m_channel);
      node->AddDevice (netDevice);
      netDevice->SetNode (node);
      // \todo add the capability to change short address, extended
      // address and panId. Right now they are hardcoded in LrWpanMac::LrWpanMac ()
      devices.Add (netDevice);
    }
  return devices;
}


Ptr<SpectrumChannel>
WsnHelper::GetChannel (void)
{
  return m_channel;
}

void
WsnHelper::SetChannel (Ptr<SpectrumChannel> channel)
{
  m_channel = channel;
}

void
WsnHelper::SetChannel (std::string channelName)
{
  Ptr<SpectrumChannel> channel = Names::Find<SpectrumChannel> (channelName);
  m_channel = channel;
}
/*
随机数流在lr-wpan里面的作用可能与安全性或干扰抗性有关。
例如，IEEE 802.15.4标准支持多种调制方式和信道选择，可以利用随机数流来实现动态频率选择或跳频技术34。
另外，IEEE 802.15.4标准也支持加密和认证功能，可以利用随机数流来生成密钥或初始化向量4。
*/

int64_t
WsnHelper::AssignStreams (NetDeviceContainer c, int64_t stream)
{
  int64_t currentStream = stream;
  Ptr<NetDevice> netDevice;
  for (NetDeviceContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      netDevice = (*i);
      Ptr<LrWpanNetDevice> lrwpan = DynamicCast<LrWpanNetDevice> (netDevice);
      if (lrwpan)
        {
          currentStream += lrwpan->AssignStreams (currentStream);
        }
    }
  return (currentStream - stream);
}
// 这个只处理非协调器节点
void
WsnHelper::AssociateToPan (NetDeviceContainer c, uint16_t panId)
{
  NetDeviceContainer devices;
  uint16_t id = 1;
  uint8_t idBuf[2];

  for (NetDeviceContainer::Iterator i = c.Begin (); i != c.End (); i++)
    {
      Ptr<LrWpanNetDevice> device = DynamicCast<LrWpanNetDevice> (*i);
      if (device)
        {
          idBuf[0] = (id >> 8) & 0xff;
          idBuf[1] = (id >> 0) & 0xff;
          Mac16Address address;
          address.CopyFrom (idBuf);

          device->GetMac ()->SetPanId (panId);
          device->GetMac ()->SetShortAddress (address);
          id++;
        }
    }
  return;
}

// 这个函数可以处理一个设备集合

void
WsnHelper::AssociateToBeaconPan (NetDeviceContainer c, uint16_t panId, Mac16Address coor, uint8_t bcnOrd, uint8_t sfrmOrd)
{
  // 设备
  NetDeviceContainer devices;
  uint16_t id = 1;
  uint8_t idBuf[2];
  Mac16Address address;
  // 设置信标的时间间隔
  if (bcnOrd > 14)
    {
      NS_LOG_DEBUG("The Beacon Order must be an int between 0 and 14");
      return;
    }

  // 设置超帧的时间间隔
  if ((sfrmOrd > 14) || (sfrmOrd > bcnOrd))
    {
      NS_LOG_DEBUG("The Superframe Order must be an int between 0 and 14, and less or equal to Beacon Order");
      return;
    }

  for (NetDeviceContainer::Iterator i = c.Begin (); i != c.End (); i++)
    {
      Ptr<LrWpanNetDevice> device = DynamicCast<LrWpanNetDevice> (*i);
      if (device)
        {
          idBuf[0] = (id >> 8) & 0xff;
          idBuf[1] = (id >> 0) & 0xff;
          address.CopyFrom (idBuf);

          device->GetMac ()->SetShortAddress (address);
          // 如果地址是协调器的地址
          if (address == coor)
            {
              MlmeStartRequestParams params;
              params.m_panCoor = true; // 把他设置成协调器
              params.m_PanId = panId;
              params.m_bcnOrd = bcnOrd;
              params.m_sfrmOrd = sfrmOrd;
              // 生成一个随机均匀分布的随机数
              Ptr<UniformRandomVariable> uniformRandomVariable = CreateObject<UniformRandomVariable> ();;
              Time jitter = Time (MilliSeconds (uniformRandomVariable->GetInteger (0, 10)));
              // 随机一个时间开始调用MlmeStartRequest 这是开始建立coor网络
              Simulator::Schedule (jitter, &LrWpanMac::MlmeStartRequest,
                                              device->GetMac (), params);
            }
          else
            {
              //如果不是coor协调器那么就是直接开始连接到pan网络
              device->GetMac ()->SetPanId (panId);
              device->GetMac ()->SetAssociatedCoor(coor);
            }
          id++;
        }
    }
  return;
}

static void
PcapSniffLrWpan (Ptr<PcapFileWrapper> file, Ptr<const Packet> packet)
{
  file->Write (Simulator::Now (), packet);
}

// 这是pacp日志
void
WsnHelper::EnablePcapInternal (std::string prefix, Ptr<NetDevice> nd, bool promiscuous, bool explicitFilename)
{
  NS_LOG_FUNCTION (this << prefix << nd << promiscuous << explicitFilename);

  Ptr<LrWpanNetDevice> device = nd->GetObject<LrWpanNetDevice> ();
  if (device == 0)
    {
      NS_LOG_INFO ("WsnHelper::EnablePcapInternal(): Device " << device << " not of type ns3::LrWpanNetDevice");
      return;
    }

  PcapHelper pcapHelper;

  std::string filename;
  if (explicitFilename)
    {
      filename = prefix;
    }
  else
    {
      filename = pcapHelper.GetFilenameFromDevice (prefix, device);
    }

  Ptr<PcapFileWrapper> file = pcapHelper.CreateFile (filename, std::ios::out,
                                                     PcapHelper::DLT_IEEE802_15_4);

  if (promiscuous == true)
    {
      device->GetMac ()->TraceConnectWithoutContext ("PromiscSniffer", MakeBoundCallback (&PcapSniffLrWpan, file));

    }
  else
    {
      device->GetMac ()->TraceConnectWithoutContext ("Sniffer", MakeBoundCallback (&PcapSniffLrWpan, file));
    }
}

void
WsnHelper::EnableAsciiInternal (
  Ptr<OutputStreamWrapper> stream,
  std::string prefix,
  Ptr<NetDevice> nd,
  bool explicitFilename)
{
  uint32_t nodeid = nd->GetNode ()->GetId ();
  uint32_t deviceid = nd->GetIfIndex ();
  std::ostringstream oss;

  Ptr<LrWpanNetDevice> device = nd->GetObject<LrWpanNetDevice> ();
  if (device == 0)
    {
      NS_LOG_INFO ("WsnHelper::EnableAsciiInternal(): Device " << device << " not of type ns3::LrWpanNetDevice");
      return;
    }

  //
  // Our default trace sinks are going to use packet printing, so we have to
  // make sure that is turned on.
  //
  Packet::EnablePrinting ();

  //
  // If we are not provided an OutputStreamWrapper, we are expected to create
  // one using the usual trace filename conventions and do a Hook*WithoutContext
  // since there will be one file per context and therefore the context would
  // be redundant.
  //
  if (stream == 0)
    {
      //
      // Set up an output stream object to deal with private ofstream copy
      // constructor and lifetime issues.  Let the helper decide the actual
      // name of the file given the prefix.
      //
      AsciiTraceHelper asciiTraceHelper;

      std::string filename;
      if (explicitFilename)
        {
          filename = prefix;
        }
      else
        {
          filename = asciiTraceHelper.GetFilenameFromDevice (prefix, device);
        }

      Ptr<OutputStreamWrapper> theStream = asciiTraceHelper.CreateFileStream (filename);

      // Ascii traces typically have "+", '-", "d", "r", and sometimes "t"
      // The Mac and Phy objects have the trace sources for these
      //

      asciiTraceHelper.HookDefaultReceiveSinkWithoutContext<LrWpanMac> (device->GetMac (), "MacRx", theStream);

      device->GetMac ()->TraceConnectWithoutContext ("MacTx", MakeBoundCallback (&AsciiLrWpanMacTransmitSinkWithoutContext, theStream));

      asciiTraceHelper.HookDefaultEnqueueSinkWithoutContext<LrWpanMac> (device->GetMac (), "MacTxEnqueue", theStream);
      asciiTraceHelper.HookDefaultDequeueSinkWithoutContext<LrWpanMac> (device->GetMac (), "MacTxDequeue", theStream);
      asciiTraceHelper.HookDefaultDropSinkWithoutContext<LrWpanMac> (device->GetMac (), "MacTxDrop", theStream);

      return;
    }

  //
  // If we are provided an OutputStreamWrapper, we are expected to use it, and
  // to provide a context.  We are free to come up with our own context if we
  // want, and use the AsciiTraceHelper Hook*WithContext functions, but for
  // compatibility and simplicity, we just use Config::Connect and let it deal
  // with the context.
  //
  // Note that we are going to use the default trace sinks provided by the
  // ascii trace helper.  There is actually no AsciiTraceHelper in sight here,
  // but the default trace sinks are actually publicly available static
  // functions that are always there waiting for just such a case.
  //


  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::LrWpanNetDevice/Mac/MacRx";
  device->GetMac ()->TraceConnect ("MacRx", oss.str (), MakeBoundCallback (&AsciiTraceHelper::DefaultReceiveSinkWithContext, stream));

  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::LrWpanNetDevice/Mac/MacTx";
  device->GetMac ()->TraceConnect ("MacTx", oss.str (), MakeBoundCallback (&AsciiLrWpanMacTransmitSinkWithContext, stream));

  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::LrWpanNetDevice/Mac/MacTxEnqueue";
  device->GetMac ()->TraceConnect ("MacTxEnqueue", oss.str (), MakeBoundCallback (&AsciiTraceHelper::DefaultEnqueueSinkWithContext, stream));

  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::LrWpanNetDevice/Mac/MacTxDequeue";
  device->GetMac ()->TraceConnect ("MacTxDequeue", oss.str (), MakeBoundCallback (&AsciiTraceHelper::DefaultDequeueSinkWithContext, stream));

  oss.str ("");
  oss << "/NodeList/" << nodeid << "/DeviceList/" << deviceid << "/$ns3::LrWpanNetDevice/Mac/MacTxDrop";
  device->GetMac ()->TraceConnect ("MacTxDrop", oss.str (), MakeBoundCallback (&AsciiTraceHelper::DefaultDropSinkWithContext, stream));

}

} // namespace ns3

