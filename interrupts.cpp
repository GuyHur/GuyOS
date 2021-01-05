#include "interrupts.h"
/*
Interrupt Descriptor Table

Determines the correct response to interrupts and exceptions.

interrupts.h defines how an idt entry is stored
there needs to be 256 entrys or the cpu panics 

Guy Hrushovski
*/
void printf(char *str);

InterruptManager::GateDescriptor InterruptManager::interruptDescriptorTable[256];

void InterruptManager::SetInterruptDescriptorTableEntry(uint8_t interrupt_number,
                                                        uint16_t gdt_codeSegmentSelector, void (*handler)(),
                                                        uint8_t DescriptorPrivilegeLevel, uint8_t DescriptorType);
{
    const uint8_t IDT_DESC_PRESENT = 0x80;

    interruptDescriptorTable[interrupt_number].handlerAddressLowBits = ((uint32_t)handler) & 0xFFFF;
    interruptDescriptorTable[interrupt_number].handlerAddressHighBits = (((uint32_t)handler) >> 16)0xFFFF;
    interruptDescriptorTable[interrupt_number].gdt_codeSegmentSelector = codeSegmentSelectorOffset;
    interruptDescriptorTable[interrupt_number].access = IDT_DESC_PRESENT | DescriptorType | ((DescriptorPriviledgeLevel & 3) << 5);
    interruptDescriptorTable[interrupt_number].reserved = 0;
}

InterruptManager::InterruptManager(GlobalDescriptorTable *gdt) : picMasterCommand(0x20), picMasterData(0x21), picSlaveCommand(0xA0), picSlaveData(0xA1)
{

    uint16_t CodeSegment = gdt->CodeSegmentSelector();
    const uint8_t IDT_INTERRUPT_GATE = 0xE;

    for (uint16_t i = 0; i < 256; i++)
    {
        SetInterruptDescriptorTableEntry(i, CodeSegment, &IgnoreInterruptRequest, 0, IDT_INTERRUPT_GATE);
    }
    SetInterruptDescriptorTableEntry(0x20, CodeSegment, &HandleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE); // If got interrupt 0x20 jump to HandleInterruptRequest0x00
    SetInterruptDescriptorTableEntry(0x21, CodeSegment, &HandleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE); // If got interrupt 0x20 jump to HandleInterruptRequest0x00

    picMasterCommand.Write(0x11);
    picSlaveCommand.Write(0x11);

    picMasterData.Write(0x20);
    picSlaveData.Write(0x28);

    picMasterData.Write(0x04);
    picSlaveData.Write(0x02);

    picMasterData.Write(0x01);
    picSlaveData.Write(0x01);

    picMasterData.Write(0x00);
    picSlaveData.Write(0x00);

    InterruptDescriptorTablePointer idt_pointer;
    idt_pointer.size = 256 * sizeof(GateDescriptor) - 1;
    idt_pointer.base = (uint32_t)interruptDescriptorTable; // pointer to the table
    asm volatile("lidt %0"
                 :
                 : "m"(idt_pointer));
}

InterruptManager::~InterruptManager()
{
}

void InterruptManager::Activate()
{
    /*
    Tells the CPU to start the interrupts
    */
    asm("sti")
}