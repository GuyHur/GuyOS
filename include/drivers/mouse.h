
#ifndef _KEYBOARD_H
#define _KEYBOARD_H

    #include <src/common/types.h>
    #include <hardware/interrupts.h>
    #include <hardware/port.h>

    class MouseDriver : public InterruptHandler
    {
        Port8Bit dataport;
        Port8Bit commandport;

        uint8_t buffer[3];
        uint8_t offset;
        uint8_t buttons;
    public:
        MouseDriver(InterruptManager* manager);
        ~MouseDriver();
        virtual uint32_t HandleInterrupt(uint32_t esp);
    };

#endif