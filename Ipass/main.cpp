#include "hwlib.hpp"
#include "mpu6050.hpp"


int main( void ){	  
   uint_fast8_t default_address_mpu = 0x68;
   namespace target = hwlib::target;
   
   auto scl = target::pin_oc( target::pins::scl );
   auto sda = target::pin_oc( target::pins::sda );
   
   auto mpu_i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda( scl,sda );
   auto data = mpu6050(mpu_i2c_bus, default_address_mpu);
    
  


/// Connetion with the oled 
   auto scl2 = target::pin_oc( target::pins::scl1 );
   auto sda2 = target::pin_oc( target::pins::sda1 );
   auto oled_i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda( scl2,sda2 );
   auto oled_data = hwlib::glcd_oled( oled_i2c_bus, 0x3C );  

/// setting the font for the oled and putting it in terminal_from function. So its possible to put text on the oled
   auto font    = hwlib::font_default_16x16();
   auto display = hwlib::terminal_from( oled_data, font );
   data.test_function();
   hwlib::wait_ms(5000);
for(;;){
display
<< "\f" << "\n" 
<< data.speed_Test() <<""
<< hwlib::flush;   
hwlib::wait_ms(500);
}
         
}








