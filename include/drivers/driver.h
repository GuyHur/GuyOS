#ifndef __GUYOS__DRIVERS__DRIVER_H
#define __GUYOS__DRIVERS__DRIVER_H
/*
class Driver provides abstraction for Drivers, each driver must have 3 functions
Activate()-> activates the driver
Reset() -> resets the driver
Deactivate() -> Deactivates the driver

class DriverManager manages the drivers
it has an array of 256 drivers
addDriver(Driver*) -> receives a driver, adds it to the array of drivers
ActivateAll() -> activates all the drivers
*/
namespace guyos
{
    namespace drivers
    {
        class Driver
        {
            public:
                Driver();
                ~Driver();

                virtual void Activate();
                virtual int Reset();
                virtual void Deactivate();

        };

        class DriverManager
        {
            private:
                Driver* drivers[265];
                int numDrivers;

            public:
                DriverManager();
                void AddDriver(Driver*);
                void ActivateAll();
            
        };
    }
}
#endif