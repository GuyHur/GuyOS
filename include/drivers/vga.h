#ifndef __GUYOS__DRIVERS__VGA_H
#define __GUYOS__DRIVERS__VGA_H

#include <common/types.h>
#include <hardware/port.h>
#include <drivers/driver.h>

namespace guyos
{
    namespace drivers
    {
        class VGA
        {
            protected:
                hardware::Port8Bit miscPort;
                hardware::Port8Bit crtcIndexPort;
                hardware::Port8Bit crtcDataPort;
                hardware::Port8Bit sequencerIndexPort;
                hardware::Port8Bit sequencerDataPort;
                hardware::Port8Bit graphicsControllerIndexPort;
                hardware::Port8Bit graphicsControllerDataPort;
                hardware::Port8Bit attributeControllerIndexPort;
                hardware::Port8Bit attributeControllerReadPort;
                hardware::Port8Bit attributeControllerWritePort;
                hardware::Port8Bit attributeControllerResetPort;
                
                void WriteRegisters(common::uint8_t *registers);
                common::uint8_t* GetFrameBufferSegment();

            virtual common::uint8_t GetColorIndex(guyos::common::uint8_t r, guyos::common::uint8_t g, guyos::common::uint8_t b);

            public:
                VGA();
                ~VGA();

                virtual bool SupportsMode(common::uint32_t width, common::uint32_t height, common::uint32_t colordepth);
                virtual bool SetMode(common::uint32_t width, common::uint32_t height, common::uint32_t colordepth);
                virtual void PutPixel(common::uint32_t x, common::uint32_t y,  common::uint8_t r, common::uint8_t g, common::uint8_t b);
                virtual void PutPixel(common::uint32_t x, common::uint32_t y, common::uint8_t colorIndex);

                
        };
    }
}



#endif