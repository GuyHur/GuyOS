#include <syscalls.h>

using namespace guyos;
using namespace guyos::common;
using namespace guyos::hardware;
//Constructor
SyscallHandler::SyscallHandler(InterruptManager* interruptManager, uint8_t InterruptNumber): InterruptHandler(interruptManager, InterruptNumber  + interruptManager->HardwareInterruptOffset())
{

}

SyscallHandler::~SyscallHandler()
{

}
//destructor

void printf(char*);
void kprintf(const char*, unsigned char);

#define FOREGROUND_WHITE 0x0F

#define BACKGROUND_BLACK 0x00

uint32_t SyscallHandler::HandleInterrupt(uint32_t esp)
{
    CPUState* cpu = (CPUState*)esp;
    /*
    eax holds the action.

    TODO: create more cases by this list:
    http://vm.ru54.com/asm/derick_tut/syscalls.html
    or
    https://chromium.googlesource.com/chromiumos/docs/+/master/constants/syscalls.md#x86-32_bit
    */

    switch(cpu->eax)
    {
        case 4:
            kprintf((const char*)cpu->ebx, 0x00 | 0x0F);
            break;

        default:
            break;
    }


    return esp;
}