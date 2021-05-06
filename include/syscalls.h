#ifndef __GUYOS__SYSCALLS_H
#define __GUYOS__SYSCALLS_H

#include <common/types.h>
#include <hardware/interrupts.h>
#include <multitasking.h>

namespace guyos
{

    class SyscallHandler : public hardware::InterruptHandler
    {

    public:
        SyscallHandler(hardware::InterruptManager* interruptManager, guyos::common::uint8_t InterruptNumber);
        ~SyscallHandler();

        virtual guyos::common::uint32_t HandleInterrupt(guyos::common::uint32_t esp);

    };

}



#endif