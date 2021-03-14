
#ifndef _MOUSE_H
#define _MOUSE_H

    #include <common/types.h>
    #include <hardware/interrupts.h>
    #include <hardware/port.h>

    class MouseEventHandler
    {
        public:
            MouseEventHandler();
            virtual void OnActivate();
            virtual void OnMouseDown(uint8_t button);
            virtual void OnMouseUp(uint8_t button);
            virtual void OnMouseMove(int x, int y);
    };


    class MouseDriver : public InterruptHandler
    {
        Port8Bit dataport;
        Port8Bit commandport;

        uint8_t buffer[3];
        uint8_t offset;
        uint8_t buttons;

        MouseEventHandler *handler;

    public:
        MouseDriver(InterruptManager* manager, MouseEventHandler *handler);
        ~MouseDriver();
        virtual uint32_t HandleInterrupt(uint32_t esp);
        virtual void Activate();
    };

#endif