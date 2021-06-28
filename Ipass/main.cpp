#include "hwlib.hpp"
#include "test.hpp"
#include "mpu6050.hpp"


int main( void ){	  
    uint_fast8_t default_address= 0x68;
   namespace target = hwlib::target;
   
   auto scl = target::pin_oc( target::pins::scl );
   auto sda = target::pin_oc( target::pins::sda );
   
   auto fake_i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda( scl,sda );
   auto data = mpu6050(fake_i2c_bus, default_address);
   data.speed_Test();
  
  
// defaulte 16384.0  accel
//deafaulte 131 gyro

// 1g = 35,303940000000004 km/u 
}








