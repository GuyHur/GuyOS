#include <common/types.h>
#include <gdt.h>
#include <hardware/interrupts.h>
#include <hardware/pci.h>
#include <drivers/driver.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#include <drivers/vga.h>
#include <multitasking.h>
#include <net/arp.h>
#include <syscalls.h>
#include <memory.h>
#include <drivers/ata.h>

#include <drivers/amd_am79c973.h>
#include <net/etherframe.h>
#include <net/arp.h>
#include <net/tcp.h>
#include <net/ipv4.h>
#include <net/icmp.h>
#include <net/udp.h>

#include <drivers/vga.h>
#include <gui/desktop.h>
#include <gui/window.h>

#include <lib/system.h>


using namespace guyos;
using namespace guyos::gui;
using namespace guyos::common;
using namespace guyos::drivers;
using namespace guyos::hardware;
using namespace guyos::net;





uint8_t calculate_color(uint8_t foreground, uint8_t background)
{
    return foreground | (background << 4);
}
/*
void init_shell()
{
    static uint8_t x=0, y= 0;
    static uint8_t color = calculate_color(0x0F, 0x00);
    static uint16_t* videoMemory = (uint16_t*)0xB8000;
    for(uint8_t y=0; y < 80; y++)
    {
        for(uint8_t x=0; x < 25; x++)
        {
            const size_t index = y*80 + x;
            videoMemory[index] = ((uint16_t)" " | (uint16_t) color << 8);
        }
    }
}
*/

void kprintf(char* c);



void printf(char* str)
{
    static uint16_t* VideoMemory = (uint16_t*)0xb8000;

    static uint8_t x=0,y=0;

    for(int i = 0; str[i] != '\0'; ++i)
    {
        switch(str[i])
        {
            case '\n':
                x = 0;
                y++;
                break;
            case '\t':
                x +=4;
            default:
                VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | str[i];
                x++;
                break;
        }

        if(x >= 80)
        {
            x = 0;
            y++;
        }

        if(y >= 25)
        {
            for(y = 0; y < 25; y++)
                for(x = 0; x < 80; x++)
                    VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | ' ';
            x = 0;
            y = 0;
        }
    }
}

void printfHex(uint8_t key)
{
    char* buffer = "00";
    char* hex = "0123456789ABCDEF";
    buffer[0] = hex[(key >> 4) & 0xF];
    buffer[1] = hex[key & 0xF];
    printf(buffer);
}

void printfHex16(uint16_t key)
{
    printfHex((key >> 8) & 0xFF);
    printfHex( key & 0xFF);
}
void printfHex32(uint32_t key)
{
    printfHex((key >> 24) & 0xFF);
    printfHex((key >> 16) & 0xFF);
    printfHex((key >> 8) & 0xFF);
    printfHex( key & 0xFF);
}


class PrintfTCPHandler : public TCPHandler
{
public:
    bool HandleTCPMessage(TCPSocket* socket, common::uint8_t* data, common::uint16_t size)
    {
        char* buf = " ";
        for(int i = 0; i < size; i++)
        {
            buf[0] = data[i];
            printf(buf);
        }

        if(size > 9
            && data[0] == 'G'
            && data[1] == 'E'
            && data[2] == 'T'
            && data[3] == ' '
            && data[4] == '/'
            && data[5] == ' '
            && data[6] == 'H'
            && data[7] == 'T'
            && data[8] == 'T'
            && data[9] == 'P'
        )
        {
            //FIXME: heartbleed String overwrite?
            socket->Send((uint8_t*)"HTTP/1.1 200 OK\r\nServer: GuyOS\r\nContent-Type: text/html\r\n\r\n<html><head><title>GuyOS</title></head><body><b>GuyOS</b></body></html>\r\n",142);
            socket->Disconnect();
        }  

        return true;
    }
};

class PrintfKeyboardEventHandler : public KeyboardEventHandler
{
public:
    void OnKeyDown(char c)
    {
        char* buffer = " ";
        buffer[0] = c;   
        printf(buffer);
    }
};

void keyboardHandler(uint8_t scanCode, uint8_t chr)
{

}

class MouseToConsole : public MouseEventHandler
{
    int8_t x, y;
public:
    
    MouseToConsole()
    {
        uint16_t* VideoMemory = (uint16_t*)0xb8000;
        x = 40;
        y = 12;
        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);        
    }
    
    virtual void OnMouseMove(int xoffset, int yoffset)
    {
        static uint16_t* VideoMemory = (uint16_t*)0xb8000;
        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);

        x += xoffset;
        if(x >= 80) x = 79;
        if(x < 0) x = 0;
        y += yoffset;
        if(y >= 25) y = 24;
        if(y < 0) y = 0;

        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);
    }
    
};

class PrintfUDPHandler : public UDPHandler
{
    public:
        void HandleUDPMessage(UDPSocket* socket, common::uint8_t* data, common::uint16_t size)
        {
            char* buffer = " ";
            for(int i = 0; i < size; i++)
            {
                buffer[0] = data[i];
                printf(buffer);
            }
        }   
};

void sysprintf(char *str)
{
    asm("int $0x80" : : "a" (4), "b" (str));
}


void taskA()
{
    while(true)
        sysprintf("Hello");
}

void taskB()
{
    while(true)
        sysprintf("World");
}


typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()// enable the constructors
{
    for (constructor *i = &start_ctors; i != &end_ctors; i++)
    {
        (*i)();
    }
}

