#include <iostream>
#include <sstream>
#include <iomanip>


    class EUI64Converter {
        public:
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

                std::stringstream sst;
                sst << std::hex << std::setfill('0');
                for (int i = 7; i >= 0; i--) {
                    sst << std::setw(2) << ((value >> (i * 8)) & 0xFF);
                    if (i > 0) {
                        sst << ":";
                    }
                }
                return sst.str();
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
                std::stringstream sst;
                sst << std::hex << std::setfill('0');
                for (int i = 5; i >= 0; i--) {
                    sst << std::setw(2) << ((value >> (i * 8)) & 0xFF);
                    if (i > 0) {
                        sst << ":";
                    }
                }
                return sst.str();
            }
    };

//02:11:22:ff:fe:33:44:55
int main() {
    std::string mac = "00:AA:00:3F:2A:1C";
    EUI64Converter converter;
    std::string t = converter.toEUI64(mac);
    std::cout << "EUI64 address: " << t << std::endl;
    std::string t2 = converter.toMAC48(t);
    std::cout << "MAC48 address: " << t2 << std::endl;
}