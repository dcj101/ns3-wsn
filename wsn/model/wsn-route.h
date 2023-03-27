#ifndef WSN_ROUTE_H
#define WSN_ROUTE_H

#include "ns3/type-id.h"
#include "wsn-nwk-short-address.h"
#include <map>
#include <iostream>


namespace ns3
{

// 静态路由类
class StaticRoute {
public:
    static TypeId GetType(void);
    /**
     * 构造函数
     * @param dest 目的地址
     * @param nextHop 下一跳地址
     */ 
    StaticRoute(NwkShortAddress dest, NwkShortAddress nextHop);

    StaticRoute():m_dest(),m_nextHop() {}

    ~StaticRoute(){}
    /**
     * 获取目的地址
     */
    NwkShortAddress GetDestination() const;

    /**
     * 获取下一跳地址
     */
    NwkShortAddress GetNextHop() const;

private:
    NwkShortAddress m_dest;      // 目的地址
    NwkShortAddress m_nextHop;   // 下一跳地址
    bool isValue;               // 路由是否有效

};


// 路由表类
class RoutingTable {
public:
    static TypeId GetType(void);
    /**
     * 构造函数
     */
    RoutingTable();
    ~RoutingTable(){}
    /**
     * 添加一条静态路由
     * @param route 要添加的路由
     */
    void AddRoute(const StaticRoute& route);

    /**
     * 删除一条静态路由
     * @param dest 目的地址
     */
    void RemoveRoute(NwkShortAddress dest);

    /**
     * 查找给定目的地址的下一跳地址
     * @param dest 目的地址
     * @return 下一跳地址，若不存在则返回无效地址
     */
    NwkShortAddress Lookup(NwkShortAddress dest);

    /**
     * 打印当前路由表
     */
    void Print() const;

private:
    std::map<NwkShortAddress,NwkShortAddress> m_routes;  // 静态路由表
};

}

#endif

