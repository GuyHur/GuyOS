#include <net/udp.h>

using namespace guyos;
using namespace guyos::common;
using namespace guyos::net;

UDPHandler::UDPHandler()
{
}

UDPHandler::~UDPHandler()
{
}

void UDPHandler::HandleUDPMessage(UDPSocket* socket, uint8_t* data, uint16_t size)
{
    
}
UDPSocket::UDPSocket(UDPProvider* backend)
{
    this->backend = backend;
    handler = 0;
    listening = false;
}

UDPSocket::~UDPSocket()
{
}

void UDPSocket::HandleUDPMessage(uint8_t* data, uint16_t size)
{
    if(handler != 0)
        handler->HandleUDPMessage(this, data, size);
}

void UDPSocket::Send(uint8_t* data, uint16_t size)
{
    backend->Send(this, data, size);
}

void UDPSocket::Disconnect()
{
    backend->Disconnect(this);
}





UDPProvider::UDPProvider(IPProvider* backend)
: IPHandler(backend, 0x11)
{
    for(int i = 0; i < 65535; i++)
        sockets[i] = 0;
    numSockets = 0;
    freePort = 1024;
}

UDPProvider::~UDPProvider()
{
}

bool UDPProvider::OnIPReceived(uint32_t srcIP_BE, uint32_t dstIP_BE,
                                        uint8_t* IPPayload, uint32_t size)
{
    if(size < sizeof(UDPHeader))
        return false;

    UDPHeader* msg = (UDPHeader*)IPPayload;
    uint16_t localPort = msg->dstPort;
    uint16_t remotePort = msg->srcPort;


    UDPSocket* socket = 0;
    for(uint16_t i = 0; i < numSockets && socket == 0; i++)
    {
        if( sockets[i]->localPort == msg->dstPort
        &&  sockets[i]->localIP == dstIP_BE
        &&  sockets[i]->listening)
        {
            socket = sockets[i];
            socket->listening = false;
            socket->remotePort = msg->srcPort;
            socket->remoteIP = srcIP_BE;
        }

        else if( sockets[i]->localPort == msg->dstPort
        &&  sockets[i]->localIP == dstIP_BE
        &&  sockets[i]->remotePort == msg->srcPort
        &&  sockets[i]->remoteIP == srcIP_BE)
            socket = sockets[i];
    }

    if(socket != 0)
        socket->HandleUDPMessage(IPPayload + sizeof(UDPHeader),
                                                  size - sizeof(UDPHeader));

    return false;
}


UDPSocket* UDPProvider::Connect(uint32_t ip, uint16_t port)
{
    UDPSocket* socket = (UDPSocket*)MemoryManager::activeMemoryManager->malloc(sizeof(UDPSocket));

    if(socket != 0)
    {
        new (socket) UDPSocket(this);

        socket -> remotePort = port;
        socket -> remoteIP = ip;
        socket -> localPort = freePort++;
        socket -> localIP = backend->GetIPAddress();

        socket -> remotePort = ((socket -> remotePort & 0xFF00)>>8) | ((socket -> remotePort & 0x00FF) << 8);
        socket -> localPort = ((socket -> localPort & 0xFF00)>>8) | ((socket -> localPort & 0x00FF) << 8);

        sockets[numSockets++] = socket;
    }

    return socket;
}



UDPSocket* UDPProvider::Listen(uint16_t port)
{
    UDPSocket* socket = (UDPSocket*)MemoryManager::activeMemoryManager->malloc(sizeof(UDPSocket));

    if(socket != 0)
    {
        new (socket) UDPSocket(this);

        socket -> listening = true;
        socket -> localPort = port;
        socket -> localIP = backend->GetIPAddress();

        socket -> localPort = ((socket -> localPort & 0xFF00)>>8) | ((socket -> localPort & 0x00FF) << 8);

        sockets[numSockets++] = socket;
    }

    return socket;
}


void UDPProvider::Disconnect(UDPSocket* socket)
{
    for(uint16_t i = 0; i < numSockets && socket == 0; i++)
        if(sockets[i] == socket)
        {
            sockets[i] = sockets[--numSockets];
            MemoryManager::activeMemoryManager->free(socket);
            break;
        }
}

void UDPProvider::Send(UDPSocket* socket, uint8_t* data, uint16_t size)
{
    uint16_t totalLength = size + sizeof(UDPHeader);
    uint8_t* buffer = (uint8_t*)MemoryManager::activeMemoryManager->malloc(totalLength);
    uint8_t* buffer2 = buffer + sizeof(UDPHeader);

    UDPHeader* msg = (UDPHeader*)buffer;

    msg->srcPort = socket->localPort;
    msg->dstPort = socket->remotePort;
    msg->length = ((totalLength & 0x00FF) << 8) | ((totalLength & 0xFF00) >> 8);

    for(int i = 0; i < size; i++)
        buffer2[i] = data[i];

    msg -> checksum = 0;
    IPHandler::Send(socket->remoteIP, buffer, totalLength);

    MemoryManager::activeMemoryManager->free(buffer);
}

void UDPProvider::Bind(UDPSocket* socket, UDPHandler* handler)
{
    socket->handler = handler;
}