extern "C" void kernelMain(const void *multiboot_structure, uint32_t /*multiboot_magic*/)
{
    printf("Welcome to GuyOS!\n");

    GlobalDescriptorTable gdt;// initialize the GDT

    uint32_t* memupper = (uint32_t*)(((size_t)multiboot_structure) + 8);// dynamic memory allocation
    size_t heap = 10*1024*1024;// heap size
    MemoryManager memoryManager(heap, (*memupper)*1024 - heap - 10*1024);

    printf("heap: 0x");
    printfHex((heap >> 24) & 0xFF);
    printfHex((heap >> 16) & 0xFF);
    printfHex((heap >> 8 ) & 0xFF);
    printfHex((heap      ) & 0xFF);

    void* allocated = memoryManager.malloc(1024);
    printf("\nallocated: 0x");
    printfHex(((size_t)allocated >> 24) & 0xFF);
    printfHex(((size_t)allocated >> 16) & 0xFF);
    printfHex(((size_t)allocated >> 8 ) & 0xFF);
    printfHex(((size_t)allocated      ) & 0xFF);
    printf("\n");

    TaskManager taskManager;// initialize task manager
    //Show Task Manager
    /*
    Task task1(&gdt, taskA);
    Task task2(&gdt, taskB);
    taskManager.AddTask(&task1);
    taskManager.AddTask(&task2);
    */
    InterruptManager interrupts(0x20, &gdt, &taskManager);//interrupt manager
    SyscallHandler syscalls(&interrupts, 0x80);// syscalls

    printf("Initializing Hardware...\n");
    #ifdef GRAPHICSMODE
        Desktop desktop(320,200, 0x00,0x00,0xA8);
    #endif
    
    DriverManager drvManager;
    
        #ifdef GRAPHICSMODE
            KeyboardDriver keyboard(&interrupts, &desktop);
        #else
            PrintfKeyboardEventHandler kbhandler;
            KeyboardDriver keyboard(&interrupts, &kbhandler);
        #endif
        drvManager.AddDriver(&keyboard);
        
    
        #ifdef GRAPHICSMODE
            MouseDriver mouse(&interrupts, &desktop);
        #else
            MouseToConsole mousehandler;
            MouseDriver mouse(&interrupts, &mousehandler);
        #endif
        drvManager.AddDriver(&mouse);
        
        PeripheralComponentInterconnectController PCIController;
        PCIController.SelectDrivers(&drvManager, &interrupts);

        #ifdef GRAPHICSMODE
            VideoGraphicsArray vga;
        #endif

    printf("Finalizing Hardware...\n");

    drvManager.ActivateAll();

    printf("Activating interrupts...\n");







    /*
    printf("\nS-ATA primary master: ");
    ATA ata0m(true, 0x1F0);
    ata0m.Identify();

    printf("\nS-ATA primary slave: ");
    ATA ata0s(false, 0x1F0);
    ata0s.Identify();
    ata0s.Write28(0, (uint8_t*)"TestGuy", 7);
    ata0s.Flush();
    ata0s.Read28(0, 25);

    printf("\nS-ATA secondary master: ");
    ATA ata1m(true, 0x170);
    ata1m.Identify();

    printf("\nS-ATA secondary slave: ");
    ATA ata1s(false, 0x170);
    ata1s.Identify();

    // third: 0x1E8
    // fourth: 0x168
    */

   //Networking...
   //http://beej.us/guide/bgnet/ Network programming
   //http://www.saminiir.com/lets-code-tcp-ip-stack-1-ethernet-arp/ // Building a Network stack



   //ip address
    amd_am79c973* eth0 = (amd_am79c973*)(drvManager.drivers[2]);

    uint8_t ip1 = 10, ip2 = 0, ip3 = 2, ip4 = 15;// 10.0.2.15
    uint32_t ip_be = ((uint32_t)ip4 << 24)
                | ((uint32_t)ip3 << 16)
                | ((uint32_t)ip2 << 8)
                | (uint32_t)ip1;

    eth0->SetIPAddress(ip_be);
    EtherFrameProvider etherframe(eth0);
    ARP arp(&etherframe);    


    //ip of default gateway
    uint8_t gip1 = 10, gip2 = 0, gip3 = 2, gip4 = 2;
    uint32_t gip_be = ((uint32_t)gip4 << 24)
                   | ((uint32_t)gip3 << 16)
                   | ((uint32_t)gip2 << 8)
                   | (uint32_t)gip1;

    uint8_t subnet1 = 255, subnet2 = 255, subnet3 = 255, subnet4 = 0;
    uint32_t subnet_be = ((uint32_t)subnet4 << 24)
                   | ((uint32_t)subnet3 << 16)
                   | ((uint32_t)subnet2 << 8)
                   | (uint32_t)subnet1;


    IPProvider ipv4(&etherframe, &arp, gip_be, subnet_be); 
    ICMP icmp(&ipv4);   
    UDPProvider udp(&ipv4);
    TCPProvider tcp(&ipv4);


    interrupts.Activate();

    printf("Initializing Networking Services...");
    /*Show TCP
    */ 
    //Show ICMP
    arp.BroadcastMACAddress(gip_be);

    PrintfTCPHandler tcphandler;
    TCPSocket* tcpsocket = tcp.Listen(12345);
    tcp.Bind(tcpsocket, &tcphandler);
    
    // END Show TCP

    //tcpsocket->Send((uint8_t*)"Hello TCP!", 10);

    //Show ICMP
    //icmp.RequestEchoReply(gip_be);

    /*Show UDP
    PrintfUDPHandler udphandler;
    UDPSocket* udpsocket = udp.Connect(gip_be, 1234);
    udp.Bind(udpsocket, &udphandler);
    udpsocket->Send((uint8_t*)"Hello UDP!", 10);

    UDPSocket* udpsocket = udp.Listen(1234);
    udp.Bind(udpsocket, &udphandler);
    */
    //help_command();

    

    while(1)
    {
        #ifdef GRAPHICSMODE
            desktop.draw(&vga);
        #endif
    }
}
