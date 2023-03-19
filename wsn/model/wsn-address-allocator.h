#ifndef WSN_ADDRESS_ALLOCATOR_H
#define WSN_ADDRESS_ALLOCATOR_H
#include <stdint.h>
#include <cmath>
#include <unordered_map>
#include "ns3/type-id.h"
#include "ns3/log.h"
#include "ns3/singleton.h"

namespace ns3
{

class WsnAddressAllocator : public Singleton<WsnAddressAllocator>{
public:
    static TypeId GetTypeId (void);
 
    ~WsnAddressAllocator();

    void SetWsnAddressAllocator(uint16_t n, uint16_t r, uint8_t d);
    
    uint16_t AllocateAddress(uint8_t depth, bool node_type, uint16_t Aparent);
    
    uint16_t GetMaxAddress() const;
    
    uint16_t GetMinAddress() const;
    
    void Reset();

private:

    friend class Singleton<WsnAddressAllocator>;

    uint16_t m_n;           // 每个父设备拥有的孩子设备最大数 m_c
    uint16_t m_r;           // 每个父节点拥有的路由器的最大数目 m_r
    uint8_t m_d;            // 网络树的最大深度 m_l
    uint16_t m_baseAddr;    // 该节点的地址起始值
    uint16_t m_maxAddr;     // 可分配的最大地址
    
    std::unordered_map<uint8_t,uint16_t> m_node;
    
    std::unordered_map<uint8_t,uint16_t> m_route;


    WsnAddressAllocator(){};

    WsnAddressAllocator(const WsnAddressAllocator& rhs){};

    uint16_t CalcCSkip(uint8_t depth) const;

    uint16_t CalcMaxAddr() const;
};

}
#endif