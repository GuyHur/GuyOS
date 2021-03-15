
#ifndef __GUYOS__DRIVERS__MOUSE_H
#define __GUYOS__DRIVERS__MOUSE_H

    #include <common/types.h>
    #include <hardware/interrupts.h>
    #include <drivers/driver.h>
    #include <hardware/port.h>
namespace guyos
{
    namespace drivers
    {

    
        class MouseEventHandler
        {
            public:
                MouseEventHandler();
                virtual void OnActivate();
                virtual void OnMouseDown(guyos::common::uint8_t button);
                virtual void OnMouseUp(guyos::common::uint8_t button);
                virtual void OnMouseMove(int x, int y);
        };


        class MouseDriver : public guyos::hardware::InterruptHandler, public Driver
        {
            guyos::hardware::Port8Bit dataport;
            guyos::hardware::Port8Bit commandport;

            guyos::common::uint8_t buffer[3];
            guyos::common::uint8_t offset;
            guyos::common::uint8_t buttons;

            MouseEventHandler *handler;

        public:
            MouseDriver(guyos::hardware::InterruptManager* manager, MouseEventHandler *handler);
            ~MouseDriver();
            virtual guyos::common::uint32_t HandleInterrupt(guyos::common::uint32_t esp);
            virtual void Activate();
        };
    }
}
#endif