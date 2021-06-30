#ifndef MPU6050_HPP
#define MPU6050_HPP


/// @file

///  @author Nick Teeuwen
///  @date  28/06/2021
/// \brief
/// Class Mpu6 050
/// \details
/// This is a microcontroller with a accelerometer and a gyroscope on it so you can calculate linaere acceleration and rotating acceleration.
/// First we store the main addres of the Mpu6050 in the class and we store the i2c bus in the class(combined sda pin and scl pin).
/// The appropriate constructors and operators are provided.

class mpu6050{
private:
    uint16_t address;
    hwlib::i2c_bus & bus;

public:
        mpu6050( hwlib::i2c_bus & bus, uint16_t address):
        address(address),
        bus(bus)
        {}
        
/// \brief
/// Startup (no parameters)
/// \details
/// Writes 0 to 0x6B to wake up the mpu6050 in case it is in sleeping mode so we can communicate with the mpu6050.

        void start_up(){
    
        auto startup_write = bus.write(address);
            startup_write.write(0x6B);
            startup_write.write(0);
            
        }
/// \brief
/// Accelerometer  X axis(no parameters)
/// \details
///   writes the high X axis Accel  address to the chip so we can then read / saves the raw data  from the X axis. and saves the high X in a byte
/// Do the exact same with the low X accel as with the high X accel.
/// Combine the 2 bytes to a 16byte  instead of 8byte. The first byte gets shifted left because its the high byte.
/// The combined bytes are put in a variable and the variable is returned. Note: this is the raw data
        int16_t accel_X(){
            uint8_t first_byte;
            uint8_t second_byte;

            bus.write(address).write(0x3B);
            bus.read(address).read(first_byte);

            
            bus.write(address).write(0x3C);
            bus.read(address).read(second_byte);


           int16_t raw_data = (first_byte << 8) + second_byte;
           return raw_data;
        }
     
/// \brief
/// Accelerometer Y axis(no parameters)
/// \details
///   writes the high Y axis Accel  address to the chip so we can then read / saves the raw data  from the Y axis. and saves the high Y in a byte
/// Do the exact same with the low Y accel as with the high Y accel.
/// Combine the 2 bytes to a 16byte  instead of 8byte. The first byte gets shifted left because its the high byte.
/// The combined bytes are put in a variable and the variable is returned.Note: this is the raw data
   
       int16_t accel_Y(){
            uint8_t first_byte;
            uint8_t second_byte;

            bus.write(address).write(0x3D);
            bus.read(address).read(first_byte);
            
            bus.write(address).write(0x3E);
            bus.read(address).read(second_byte);

           int16_t raw_data = (first_byte << 8) + second_byte;
            return raw_data;
        }
        
/// \brief
/// Accelerometer Z axis(no parameters)
/// \details
///   writes the high Z axis Accel  address to the chip so we can then read / saves the raw data  from the Z axis. and saves the high Z in a byte
/// Do the exact same with the low Z accel as with the high Z accel.
/// Combine the 2 bytes to a 16byte  instead of 8byte. The first byte gets shifted left because its the high byte.
/// The combined bytes are put in a variable and the variable is returned. Note: this is the raw data
        
       int16_t accel_Z(){
            uint8_t first_byte;
            uint8_t second_byte;

            bus.write(address).write(0x3F);
            bus.read(address).read(first_byte);

            bus.write(address).write(0x40);
            bus.read(address).read(second_byte);

           int16_t raw_data = (first_byte << 8) + second_byte;
            return raw_data;
        }
/// \brief
/// Speed Test(no parameters)
/// \details
///   Get the value of Z axis with accel_Z and set it as startpoint. then set a startpoint for high and low values
/// The low value gets - 1500 and the high value + 1500. this is to prevent the airvibration to give us back value's.
///Then start_up()
/// Then loop until accel_Z higher or lower is then 1 of the 2 startpoints
/// if that is the case save the given value and return it
       int16_t speed_Test(){
                int16_t  normal_cap = accel_Z();
                int16_t  high_cap= normal_cap +1000 ;
                int16_t  low_cap = normal_cap - 1000  ;
                hwlib::wait_ms(2000); 
               start_up();

               for(;;){
                   if(accel_Z() > high_cap){
                       int16_t high_value = accel_Z() - normal_cap;
                       return abs(high_value);
                   }
                  if(accel_Z() < low_cap){
                      int16_t low_value = accel_Z() - normal_cap;
                      return abs(low_value);
                   }
                    hwlib::wait_ms(500); 
               }
               
        }
        
/// \brief
/// Smal test function 
/// \details
/// smal function to test if the axis give a value.
        void test_function(){
            if(accel_X() == 0){
                hwlib::cout<< "the X axis is not working";
                }
                 if(accel_Y() == 0){
                hwlib::cout<< "the Y axis is not working";
                }
                 if(accel_Z() == 0){
                hwlib::cout<< "the Z axis is not working";
                } 
		hwlib::cout<< "Test works if there are no further message's";           
            }
    };

// all registers for mpu gyro and accelerometer
//wake up = 0x6B
//
// accel X h = 0x3B;
//  accel X l = 0x3C;
//  accel Y h = 0x3D;
//  accel Y l = 0x3E;
//  accel Z h = 0x3F;
//  accel Z l = 0x40;
//
//  gyro X h    = 0x43;
//  gyro X l    = 0x44;
//  gyro Y h    = 0x45;
//  gyro Y l    = 0x46;
//  gyro Z h    = 0x47;
//  gyro Z l    = 0x48;


// defaulte 16384.0  accel
//deafaulte 131 gyro
#endif // MPU6050_HPP

