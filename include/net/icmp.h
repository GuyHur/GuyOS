
#ifndef __GUYOS__NET__ICMP_H
#define __GUYOS__NET__ICMP_H

#include <common/types.h>
#include <net/ipv4.h>


namespace guyos
{
    namespace net
    {


        struct ICMPMessage
        {
            common::uint8_t type;
            common::uint8_t code;

            common::uint16_t checksum;
            common::uint32_t data;

        }__attribute__((packed));

        class ICMP : IPHandler
        {
        public:
            ICMP(IPProvider* backend);
            ~ICMP();

            bool OnIPReceived(common::uint32_t srcIP_BE, common::uint32_t dstIP_BE, common::uint8_t* IPPayload, common::uint32_t size);
            void RequestEchoReply(common::uint32_t ip_be);
        };


    }
}


#endif