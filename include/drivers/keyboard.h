
#ifndef __GUYOS__DRIVERS__KEYBOARD_H
#define __GUYOS__DRIVERS__KEYBOARD_H

    #include <drivers/driver.h>
    #include <common/types.h>
    #include <hardware/interrupts.h>
    #include <hardware/port.h>
    #include <lib/string.h>

namespace guyos
{

    namespace drivers
    {

    

        class KeyboardEventHandler
        {
            public:
                KeyboardEventHandler();

                virtual void OnKeyDown(char);
                virtual void OnKeyUp(char);
        };


        class KeyboardDriver : public guyos::hardware::InterruptHandler, public Driver
        {
            guyos::hardware::Port8Bit dataport;
            guyos::hardware::Port8Bit commandport;

            KeyboardEventHandler *handler;
        public:
            KeyboardDriver(guyos::hardware::InterruptManager* manager, KeyboardEventHandler *handler);// consturctor
            ~KeyboardDriver();//destructor
            virtual guyos::common::uint32_t HandleInterrupt(guyos::common::uint32_t esp);//handleInterrupt

            virtual void Activate();//activates the driver
        };
    }
}

#endif