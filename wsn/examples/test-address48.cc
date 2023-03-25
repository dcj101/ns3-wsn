#include "ns3/core-module.h"
#include "ns3/wsn-address-allocator.h"
#include "ns3/log.h"
#include "ns3/ptr.h"

using namespace ns3;
NS_LOG_COMPONENT_DEFINE ("testExample");


int main () {
    LogComponentEnable ("testExample", LogLevel (LOG_PREFIX_TIME | LOG_PREFIX_NODE | LOG_LEVEL_INFO));
    // 定义 ZigbeeAddressAllocator 对象

    WsnAddressAllocator *allocator = WsnAddressAllocator::Get ();
    allocator->SetWsnAddressAllocator(5,3,3);
    // 分配地址
    uint16_t addr1 = allocator->AllocateNwkAddress(0,1,0);
    uint16_t addr2 = allocator->AllocateNwkAddress(0,1,0);
    uint16_t addr3 = allocator->AllocateNwkAddress(0,1,0);
    uint16_t addr7 = allocator->AllocateNwkAddress(0,0,0);
    uint16_t addr8 = allocator->AllocateNwkAddress(0,0,0);

    // 输出分配的地址
    NS_LOG_INFO("Address 1: " << addr1);
    NS_LOG_INFO("Address 2: " << addr2);
    NS_LOG_INFO("Address 3: " << addr3);
    NS_LOG_INFO("Address 7: " << addr7);
    NS_LOG_INFO("Address 8: " << addr8);

    // 重置地址分配器
    // allocator->Reset();

    // 再次分配地址
    uint16_t addr4 = allocator->AllocateNwkAddress(1,1,addr1);
    uint16_t addr5 = allocator->AllocateNwkAddress(1,1,addr1);
    uint16_t addr6 = allocator->AllocateNwkAddress(1,1,addr1);
    uint16_t addr9 = allocator->AllocateNwkAddress(1,0,addr1);
    uint16_t addr10= allocator->AllocateNwkAddress(1,0,addr1);

    // 输出分配的地址
    NS_LOG_INFO("Address 4: " << addr4);
    NS_LOG_INFO("Address 5: " << addr5);
    NS_LOG_INFO("Address 6: " << addr6);
    NS_LOG_INFO("Address 9: " << addr9);
    NS_LOG_INFO("Address 10: " << addr10);

    return 0;
}
