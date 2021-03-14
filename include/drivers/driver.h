#ifndef _DRIVER_H
#define _DRIVER_H
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
        public:
            DriverManager();
            void addDriver(Driver *);

            void ActivateAll();
            Driver * drivers[265];
            int numDrivers;
    };

#endif