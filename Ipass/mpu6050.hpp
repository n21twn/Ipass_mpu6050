#ifndef MPU6050_HPP
#define MPU6050_HPP


/// @file

///  @author Nick Teeuwen
///  @date  28/06/2021
/// \brief
/// Class Mpu6050
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
///   First we get the value of the Z axis(accel_Z) so we can determen our start point. Then we make a start point for the low value and the high value's
/// The low value gets - 1500 and the high value + 1500. So we have a cap at what point the acceleration is higher then the basic air pressure.
/// After all this we do the Start up function to wake the chip.
/// If that is done we start a for loop and first check if the value the given value from the chip is higher then our high_cap or lower then our low_cap.
/// So we can determen wich formule we need to use. After 1 of the 2 is true we quickly save the value of the current Z axis.
/// Then we do the z axis value / 16384(16384 = 1 g force ) so we get the force of the acceleration in g force after.
///  And at last we use the new g force value to calculate the speed in Km/H, 1 G  = 34 km/h so we mutiply our value with 34 and we put a ABS over it so the value is alway positive.
/// Extra Note: when we hold the chip horizontal there is already a output of a average of 1g on the chip because of gravity so it calulates that force also with the for of the impact.
       void speed_Test(){
                int16_t  high_cap= accel_Z() +1500 ;
                int16_t  low_cap = accel_Z() - 1500  ;
                hwlib::wait_ms(2000); 
               start_up();
               for(;;){
                   hwlib::wait_ms(200); 
                   if(accel_Z()> high_cap){
                       float tmp = accel_Z();
                        tmp = (tmp) /16384 ;    
                       int16_t high_value = tmp * 34;
                       hwlib::cout<<abs(high_value)<<" KM/h";
                   }
                   
                  if(accel_Z() < low_cap){
                      float tmp2 = accel_Z();
                       tmp2 = (tmp2) /16384 ;
                       int16_t low_value = tmp2 * 34;
                       hwlib::cout<<abs(low_value)<<" KM/h";
                   }
               }
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
#endif // MPU6050_HPP
