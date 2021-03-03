
#ifndef _KEYBOARD_H
#define _KEYBOARD_H

    #include <common/types.h>
    #include <hardware/interrupts.h>
    #include <hardware/port.h>

    class KeyboardDriver : public InterruptHandler
    {
        Port8Bit dataport;
        Port8Bit commandport;
    public:
        KeyboardDriver(InterruptManager* manager);
        ~KeyboardDriver();
        virtual uint32_t HandleInterrupt(uint32_t esp);
    };

#endif