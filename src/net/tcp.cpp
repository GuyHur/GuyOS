#include <net/tcp.h>

using namespace guyos;
using namespace guyos::common;
using namespace guyos::net;



TCPHandler::TCPHandler()
{
}

TCPHandler::~TCPHandler()
{
}

bool TCPHandler::HandleTCPMessage(TCPSocket* socket, uint8_t* data, uint16_t size)
{
    return true;
}





TCPSocket::TCPSocket(TCPProvider* backend)
{
    /*
    Initial state of the Socket
    */
    this->backend = backend;
    handler = 0;
    state = CLOSED;
}

TCPSocket::~TCPSocket()
{
}

bool TCPSocket::HandleTCPMessage(uint8_t* data, uint16_t size)
{
    if(handler != 0)// if handler does not exist
        return handler->HandleTCPMessage(this, data, size);
    return false;
}

void TCPSocket::Send(uint8_t* data, uint16_t size)
{
    while(state != ESTABLISHED)
    {
    }
    backend->Send(this, data, size, PSH|ACK);// call the base class Send
}

void TCPSocket::Disconnect()
{
    backend->Disconnect(this);// call the base class Disconnect
}





TCPProvider::TCPProvider(IPProvider* backend)
: IPHandler(backend, 0x06)
{
    for(int i = 0; i < 65535; i++)
        sockets[i] = 0;
    numSockets = 0;
    freePort = 1024;
}

TCPProvider::~TCPProvider()
{
}


uint32_t bigEndian32(uint32_t x)
{
    return ((x & 0xFF000000) >> 24)
         | ((x & 0x00FF0000) >> 8)
         | ((x & 0x0000FF00) << 8)
         | ((x & 0x000000FF) << 24);
}


bool TCPProvider::OnIPReceived(uint32_t srcIP_BE, uint32_t dstIP_BE,
                                        uint8_t* IPPayload, uint32_t size)
{


    if(size < 20)
        return false;

    TCPHeader* msg = (TCPHeader*)IPPayload;
    uint16_t localPort = msg->dstPort;
    uint16_t remotePort = msg->srcPort;


    TCPSocket* socket = 0;
    for(uint16_t i = 0; i < numSockets && socket == 0; i++)
    {
        if( sockets[i]->localPort == msg->dstPort
        &&  sockets[i]->localIP == dstIP_BE
        &&  sockets[i]->state == LISTEN
        && (((msg -> flags) & (SYN | ACK)) == SYN))
            socket = sockets[i];
        else if( sockets[i]->localPort == msg->dstPort
        &&  sockets[i]->localIP == dstIP_BE
        &&  sockets[i]->remotePort == msg->srcPort
        &&  sockets[i]->remoteIP == srcIP_BE)
            socket = sockets[i];
    }



    bool reset = false;

    if(socket != 0 && msg->flags & RST)
        socket->state = CLOSED;


    if(socket != 0 && socket->state != CLOSED)
    {
        switch((msg -> flags) & (SYN | ACK | FIN))
        {
            case SYN:
                if(socket -> state == LISTEN)
                {
                    socket->state = SYN_RECEIVED;
                    socket->remotePort = msg->srcPort;
                    socket->remoteIP = srcIP_BE;
                    socket->acknowledgementNumber = bigEndian32( msg->sequenceNumber ) + 1;
                    socket->sequenceNumber = 0xbeefcafe;
                    Send(socket, 0,0, SYN|ACK);
                    socket->sequenceNumber++;
                }
                else
                    reset = true;
                break;


            case SYN | ACK:
                if(socket->state == SYN_SENT)
                {
                    socket->state = ESTABLISHED;
                    socket->acknowledgementNumber = bigEndian32( msg->sequenceNumber ) + 1;
                    socket->sequenceNumber++;
                    Send(socket, 0,0, ACK);
                }
                else
                    reset = true;
                break;


            case SYN | FIN:
            case SYN | FIN | ACK:
                reset = true;
                break;


            case FIN:
            case FIN|ACK:
                if(socket->state == ESTABLISHED)
                {
                    socket->state = CLOSE_WAIT;
                    socket->acknowledgementNumber++;
                    Send(socket, 0,0, ACK);
                    Send(socket, 0,0, FIN|ACK);
                }
                else if(socket->state == CLOSE_WAIT)
                {
                    socket->state = CLOSED;
                }
                else if(socket->state == FIN_WAIT1
                    || socket->state == FIN_WAIT2)
                {
                    socket->state = CLOSED;
                    socket->acknowledgementNumber++;
                    Send(socket, 0,0, ACK);
                }
                else
                    reset = true;
                break;


            case ACK:
                if(socket->state == SYN_RECEIVED)
                {
                    socket->state = ESTABLISHED;
                    return false;
                }
                else if(socket->state == FIN_WAIT1)
                {
                    socket->state = FIN_WAIT2;
                    return false;
                }
                else if(socket->state == CLOSE_WAIT)
                {
                    socket->state = CLOSED;
                    break;
                }

                if(msg->flags == ACK)
                    break;

                // no break, because of piggybacking

            default:

                if(bigEndian32(msg->sequenceNumber) == socket->acknowledgementNumber)
                {
                    reset = !(socket->HandleTCPMessage(IPPayload + msg->headerSize32*4,
                                                                              size - msg->headerSize32*4));
                    if(!reset)
                    {
                        int x = 0;
                        for(int i = msg->headerSize32*4; i < size; i++)
                            if(IPPayload[i] != 0)
                                x = i;
                        socket->acknowledgementNumber += x - msg->headerSize32*4 + 1;
                        Send(socket, 0,0, ACK);
                    }
                }
                else
                {
                    // data in wrong order
                    reset = true;
                }

        }
    }


    if(reset)
    {
        if(socket != 0)
        {
            Send(socket, 0,0, RST);
        }
        else
        {
            TCPSocket socket(this);
            socket.remotePort = msg->srcPort;
            socket.remoteIP = srcIP_BE;
            socket.localPort = msg->dstPort;
            socket.localIP = dstIP_BE;
            socket.sequenceNumber = bigEndian32(msg->acknowledgementNumber);
            socket.acknowledgementNumber = bigEndian32(msg->sequenceNumber) + 1;
            Send(&socket, 0,0, RST);
        }
    }


    if(socket != 0 && socket->state == CLOSED)
        for(uint16_t i = 0; i < numSockets && socket == 0; i++)
            if(sockets[i] == socket)
            {
                sockets[i] = sockets[--numSockets];
                MemoryManager::activeMemoryManager->free(socket);
                break;
            }

    return false;
}


