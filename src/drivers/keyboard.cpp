
#include <drivers/keyboard.h>
using namespace guyos::drivers;
using namespace guyos::common;
using namespace guyos::hardware;

KeyboardEventHandler::KeyboardEventHandler()
{

}

void KeyboardEventHandler::OnKeyDown(char)
{

}
void KeyboardEventHandler::OnKeyUp(char)
{

}


KeyboardDriver::KeyboardDriver(InterruptManager* manager, KeyboardEventHandler *handler)
:InterruptHandler(manager, 0x21),
dataport(0x60),
commandport(0x64)
{
    this->handler = handler;
}

/**
 *  Keyboard Encoder
 *   port 0x60 with Read -> Read input buffer
 *   port 0x60 with Write -> Send command
 * Onboard Keyboard Controller
 * port 0x64 with Read -> Status register
 * port 0x64 with Write -> send command, bit 0 is the output buffer status
 * bit 1 is input buffer status
 * 
 * 
 * 
 * */


KeyboardDriver::~KeyboardDriver()
{
}

void KeyboardDriver::Activate()
{
    while(commandport.Read() & 0x1)// clear the keyboard buffer
        dataport.Read();
    commandport.Write(0xae); // Enables the keyboard
    commandport.Write(0x20); // command 0x20 = read controller command byte
    uint8_t status = (dataport.Read() | 1) & ~0x10;// read the status of the data port
    commandport.Write(0x60); // command 0x60 = Write command byte, afterwards we change the state of data port
    dataport.Write(status);
    dataport.Write(0xf4);// Enable keyboard encoder command
}

void printf(char*);
void printfHex(uint8_t);

static char key_buffer[256];

uint32_t KeyboardDriver::HandleInterrupt(uint32_t esp)// Handles the key presses and releases
{
    uint8_t key = dataport.Read();

    if(handler == 0)
        return esp;

    if(key < 0x80)
    {
        switch(key)
        {
            case 0x02: handler->OnKeyDown('1');append(key_buffer, {'1'}); break;
            case 0x03: handler->OnKeyDown('2');append(key_buffer, {'2'}); break;
            case 0x04: handler->OnKeyDown('3');append(key_buffer, {'3'}); break;
            case 0x05: handler->OnKeyDown('4');append(key_buffer, {'4'}); break;
            case 0x06: handler->OnKeyDown('5');append(key_buffer, {'5'}); break;
            case 0x07: handler->OnKeyDown('6');append(key_buffer, {'6'}); break;
            case 0x08: handler->OnKeyDown('7');append(key_buffer, {'7'}); break;
            case 0x09: handler->OnKeyDown('8');append(key_buffer, {'8'}); break;
            case 0x0A: handler->OnKeyDown('9');append(key_buffer, {'9'}); break;
            case 0x0B: handler->OnKeyDown('0');append(key_buffer, {'0'}); break;

            case 0x10: handler->OnKeyDown('q');append(key_buffer, {'q'}); break;
            case 0x11: handler->OnKeyDown('w');append(key_buffer, {'w'}); break;
            case 0x12: handler->OnKeyDown('e');append(key_buffer, {'e'}); break;
            case 0x13: handler->OnKeyDown('r');append(key_buffer, {'r'}); break;
            case 0x14: handler->OnKeyDown('t');append(key_buffer, {'t'}); break;
            case 0x15: handler->OnKeyDown('z');append(key_buffer, {'z'}); break;
            case 0x16: handler->OnKeyDown('u');append(key_buffer, {'u'}); break;
            case 0x17: handler->OnKeyDown('i');append(key_buffer, {'i'}); break;
            case 0x18: handler->OnKeyDown('o');append(key_buffer, {'o'}); break;
            case 0x19: handler->OnKeyDown('p');append(key_buffer, {'p'}); break;

            case 0x1E: handler->OnKeyDown('a');append(key_buffer, {'a'}); break;
            case 0x1F: handler->OnKeyDown('s');append(key_buffer, {'s'}); break;
            case 0x20: handler->OnKeyDown('d');append(key_buffer, {'d'}); break;
            case 0x21: handler->OnKeyDown('f');append(key_buffer, {'f'}); break;
            case 0x22: handler->OnKeyDown('g');append(key_buffer, {'g'}); break;
            case 0x23: handler->OnKeyDown('h');append(key_buffer, {'h'}); break;
            case 0x24: handler->OnKeyDown('j');append(key_buffer, {'j'}); break;
            case 0x25: handler->OnKeyDown('k');append(key_buffer, {'k'}); break;
            case 0x26: handler->OnKeyDown('l');append(key_buffer, {'l'}); break;

            case 0x2C: handler->OnKeyDown('y');append(key_buffer, {'y'}); break;
            case 0x2D: handler->OnKeyDown('x');append(key_buffer, {'x'}); break;
            case 0x2E: handler->OnKeyDown('c');append(key_buffer, {'c'}); break;
            case 0x2F: handler->OnKeyDown('v');append(key_buffer, {'v'}); break;
            case 0x30: handler->OnKeyDown('b');append(key_buffer, {'b'}); break;
            case 0x31: handler->OnKeyDown('n');append(key_buffer, {'n'}); break;
            case 0x32: handler->OnKeyDown('m');append(key_buffer, {'m'}); break;
            case 0x33: handler->OnKeyDown(',');append(key_buffer, {','}); break;
            case 0x34: handler->OnKeyDown('.');append(key_buffer, {'.'}); break;
            case 0x35: handler->OnKeyDown('-');append(key_buffer, {'-'}); break;

            //Enter
            case 0x1C: handler->OnKeyDown('\n'); user_input(key_buffer); key_buffer[0] = '\0'; break;
            case 0x0F: handler->OnKeyDown('\t'); break;
            //backspace(delete)
            case 0x0E: 
                       backspace(key_buffer);
                       handler->OnKeyDown('\b'); 
                       break;

            case 0x39: handler->OnKeyDown(' '); break;

            

            default:
            {
                printf("KEYBOARD 0x");
                printfHex(key);
                break;
            }
        }
    }
    return esp;
}