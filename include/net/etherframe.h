#ifndef __GUYOS__NET__ETHERFRAME_H
#define __GUYOS__NET__ETHERFRAME_H

#include <common/types.h>
#include <drivers/amd_am79c973.h>
#include <memory.h>

/*

Ethernet implementation
dstMAC_BE -> Destination MAC address in big endian encoding
srcMAC_BE -> source MAC address in big endian encoding
etherType_BE -> type of the etherframe, in big endian encoding.
0x800 -> send the data to the ipv4 handler
0x806 -> send the frame to the ARP handler
*/




namespace guyos
{
    namespace net
    {
        struct EtherFrameHeader
        {
            common::uint64_t dstMAC_BE : 48;// only 6 bytes not 8... 15/5/21
            common::uint64_t srcMAC_BE : 48;//only 6 bytes not 8... 15/5/21
            common::uint16_t etherType_BE;
        } __attribute__((packed));
        typedef common::uint32_t EtherFrameFooter;

        class EtherFrameProvider;

        //Etherframe handler, this is the base class to be derived from.
        class EtherFrameHandler
        {
        protected:
            EtherFrameProvider* backend;
            common::uint16_t etherType_BE;

        public:
            EtherFrameHandler(EtherFrameProvider* backend, common::uint16_t etherType);// constructor
            ~EtherFrameHandler();//destructor

            virtual bool OnEtherFrameReceived(common::uint8_t* etherframePayload, common::uint32_t size);
            void Send(common::uint64_t dstMAC_BE, common::uint8_t* etherframePayload, common::uint32_t size);
            common::uint32_t GetIPAddress();
        };
        //Etherframe Provider
        class EtherFrameProvider : public guyos::drivers::RawDataHandler
        {
        friend class EtherFrameHandler;
        protected:
            EtherFrameHandler* handlers[65535];
        public:
            EtherFrameProvider(drivers::amd_am79c973* backend);
            ~EtherFrameProvider();

            bool OnRawDataReceived(common::uint8_t* buffer, common::uint32_t size);
            void Send(common::uint64_t dstMAC_BE, common::uint16_t etherType_BE, common::uint8_t* buffer, common::uint32_t size);
            common::uint64_t GetMACAddress();
            common::uint32_t GetIPAddress();
        
        };
    }
}




#endif