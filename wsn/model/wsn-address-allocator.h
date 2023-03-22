#ifndef WSN_ADDRESS_ALLOCATOR_H
#define WSN_ADDRESS_ALLOCATOR_H
#include <stdint.h>
#include <iostream>
#include <sstream>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <unordered_set>
#include <iomanip>
#include <unordered_map>

#include "ns3/double.h"
#include "ns3/random-variable-stream.h"
#include "ns3/type-id.h"
#include "ns3/log.h"
#include "ns3/singleton.h"

namespace ns3
{

class WsnAddressAllocator : public Singleton<WsnAddressAllocator>{
public:
    class EUI64Converter {
        public:
            std::string FormatOutPut(uint64_t value, uint8_t offset) const {
                std::stringstream sst;
                sst << std::hex << std::setfill('0');
                for (int i = offset; i >= 0; i--) {
                    sst << std::setw(2) << ((value >> (i * 8)) & 0xFF);
                    if (i > 0) {
                        sst << ":";
                    }
                }
                return sst.str();
            }

            std::string toEUI64(const std::string& mac) const {
                uint64_t value = 0;
                std::stringstream ss(mac);
                for (int i = 0; i < 6; i++) {
                    unsigned int byte;
                    ss >> std::hex >> byte;
                    value = (value << 8) | byte;
                    if (ss.peek() == ':') {
                        ss.ignore();
                    }
                }
                
                value = ((value & 0xFFFFFF000000ULL)<<16) | ((value & 0xFFFFFFULL)) | ((uint64_t)0xFFFE << 24); // 插入0xFFFE
                value ^= (1ULL << 57); // 取反U位
                return FormatOutPut(value,7);
            }
            

            std::string toMAC48(const std::string& eui64) const {
                uint64_t value = 0;
                std::stringstream ss(eui64);
                for (int i = 0; i < 8; i++) {
                    unsigned int byte;
                    ss >> std::hex >> byte;
                    value = (value << 8) | byte;
                    if (ss.peek() == ':') {
                        ss.ignore();
                    }
                }

                value ^= (1ULL << 57);
                value = (value & 0xFFFFFFULL) | ((value & 0xFFFFFF0000000000ULL) >> 16);
                return FormatOutPut(value,5);
            }
    };

    static TypeId GetTypeId (void);
 
    ~WsnAddressAllocator();

    void SetWsnAddressAllocator(uint16_t n, uint16_t r, uint8_t d);
    
    uint16_t AllocateNwkAddress(uint8_t depth, bool node_type, uint16_t Aparent);
    
    std::string AllocateRandMac48Address();

    std::string AnalysisMac48AddresstoEUI64 (const std::string& mac48) const;

    std::string AnalysisEUI64ToMac48Address (const std::string& eui) const;

    std::string FormatOutPut(uint64_t value, uint8_t offset) const;

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

    std::unordered_set<std::string> m_macSet;

    WsnAddressAllocator(){};

    WsnAddressAllocator(const WsnAddressAllocator& rhs){};

    uint16_t CalcCSkip(uint8_t depth) const;

    uint16_t CalcMaxAddr() const;
};

}
#endif
