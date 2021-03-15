#ifndef __GUYOS__HARDWARE__PCI_H
#define __GUYOS__HARDWARE__PCI_H

#include <common/types.h>
#include <hardware/port.h>
#include <hardware/interrupts.h>
#include <drivers/driver.h>
namespace guyos
{
    namespace hardware
    {

        enum BaseAddressRegisterType
        {
            MemoryMapping = 0,
            InputOutput = 1
        };

        class BaseAddressRegister
        {
            public:
                bool prefetchable;
                guyos::common::uint8_t *address;
                guyos::common::uint32_t size;
                BaseAddressRegisterType type;
        };

        class PeripheralComponentInterconnectDeviceDescriptor
        {
            public:
                guyos::common::uint32_t portBase;
                guyos::common::uint32_t interrupt;

                guyos::common::uint16_t bus;
                guyos::common::uint16_t device;
                guyos::common::uint16_t function;

                guyos::common::uint16_t vendor_id;
                guyos::common::uint16_t device_id;

                guyos::common::uint8_t class_id;
                guyos::common::uint8_t subclass_id;
                guyos::common::uint8_t interface_id;

                guyos::common::uint8_t revision;

                PeripheralComponentInterconnectDeviceDescriptor();
                    //constructor
                ~PeripheralComponentInterconnectDeviceDescriptor();
                    //destructor


        };

            class PeripheralComponentInterconnectController
            {
                Port32Bit dataPort;
                Port32Bit commandPort;

                public:
                    PeripheralComponentInterconnectController();

                    ~PeripheralComponentInterconnectController();

                    guyos::common::uint32_t Read(guyos::common::uint16_t bus, guyos::common::uint16_t device, guyos::common::uint16_t function, guyos::common::uint32_t registeroffset);

                    void Write(guyos::common::uint16_t bus, guyos::common::uint16_t device, guyos::common::uint16_t function, guyos::common::uint32_t registeroffset, guyos::common::uint32_t value);

                    bool DeviceHasFunctions(guyos::common::uint16_t bus, guyos::common::uint16_t device);

                    void SelectDrivers(guyos::drivers::DriverManager *driverManager, guyos::hardware::InterruptManager* interrupts);

                    guyos::drivers::Driver* GetDriver(PeripheralComponentInterconnectDeviceDescriptor dev, guyos::hardware::InterruptManager* interrupts);
                
                    PeripheralComponentInterconnectDeviceDescriptor GetDeviceDescriptor(guyos::common::uint16_t bus, guyos::common::uint16_t device, guyos::common::uint16_t function);

                    BaseAddressRegister GetBaseAddressRegister(guyos::common::uint16_t bus, guyos::common::uint16_t device, guyos::common::uint16_t function, guyos::common::uint16_t bar);

            };
    }
}
#endif