
#ifndef __GUYOS__HARDWARE__INTERRUPTMANAGER_H
#define __GUYOS__HARDWARE__INTERRUPTMANAGER_H

#include <gdt.h>
#include <common/types.h>
#include <hardware/port.h>


/*
class InterruptManager -> Manages the interrupts
ActiveInterruptManager -> Activates the interrupt manager
handlers -> an array of 256 possible interrupts


class InterruptHandler -> handles an interrupt

uint8_t InterruptNumber -> The interrupt number
InterruptManager* interruptManager -> a pointer to the interruptManager
uint32_t HandleInterrupt(uint32_t esp) -> handles an interrupt, esp: interrupt



*/

namespace guyos
{
    namespace hardware
    {

        class InterruptManager;

        class InterruptHandler
        {
        protected:
            guyos::common::uint8_t InterruptNumber;
            InterruptManager* interruptManager;
            InterruptHandler(InterruptManager* interruptManager, guyos::common::uint8_t InterruptNumber);
            ~InterruptHandler();
        public:
            virtual guyos::common::uint32_t HandleInterrupt(guyos::common::uint32_t esp);
        };


        class InterruptManager
        {
            friend class InterruptHandler;
            protected:

                static InterruptManager* ActiveInterruptManager;
                InterruptHandler* handlers[256];

                struct GateDescriptor
                {
                    guyos::common::uint16_t handlerAddressLowBits;
                    guyos::common::uint16_t gdt_codeSegmentSelector;
                    guyos::common::uint8_t reserved;
                    guyos::common::uint8_t access;
                    guyos::common::uint16_t handlerAddressHighBits;
                } __attribute__((packed));

                static GateDescriptor interruptDescriptorTable[256];

                struct InterruptDescriptorTablePointer
                {
                    guyos::common::uint16_t size;
                    guyos::common::uint32_t base;
                } __attribute__((packed));

                guyos::common::uint16_t hardwareInterruptOffset;
                static void SetInterruptDescriptorTableEntry(guyos::common::uint8_t interrupt,
                    guyos::common::uint16_t codeSegmentSelectorOffset, void (*handler)(),
                    guyos::common::uint8_t DescriptorPrivilegeLevel, guyos::common::uint8_t DescriptorType);


                static void InterruptIgnore();

                static void HandleInterruptRequest0x00();
                static void HandleInterruptRequest0x01();
                static void HandleInterruptRequest0x02();
                static void HandleInterruptRequest0x03();
                static void HandleInterruptRequest0x04();
                static void HandleInterruptRequest0x05();
                static void HandleInterruptRequest0x06();
                static void HandleInterruptRequest0x07();
                static void HandleInterruptRequest0x08();
                static void HandleInterruptRequest0x09();
                static void HandleInterruptRequest0x0A();
                static void HandleInterruptRequest0x0B();
                static void HandleInterruptRequest0x0C();
                static void HandleInterruptRequest0x0D();
                static void HandleInterruptRequest0x0E();
                static void HandleInterruptRequest0x0F();
                static void HandleInterruptRequest0x31();

                static void HandleException0x00();
                static void HandleException0x01();
                static void HandleException0x02();
                static void HandleException0x03();
                static void HandleException0x04();
                static void HandleException0x05();
                static void HandleException0x06();
                static void HandleException0x07();
                static void HandleException0x08();
                static void HandleException0x09();
                static void HandleException0x0A();
                static void HandleException0x0B();
                static void HandleException0x0C();
                static void HandleException0x0D();
                static void HandleException0x0E();
                static void HandleException0x0F();
                static void HandleException0x10();
                static void HandleException0x11();
                static void HandleException0x12();
                static void HandleException0x13();

                static guyos::common::uint32_t HandleInterrupt(guyos::common::uint8_t interrupt, guyos::common::uint32_t esp);
                guyos::common::uint32_t DoHandleInterrupt(guyos::common::uint8_t interrupt, guyos::common::uint32_t esp);

                Port8BitSlow programmableInterruptControllerMasterCommandPort;
                Port8BitSlow programmableInterruptControllerMasterDataPort;
                Port8BitSlow programmableInterruptControllerSlaveCommandPort;
                Port8BitSlow programmableInterruptControllerSlaveDataPort;

            public:
                InterruptManager(guyos::common::uint16_t hardwareInterruptOffset, guyos::GlobalDescriptorTable* globalDescriptorTable);
                ~InterruptManager();
                guyos::common::uint16_t HardwareInterruptOffset();
                void Activate();
                void Deactivate();
        };
        
    }
}

#endif