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
    
    uint8_t wakeup = 0x6B;
    uint8_t reset = 0x68;
    
    uint8_t accel_conf = 0x1C;
    uint8_t accel_2g = 0b00000;
    uint8_t accel_4g = 0b01000;
    uint8_t accel_8g = 0b10000;
    uint8_t accel_16g = 0b11000;
    
    uint8_t accelXh = 0x3B;
    uint8_t accelXl = 0x3C;
    uint8_t accelYh = 0x3D;
    uint8_t accelYl = 0x3E;
    uint8_t accelZh = 0x3F;
    uint8_t accelZl = 0x40;
    
    uint8_t gyroXh = 0x43;
    uint8_t gyroXl = 0x44;
    uint8_t gyroYh = 0x45;
    uint8_t gyroYl = 0x46;
    uint8_t gyroZh = 0x47;
    uint8_t gyroZl = 0x48;
    
    uint8_t temp_h = 0x41;
    uint8_t temp_l = 0x42;
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
            startup_write.write(wakeup);
            startup_write.write(0x00);
        }
/// \brief
/// set range for accelerometer
/// \details
/// write's to accelerometer configuration writes 0b11000 = 16g to register 
        void set_range_accel(){
             auto setrange_write = bus.write(address);
            setrange_write.write(accel_conf);
            setrange_write.write(accel_16g);
            }
            
            
            
/// \brief
/// Accelerometer  X axis(no parameters)
/// \details
///   writes the high X axis Accel  address to the chip so we can then read / saves the raw data  from the X axis. and saves the high X in a byte
/// Do the exact same with the low X accel as with the high X accel.
/// Combine the 2 bytes to a 16byte  instead of 8byte. The first byte gets shifted left because its the high byte. and return the byte.
/// !! Note: this is the raw data
        int16_t accel_X(){
            uint8_t first_byte;
            uint8_t second_byte;

            bus.write(address).write(accelXh);
            bus.read(address).read(first_byte);

            
            bus.write(address).write(accelXl);
            bus.read(address).read(second_byte);


           int16_t raw_data = (first_byte << 8) + second_byte;
           return raw_data;
        }
     
/// \brief
/// Accelerometer Y axis(no parameters)
/// \details
///   writes the high Y axis Accel  address to the chip so we can then read / saves the raw data  from the Y axis. and saves the high Y in a byte
/// Do the exact same with the low Y accel as with the high Y accel.
/// Combine the 2 bytes to a 16byte  instead of 8byte. The first byte gets shifted left because its the high byte.and returns the byte
/// !! Note: this is the raw data
   
       int16_t accel_Y(){
            uint8_t first_byte;
            uint8_t second_byte;

            bus.write(address).write(accelYh);
            bus.read(address).read(first_byte);
            
            bus.write(address).write(accelYl);
            bus.read(address).read(second_byte);

           int16_t raw_data = (first_byte << 8) + second_byte;
            return raw_data;
        }
        
/// \brief
/// Accelerometer Z axis(no parameters)
/// \details
///   writes the high Z axis Accel  address to the chip so we can then read / saves the raw data  from the Z axis. and saves the high Z in a byte
/// Do the exact same with the low Z accel as with the high Z accel.
/// Combine the 2 bytes to a 16byte  instead of 8byte. The first byte gets shifted left because its the high byte. and returns the byte
/// !! Note: this is the raw data
        
       int16_t accel_Z(){
            uint8_t first_byte;
            uint8_t second_byte;

            bus.write(address).write(accelZh);
            bus.read(address).read(first_byte);

            bus.write(address).write(accelZl);
            bus.read(address).read(second_byte);

           int16_t raw_data = (first_byte << 8) + second_byte;
            return raw_data;
        }
        
        

/// \brief
/// Accelerometer spike 
/// \details
///  checks if the value given bij accel_Z() are higher then there base value + 20% if no continu to loop.
/// if yes check if the accel_Z() value is higher then the highst_value so whe can add a new highest value.
/// after there no higher value any more then accel_Z() + 20% return the highest_value given

       
       int16_t accel_Spike2(){
            int16_t spike_cap = abs(accel_Z()) * 1.20;
            int16_t highst_value = 0;

            while(highst_value == 0){
                
                while(abs(accel_Z())  > spike_cap){
                    if(abs(accel_Z()) > highst_value ){
                       highst_value = abs(accel_Z());
                    }
                }
            }
           hwlib::cout<<highst_value<<"\n";
            return highst_value;
    }
       
        
