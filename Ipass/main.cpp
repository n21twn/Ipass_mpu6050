#include "hwlib.hpp"
#include "mpu6050.hpp"


/// \brief
/// Oled
/// \details
/// connect the oled with the scl and sda pin.
/// change the size of the text and set the led to textual.
/// print the given variable on the oled screen
void oled(uint16_t  value_high){       
    namespace target = hwlib::target;
   auto scl2 = target::pin_oc( target::pins::scl1 );
   auto sda2 = target::pin_oc( target::pins::sda1 );
   auto oled_i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda( scl2,sda2 );
   auto oled_data = hwlib::glcd_oled( oled_i2c_bus, 0x3C );  
/// setting the font for the oled and putting it in terminal_from function. So its possible to put text on the oled
       auto font    = hwlib::font_default_16x16();
       auto display = hwlib::terminal_from( oled_data, font );

       display
               << "\f" << "\n"
              << value_high <<""
             << hwlib::flush;   
             hwlib::wait_ms(500);
    }    
    
/// \brief
/// main
/// \details
/// set all pins that are connected to the arduino due.
/// set the bus for the mpu6050 and comunicate with the mpu6050
int main( void ){	  
   uint_fast8_t default_address_mpu = 0x68;
   namespace target = hwlib::target;
   
   auto knop1 = target::pin_in( target::pins::d13);
   
   auto scl = target::pin_oc( target::pins::scl );
   auto sda = target::pin_oc( target::pins::sda );
   
   auto mpu_i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda( scl,sda );
   auto data = mpu6050(mpu_i2c_bus, default_address_mpu);
   
    hwlib::wait_ms(500); // smal wait timer so that the class can be processed
       data.start_up();

    hwlib::wait_ms(500);
        data.set_range_accel();
   

    for(;;){
        if(knop1.read() == 0){
            oled(data.accel_Spike2());
            hwlib::wait_ms(500);
        }
    }

/////////////////TEST///////////////////
// test cases
//data.test_accel();
//data.test_gyro();
//data.test_spike();
//data.read_registers(0x1C);
//hwlib::cout<<data.temprature();
////////////////////////////////////////
}









