#ifndef __GUYOS__NET__IPV4_H
#define __GUYOS__NET__IPV4_H

#include <common/types.h>
#include <net/etherframe.h>
#include <net/arp.h>

/*
Handler for the ipv4 ethernet frame.

*/

namespace guyos
{
    namespace net
    {

        struct IPV4Message
        {
            common::uint8_t headerLength : 4;
            common::uint8_t version : 4;
            common::uint8_t tos;
            common::uint16_t totalLength;

            common::uint16_t ident;
            common::uint16_t flagsAndOffset;

            common::uint8_t timeToLive;
            common::uint8_t protocol;
            common::uint16_t checksum;

            common::uint32_t srcIP;
            common::uint32_t dstIP;
        } __attribute__((packed));



        class IPProvider;

        class IPHandler
        {
        protected:
            IPProvider* backend;
            common::uint8_t ip_protocol;

        public:
            IPHandler(IPProvider* backend, common::uint8_t protocol);//constructor
            ~IPHandler();//destructor

            virtual bool OnIPReceived(common::uint32_t srcIP_BE, common::uint32_t dstIP_BE,
                                            common::uint8_t* IPPayload, common::uint32_t size);// upon receiving an IP packet
            void Send(common::uint32_t dstIP_BE, common::uint8_t* IPPayload, common::uint32_t size);// handler send
        };


        class IPProvider : public EtherFrameHandler
        {
        friend class IPHandler;
        protected:
            IPHandler* handlers[255];// handlers
            ARP* arp;//ARP
            common::uint32_t gatewayIP;// default gateway ip address
            common::uint32_t subnetMask;// subnet mask

        public:
            IPProvider(EtherFrameProvider* backend, 
                                     ARP* arp,
                                     common::uint32_t gatewayIP, common::uint32_t subnetMask);// provider constructor
            ~IPProvider();// provider destructor

            bool OnEtherFrameReceived(common::uint8_t* etherframePayload, common::uint32_t size);// ethernet frame backwords

            void Send(common::uint32_t dstIP_BE, common::uint8_t protocol, common::uint8_t* buffer, common::uint32_t size);//Provider Send

            static common::uint16_t Checksum(common::uint16_t* data, common::uint32_t lengthInBytes);// calculates the checksum
        };        
    }
}




#endif