/// \brief
/// reading registers(uint8_t)
/// \details
///  reads the given register hexidecimaal so you can check if voor example you changed your range on the gyro
        void read_registers(uint8_t given_rgst){
            uint8_t recieving_rgst = given_rgst;
            uint8_t result_rgst;
            bus.write(address).write(recieving_rgst);
            bus.read(address).read(result_rgst);
            hwlib::cout<<result_rgst;
            }
        
/// \brief
///Test accelerometer
/// \details
/// smal function to test if the axis react to tilting the chip and giving value's
        void test_accel(){
            for(;;){
                hwlib::cout<< "Z:"<<accel_Z();
                hwlib::cout<<" X:"<< accel_X();
                hwlib::cout<<" Y:"<< accel_Y()<<"\n";
                hwlib::wait_ms(500);
            }
        }
/// \brief
/// Test spike 
/// \details
/// a test function that loops the accel_Z() function to see if it's value is 20% higher then his base value
/// print all value's that are higher then that 20%
            void test_spike(){
                for(;;){
                    int16_t spike_cap = abs(accel_Z()) * 1.20;
                    hwlib::wait_ms(10);
                        if(abs(accel_Z())  > spike_cap){  
                            hwlib::cout<<accel_Z()<<"   ";
                    }
                }
            }
            
            
        
/// \brief
///Test accelerometer
/// \details
/// smal function to test if the gyro react to tilting the chip and giving value's
        void test_gyro(){
            for(;;){
                hwlib::cout<< "Z:"<<gyro_Z();
                hwlib::cout<<" X:"<<gyro_X();
                hwlib::cout<<" Y:"<< gyro_Y()<<"\n";
                hwlib::wait_ms(500);
            }
        }
/// \brief
/// gyro Z axis(no parameters)
/// \details
/// write's to the gyro Z registers. read the given data.
/// add both 8 bit values with each other for a 16 bit variable.
/// return the 16 bit data
///!! Note: this is the raw data            
        int16_t gyro_Z(){
            uint8_t first_byte;
            uint8_t second_byte;

            bus.write(address).write(gyroZh);
            bus.read(address).read(first_byte);

            bus.write(address).write(gyroZl);
            bus.read(address).read(second_byte);

           int16_t raw_data = (first_byte << 8) + second_byte;
            return raw_data;
        }
/// \brief
/// gyro Y axis(no parameters)
/// \details
/// write's to the gyro Y registers. read the given data.
/// add both 8 bit values with each other for a 16 bit variable.
/// return the 16 bit data
///!! Note: this is the raw data  
        int16_t gyro_Y(){
            uint8_t first_byte;
            uint8_t second_byte;

            bus.write(address).write(gyroYh);
            bus.read(address).read(first_byte);

            bus.write(address).write(gyroYl);
            bus.read(address).read(second_byte);

           int16_t raw_data = (first_byte << 8) + second_byte;
            return raw_data;
        }
        
/// \brief
/// gyro X axis(no parameters)
/// \details
/// write's to the gyro X registers. read the given data.
/// add both 8 bit values with each other for a 16 bit variable.
/// return the 16 bit data
///!! Note: this is the raw data  
        int16_t gyro_X(){
            uint8_t first_byte;
            uint8_t second_byte;

            bus.write(address).write(gyroXh);
            bus.read(address).read(first_byte);

            bus.write(address).write(gyroXl);
            bus.read(address).read(second_byte);

           int16_t raw_data = (first_byte << 8) + second_byte;
            return raw_data;
        }

/// \brief
/// temprature(no parameters)
/// \details
/// write's to the temp registers. read the given data.
/// add both 8 bit values with each other for a 16 bit variable.
/// return the 16 bit data / 340 +36.53 for the Celsuis

        int16_t temprature(){
            uint8_t first_byte;
            uint8_t second_byte;

            bus.write(address).write(temp_h);
            bus.read(address).read(first_byte);

            bus.write(address).write(temp_l);
            bus.read(address).read(second_byte);

           int16_t raw_data = (first_byte << 8) + second_byte;
            return (raw_data/340) + 36.53;
        }
        
    };
#endif // MPU6050_HPP

