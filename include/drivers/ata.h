#ifndef __GUYOS__DRIVERS__ATA_H
#define __GUYOS__DRIVERS__ATA_H

#include <common/types.h>
#include <hardware/interrupts.h>
#include <hardware/port.h>

namespace guyos
{
    namespace drivers
    {

        class ATA
        {
        protected:
            bool master;
            hardware::Port16Bit dataPort;
            hardware::Port8Bit errorPort;
            hardware::Port8Bit sectorCountPort;
            hardware::Port8Bit lbaLowPort;
            hardware::Port8Bit lbaMidPort;
            hardware::Port8Bit lbaHiPort;
            hardware::Port8Bit devicePort;
            hardware::Port8Bit commandPort;
            hardware::Port8Bit controlPort;
        public:

            ATA(bool master, common::uint16_t portBase);
            ~ATA();

            void Identify();
            void Read28(common::uint32_t sectorNum, int count = 512);
            void Write28(common::uint32_t sectorNum, common::uint8_t* data, common::uint32_t count);
            void Flush();


        };

    }
}

#endif