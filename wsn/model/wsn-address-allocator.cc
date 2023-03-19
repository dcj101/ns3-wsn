#include "wsn-address-allocator.h"
#include <iostream>
namespace ns3
{

NS_LOG_COMPONENT_DEFINE ("WsnAddressAllocator");

TypeId 
WsnAddressAllocator::GetTypeId (void)
{
    static TypeId tid = TypeId ("ns3::WsnAddressAllocator");
    return tid;
}
// c r l
WsnAddressAllocator::~WsnAddressAllocator()
{
    m_node.clear();
    m_route.clear();
}

void 
WsnAddressAllocator::SetWsnAddressAllocator(uint16_t n, uint16_t r, uint8_t d) 
{
    m_n = n;
    m_r = r;
    m_d = d;
    m_baseAddr = 1;
    m_maxAddr = CalcMaxAddr();
    m_node.clear();
    m_route.clear();
}
// 0 终端节点， 1路由器节点
uint16_t 
WsnAddressAllocator::AllocateAddress(uint8_t depth, bool node_type, uint16_t Aparent) 
{
    if (depth > m_d) 
    {
        NS_LOG_ERROR("Depth out of range");
        return 0;
    }
    uint16_t cSkip = CalcCSkip(depth);    
    uint16_t address = 0;
    if(node_type == 0)
    {
        if(!m_node.count(depth)) m_node[depth] = 0;
        address = Aparent + cSkip * m_r + (++m_node[depth]);
    }
    else 
    {
        if(!m_route.count(depth)) m_route[depth] = 0;
        address = Aparent + 1 + cSkip*(++m_route[depth]-1); 
    }
    if (address > m_maxAddr) 
    {
        address = m_baseAddr + (address - m_maxAddr) - 1;
    }
    return address;
}

uint16_t 
WsnAddressAllocator::GetMaxAddress() const 
{
    return m_maxAddr;
}

uint16_t 
WsnAddressAllocator::GetMinAddress() const 
{
    return 1;
}

void 
WsnAddressAllocator::Reset() 
{
    m_node.clear();
    m_route.clear();
}

uint16_t 
WsnAddressAllocator::CalcCSkip(uint8_t depth) const 
{
    if(m_r == 1) 
    {
        return 1 + m_n * (m_d - depth - 1);
    }
    else 
    {
        return (1 + m_n - m_r - m_n * std::pow(m_r,m_d - depth - 1)) / (1 - m_r);
    }
}

uint16_t 
WsnAddressAllocator::CalcMaxAddr() const 
{
    return std::pow(m_n, m_d) - 1;
}

}