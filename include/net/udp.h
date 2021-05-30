#ifndef __GUYOS__NET__UDP_H
#define __GUYOS__NET__UDP_H

#include <common/types.h>
#include <net/ipv4.h>
#include <memory.h>

namespace guyos
{
    namespace net
    {
        struct UDPHeader
        {
            common::uint16_t srcPort;
            common::uint16_t dstPort;
            common::uint16_t length;
            common::uint16_t checksum;
        }__attribute__((packed));



        class UDPSocket;
        class UDPProvider;



        class UDPHandler
        {
        public:
            UDPHandler();
            ~UDPHandler();
            virtual void HandleUDPMessage(UDPSocket* socket, common::uint8_t* data, common::uint16_t size);// Handles a UDP message
        };



        class UDPSocket
        {
        friend class UDPProvider;
        protected:
            common::uint16_t remotePort;
            common::uint32_t remoteIP;
            common::uint16_t localPort;
            common::uint32_t localIP;
            UDPProvider* backend;
            UDPHandler* handler;
            bool listening;
        public:
            UDPSocket(UDPProvider* backend);
            ~UDPSocket();
            virtual void HandleUDPMessage(common::uint8_t* data, common::uint16_t size);
            virtual void Send(common::uint8_t* data, common::uint16_t size);
            virtual void Disconnect();
        };


        class UDPProvider : IPHandler
        {
        protected:
            UDPSocket* sockets[65535];
            common::uint16_t numSockets;
            common::uint16_t freePort;

        public:
            UDPProvider(IPProvider* backend);
            ~UDPProvider();

            virtual bool OnIPReceived(common::uint32_t srcIP_BE, common::uint32_t dstIP_BE,
                                                    common::uint8_t* IPPayload, common::uint32_t size);

            virtual UDPSocket* Connect(common::uint32_t ip, common::uint16_t port);
            virtual UDPSocket* Listen(common::uint16_t port);
            virtual void Disconnect(UDPSocket* socket);
            virtual void Send(UDPSocket* socket, common::uint8_t* data, common::uint16_t size);

            virtual void Bind(UDPSocket* socket, UDPHandler* handler);
        };        
    }
}

#endif