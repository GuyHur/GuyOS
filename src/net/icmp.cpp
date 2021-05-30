#include <net/icmp.h>

using namespace guyos;
using namespace guyos::common;
using namespace guyos::net;


ICMP::ICMP(IPProvider* backend)
: IPHandler(backend, 0x01)
{
}

ICMP::~ICMP()
{
}

void printf(char*);
void printfHex(uint8_t);
bool ICMP::OnIPReceived(common::uint32_t srcIP_BE, common::uint32_t dstIP_BE,
                                            common::uint8_t* IPPayload, common::uint32_t size)
{
    if(size < sizeof(ICMPMessage))
        return false;

    ICMPMessage* msg = (ICMPMessage*)IPPayload;

    switch(msg->type)
    {

        case 0:
            printf("ping response from "); printfHex(srcIP_BE & 0xFF);
            printf("."); printfHex((srcIP_BE >> 8) & 0xFF);
            printf("."); printfHex((srcIP_BE >> 16) & 0xFF);
            printf("."); printfHex((srcIP_BE >> 24) & 0xFF);
            printf("\n");
            break;

        case 8:
            msg->type = 0;
            msg->checksum = 0;
            msg->checksum = IPProvider::Checksum((uint16_t*)msg,
                sizeof(ICMPMessage));
            return true;
    }

    return false;
}

void ICMP::RequestEchoReply(uint32_t ip_be)
{
    ICMPMessage icmp;
    icmp.type = 8; // ping
    icmp.code = 0;
    icmp.data = 0x3713; // 1337
    icmp.checksum = 0;
    icmp.checksum = IPProvider::Checksum((uint16_t*)&icmp,
        sizeof(ICMPMessage));

    IPHandler::Send(ip_be, (uint8_t*)&icmp, sizeof(ICMPMessage));
}