#ifndef __GUYOS__GDT_H
#define __GUYOS__GDT_H

    /*
To understand this code please learn about GDT(Global Descriptor table)
The class GlobalDescriptorTable represents the GDT, while the SegmentDescriptor subclass
Represents a Segment in the GDT
*/


/*
SegmentDescriptor:        
    
lo:
the least significant bits of the segment descriptor, hi means most significant bits

uint32_t Base() -> Computes the base address of the segment

uint32_t Limit() -> Computes the limit of the segment

__attribute__((packed)) -> tells the compiler to not change any alignment in the structure, 
the packed attribute specifies that a structure, or a variable should have the smallest possible alignment.
one byte for a variable, and one bit for a field 

uint16_t CodeSegmentSelector() ->
returns the offset of the code segment in the gdt

uint16_t DataSegmentSelector() ->
returns the offset of the data segment in the gdt

*/
#include <common/types.h>

namespace guyos
{
    
    class GlobalDescriptorTable
    {
        public:

            class SegmentDescriptor
            {
                private:
                    guyos::common::uint16_t limit_lo;
                    guyos::common::uint16_t base_lo;
                    guyos::common::uint8_t base_hi;
                    guyos::common::uint8_t type;
                    guyos::common::uint8_t limit_hi;
                    guyos::common::uint8_t base_vhi;

                public:
                    SegmentDescriptor(guyos::common::uint32_t base, guyos::common::uint32_t limit, guyos::common::uint8_t type);
                    guyos::common::uint32_t Base();
                    guyos::common::uint32_t Limit();
            } __attribute__((packed));

        private:
            SegmentDescriptor nullSegmentSelector;
            SegmentDescriptor unusedSegmentSelector;
            SegmentDescriptor codeSegmentSelector;
            SegmentDescriptor dataSegmentSelector;

        public:

            GlobalDescriptorTable();
            ~GlobalDescriptorTable();

            guyos::common::uint16_t CodeSegmentSelector();
            guyos::common::uint16_t DataSegmentSelector();
    };

}
    
#endif