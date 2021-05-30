#ifndef __GUYOS__NET__TCP_H
#define __GUYOS__NET__TCP_H

#include <common/types.h>
#include <net/ipv4.h>
#include <memory.h>

namespace guyos
{
    namespace net
    {
        
        
        enum TCPSocketState
        {
            CLOSED,
            LISTEN,
            SYN_SENT,
            SYN_RECEIVED,
            
            ESTABLISHED,
            
            FIN_WAIT1,
            FIN_WAIT2,
            CLOSING,
            TIME_WAIT,
            
            CLOSE_WAIT
            //LAST_ACK
        };
        
        enum TCPFlag
        {
            FIN = 1,
            SYN = 2,
            RST = 4,
            PSH = 8,
            ACK = 16,
            URG = 32,
            ECE = 64,
            CWR = 128,
            NS = 256
        };
        
        
        struct TCPHeader
        {
            common::uint16_t srcPort;
            common::uint16_t dstPort;
            common::uint32_t sequenceNumber;
            common::uint32_t acknowledgementNumber;
            
            common::uint8_t reserved : 4;
            common::uint8_t headerSize32 : 4;
            common::uint8_t flags;
            
            common::uint16_t windowSize;
            common::uint16_t checksum;
            common::uint16_t urgentPtr;
            
            common::uint32_t options;
        } __attribute__((packed));
       
      
        struct TCPPseudoHeader
        {
            common::uint32_t srcIP;
            common::uint32_t dstIP;
            common::uint16_t protocol;
            common::uint16_t totalLength;
        } __attribute__((packed));
      
      
        class TCPSocket;
        class TCPProvider;
        
        
        
        class TCPHandler
        {
        public:
            TCPHandler();
            ~TCPHandler();
            virtual bool HandleTCPMessage(TCPSocket* socket, common::uint8_t* data, common::uint16_t size);
        };
      
        
      
        class TCPSocket
        {
        friend class TCPProvider;
        protected:
            common::uint16_t remotePort;
            common::uint32_t remoteIP;
            common::uint16_t localPort;
            common::uint32_t localIP;
            common::uint32_t sequenceNumber;
            common::uint32_t acknowledgementNumber;

            TCPProvider* backend;
            TCPHandler* handler;
            
            TCPSocketState state;
        public:
            TCPSocket(TCPProvider* backend);
            ~TCPSocket();
            virtual bool HandleTCPMessage(common::uint8_t* data, common::uint16_t size);
            virtual void Send(common::uint8_t* data, common::uint16_t size);
            virtual void Disconnect();
        };
      
      
        class TCPProvider : IPHandler
        {
        protected:
            TCPSocket* sockets[65535];
            common::uint16_t numSockets;
            common::uint16_t freePort;
            
        public:
            TCPProvider(IPProvider* backend);
            ~TCPProvider();
            
            virtual bool OnIPReceived(common::uint32_t srcIP_BE, common::uint32_t dstIP_BE,
                                                    common::uint8_t* IPPayload, common::uint32_t size);

            virtual TCPSocket* Connect(common::uint32_t ip, common::uint16_t port);
            virtual void Disconnect(TCPSocket* socket);
            virtual void Send(TCPSocket* socket, common::uint8_t* data, common::uint16_t size,
                              common::uint16_t flags = 0);

            virtual TCPSocket* Listen(common::uint16_t port);
            virtual void Bind(TCPSocket* socket, TCPHandler* handler);
        };

    }
}


#endif