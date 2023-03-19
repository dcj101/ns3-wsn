#ifndef WSN_NETWORK_H
#define WSN_NETWORK_H

namespace ns3
{

class WsnNwkProtocol : public Object
{
    public:

    static TypeId GetTypeId (void);
    
    NwkHeader BuildHeader();

    protected:
    
    virtual void NotifyNewAggregate (void);
    
    virtual void DoInitialize (void);
    
    virtual void DoDispose (void);

    private:

    

};  

}

#endif