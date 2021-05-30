#include <net/ipv4.h>

using namespace guyos;
using namespace guyos::common;
using namespace guyos::net;

        
        
            
IPHandler::IPHandler(IPProvider* backend, uint8_t protocol)
{
    this->backend = backend;//set backend
    this->ip_protocol = protocol;// set protocol
    backend->handlers[protocol] = this;
}

IPHandler::~IPHandler()
{
    if(backend->handlers[ip_protocol] == this)// if this object is an handler in the backend, delete this handler from the backend's handlers
        backend->handlers[ip_protocol] = 0;
}
            
bool IPHandler::OnIPReceived(uint32_t srcIP_BE, uint32_t dstIP_BE, uint8_t* IPPayload, uint32_t size)
{
    return false;// receiving
}

void IPHandler::Send(uint32_t dstIP_BE, uint8_t* IPPayload, uint32_t size)
{
    backend->Send(dstIP_BE, ip_protocol, IPPayload, size);//Send back from the base constructor
}


     

IPProvider::IPProvider(EtherFrameProvider* backend, ARP* arp, uint32_t gatewayIP, uint32_t subnetMask)
: EtherFrameHandler(backend, 0x800)
{
    for(int i = 0; i < 255; i++)
        handlers[i] = 0;
    this->arp = arp;
    this->gatewayIP = gatewayIP;
    this->subnetMask = subnetMask;
}

IPProvider::~IPProvider()
{
}
            
bool IPProvider::OnEtherFrameReceived(uint8_t* etherframePayload, uint32_t size)
{
    if(size < sizeof(IPV4Message))
        return false;
    
    IPV4Message* ipmessage = (IPV4Message*)etherframePayload;
    bool sendBack = false;
    
    if(ipmessage->dstIP == backend->GetIPAddress())// if the message is for my ip address
    {
        int length = ipmessage->totalLength;
        if(length > size)
            length = size;// heartbleed fix
        
        if(handlers[ipmessage->protocol] != 0)//check protocol handler
            sendBack = handlers[ipmessage->protocol]->OnIPReceived(
                ipmessage->srcIP, ipmessage->dstIP, 
                etherframePayload + 4*ipmessage->headerLength, length - 4*ipmessage->headerLength);
        
    }
    
    if(sendBack)
    {
        uint32_t temp = ipmessage->dstIP;
        ipmessage->dstIP = ipmessage->srcIP;
        ipmessage->srcIP = temp;
        
        ipmessage->timeToLive = 0x40;
        ipmessage->checksum = 0;
        ipmessage->checksum = Checksum((uint16_t*)ipmessage, 4*ipmessage->headerLength);
    }
    
    return sendBack;
}


void IPProvider::Send(uint32_t dstIP_BE, uint8_t protocol, uint8_t* data, uint32_t size)
{
    
    uint8_t* buffer = (uint8_t*)MemoryManager::activeMemoryManager->malloc(sizeof(IPV4Message) + size);// mem alloc
    IPV4Message *message = (IPV4Message*)buffer;// cast to IPV4 msg
    
    message->version = 4;// set version to 4
    message->headerLength = sizeof(IPV4Message)/4;//header len
    message->tos = 0;//type of service
    message->totalLength = size + sizeof(IPV4Message);// total length
    message->totalLength = ((message->totalLength & 0xFF00) >> 8)
                         | ((message->totalLength & 0x00FF) << 8);//big endian
    message->ident = 0x0100;//identification
    message->flagsAndOffset = 0x0040;//flags, no fragmentation
    message->timeToLive = 0x40;// ttl
    message->protocol = protocol;// protocol 
    
    message->dstIP = dstIP_BE;// destination ip
    message->srcIP = backend->GetIPAddress();// my ip
    
    message->checksum = 0;// checksum initialization
    message->checksum = Checksum((uint16_t*)message, sizeof(IPV4Message));//calculate checksum
    
    uint8_t* databuffer = buffer + sizeof(IPV4Message);
    for(int i = 0; i < size; i++)++
        databuffer[i] = data[i];
    
    uint32_t route = dstIP_BE;
    if((dstIP_BE & subnetMask) != (message->srcIP & subnetMask))
        route = gatewayIP;
    

    backend->Send(arp->Resolve(route), this->etherType_BE, buffer, sizeof(IPV4Message) + size);
    
    
    MemoryManager::activeMemoryManager->free(buffer);
}


uint16_t IPProvider::Checksum(uint16_t* data, uint32_t lengthInBytes)
{
    /*
    Calculates the checksum
    */
    uint32_t temp = 0;

    for(int i = 0; i < lengthInBytes/2; i++)
        temp += ((data[i] & 0xFF00) >> 8) | ((data[i] & 0x00FF) << 8);

    if(lengthInBytes % 2)
        temp += ((uint16_t)((char*)data)[lengthInBytes-1]) << 8;
    
    while(temp & 0xFFFF0000)
        temp = (temp & 0xFFFF) + (temp >> 16);
    
    return ((~temp & 0xFF00) >> 8) | ((~temp & 0x00FF) << 8);
}