// ------------------------------------------------------------------------------------------



void TCPProvider::Send(TCPSocket* socket, uint8_t* data, uint16_t size, uint16_t flags)
{
    uint16_t totalLength = size + sizeof(TCPHeader);// length of the TCP header + the size of the packet
    uint16_t lengthInclPHdr = totalLength + sizeof(TCPPseudoHeader);//totalLength + pseudo header(src ip, port length and protocol)

    uint8_t* buffer = (uint8_t*)MemoryManager::activeMemoryManager->malloc(lengthInclPHdr);// allocated memory for the packet
    TCPPseudoHeader* phdr = (TCPPseudoHeader*)buffer;//casting to pseudo header
    TCPHeader* msg = (TCPHeader*)(buffer + sizeof(TCPPseudoHeader));// actual msg
    uint8_t* buffer2 = buffer + sizeof(TCPHeader)
                              + sizeof(TCPPseudoHeader);
    msg->headerSize32 = sizeof(TCPHeader)/4;
    msg->srcPort = socket->localPort;
    msg->dstPort = socket->remotePort;
    msg->acknowledgementNumber = bigEndian32( socket->acknowledgementNumber );
    msg->sequenceNumber = bigEndian32( socket->sequenceNumber );
    msg->reserved = 0;
    msg->flags = flags;
    msg->windowSize = 0xFFFF;
    msg->urgentPtr = 0;

    msg->options = ((flags & SYN) != 0) ? 0xB4050402 : 0;

    socket->sequenceNumber += size;

    for(int i = 0; i < size; i++)
        buffer2[i] = data[i];

    phdr->srcIP = socket->localIP;// local ip
    phdr->dstIP = socket->remoteIP;// remote ip
    phdr->protocol = 0x0600;// tcp handler id
    phdr->totalLength = ((totalLength & 0x00FF) << 8) | ((totalLength & 0xFF00) >> 8);// total length of the packet    

    msg->checksum = 0;// checksum
    msg->checksum = IPProvider::Checksum((uint16_t*)buffer, lengthInclPHdr);// calc checksum

    IPHandler::Send(socket->remoteIP, (uint8_t*)msg, totalLength);//send back to IP

    MemoryManager::activeMemoryManager->free(buffer);// frees the memory allocated before.
}



TCPSocket* TCPProvider::Connect(uint32_t ip, uint16_t port)
{
    TCPSocket* socket = (TCPSocket*)MemoryManager::activeMemoryManager->malloc(sizeof(TCPSocket));

    if(socket != 0)
    {
        new (socket) TCPSocket(this);

        socket -> remotePort = port;
        socket -> remoteIP = ip;
        socket -> localPort = freePort++;
        socket -> localIP = backend->GetIPAddress();

        socket -> remotePort = ((socket -> remotePort & 0xFF00)>>8) | ((socket -> remotePort & 0x00FF) << 8);
        socket -> localPort = ((socket -> localPort & 0xFF00)>>8) | ((socket -> localPort & 0x00FF) << 8);

        sockets[numSockets++] = socket;
        socket -> state = SYN_SENT;

        socket -> sequenceNumber = 0xbeefcafe;

        Send(socket, 0,0, SYN);
    }

    return socket;
}



void TCPProvider::Disconnect(TCPSocket* socket)
{
    socket->state = FIN_WAIT1;
    Send(socket, 0,0, FIN + ACK);
    socket->sequenceNumber++;
}


TCPSocket* TCPProvider::Listen(uint16_t port)
{
    TCPSocket* socket = (TCPSocket*)MemoryManager::activeMemoryManager->malloc(sizeof(TCPSocket));

    if(socket != 0)// if socket exists
    {
        new (socket) TCPSocket(this);

        socket -> state = LISTEN;
        socket -> localIP = backend->GetIPAddress();
        socket -> localPort = ((port & 0xFF00)>>8) | ((port & 0x00FF) << 8);

        sockets[numSockets++] = socket;
    }

    return socket;
}
void TCPProvider::Bind(TCPSocket* socket, TCPHandler* handler)
{
    socket->handler = handler;
}