#ifndef _GDT_H
#define _GDT_H

    /*
    To understand this code please learn about GDT(Global Descriptor table)
    The class GlobalDescriptorTable represents the GDT, while the SegmentDescriptor subclass
    Represents a Segment in the GDT
    */
#include <common/types.h>
    class GlobalDescriptorTable
    {
        public:
            class SegmentDescriptor
            {
                private:
                    uint16_t limit_lo;
                    uint16_t base_lo;
                    uint8_t base_hi;
                    uint8_t type;
                    uint8_t flags_limit_hi;
                    uint8_t base_vhi;
                public:
                    SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t type);

                    uint32_t Base(); // Mangales with stuff to get the low bits & high bits

                    uint32_t Limit(); // gets the limit.

            }__attribute__((packed));//Prevents compiler from moving stuff around
        
        SegmentDescriptor nullSegmentSelector;
        SegmentDescriptor codeSegmentSelector;
        SegmentDescriptor dataSegmentSelector;
        SegmentDescriptor unusedSegmentSelector;
    public:
        GlobalDescriptorTable(); // Constructor
        ~GlobalDescriptorTable();// Destructor
        uint16_t CodeSegmentSelector();
        uint16_t DataSegmentSelector();

    };




#endif