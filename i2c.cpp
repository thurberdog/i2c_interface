#include "i2c.hpp"

i2c::i2c(QObject *parent) : QObject(parent)
{
    initiateI2C();
    openI2C();
    sensors_ADC_init();
    writeI2C();
    readI2C();

}

void i2c::sensors_ADC_init(void) {
     addr = 0b00101001;          // The I2C address of the ADC
    sprintf(filename,"/dev/i2c-2");
    if ((file = open(filename,O_RDWR)) < 0) {
        printf("Failed to open the bus.");
        /* ERROR HANDLING; you can check errno to see what went wrong */
        exit(1);
    }
    if (ioctl(file,I2C_SLAVE,addr) < 0) {
        printf("Failed to acquire bus access and/or talk to slave.\n");
        /* ERROR HANDLING; you can check errno to see what went wrong */
        exit(1);
    }

    for(int i = 0; i<4; i++) {
        // Using I2C Read
        if (read(file,buf,2) != 2) {
            /* ERROR HANDLING: i2c transaction failed */
            printf("Failed to read from the i2c bus.\n");
            buffer = g_strerror(errno);
            printf(buffer);
            printf("\n\n");
        } else {
            data = (float)((buf[0] & 0b00001111)<<8)+buf[1];
            data = data/4096*5;
            channel = ((buf[0] & 0b00110000)>>4);
            printf("Channel %02d Data:  %04f\n",channel,data);
        }
    }
    //unsigned char reg = 0x10; // Device register to access
    //buf[0] = reg;
    buf[0] = 0b11110000;

    if (write(file,buf,1) != 1) {
        /* ERROR HANDLING: i2c transaction failed */
        printf("Failed to write to the i2c bus.\n");
        buffer = g_strerror(errno);
        printf(buffer);
        printf("\n\n");
    }
}

void i2c::writeI2C(){
//
//    The write system call is used to obtain data from the I²C peripheral.
//    Write requires a file handle, a buffer in which the data is stored, and a number of bytes to write.
//    Write will attempt to write the number of bytes specified and will return the actual number of bytes written,
//    which can be used to detect errors.
//    Some devices require an internal address to be sent prior to the data to specify the register on the external device to access.
    //unsigned char reg = 0x10; // Device register to access
    //buf[0] = reg;

    buf[0] = 0b11110000;
    if (write(file,buf,1) != 1) {
        /* ERROR HANDLING: i2c transaction failed */
        printf("Failed to write to the i2c bus.\n");
        buffer = g_strerror(errno);
        printf(buffer);
        printf("\n\n");
    }
}

void i2c::readI2C(){
//
//    The read system call is used to obtain data from the I²C peripheral.
//    Read requires a file handle, a buffer to store the data,
//    and a number of bytes to read.
//    Read will attempt to read the number of bytes specified and will return the actual number of bytes read,
//    which can be used to detect errors.
//    The code in the else block below calculates the voltage present at the ADC pin assuming a 5 volt reference/supply voltage.

    for (int i = 0; i<4; i++) {
        // Using I2C Read
        if (read(file,buf,2) != 2) {
            /* ERROR HANDLING: i2c transaction failed */
            printf("Failed to read from the i2c bus.\n");
            buffer = g_strerror(errno);
            printf(buffer);
            printf("\n\n");
        } else {
            data = (float)((buf[0] & 0b00001111)<<8)+buf[1];
            data = data/4096*5;
            channel = ((buf[0] & 0b00110000)>>4);
            printf("Channel %02d Data:  %04f\n",channel,data);
        }
    }

}

void i2c::initiateI2C() {
//
//    After successfully acquiring bus access,
//    you must initiate communication with whatever peripheral you are attempting to utilize.
//    I²C does this by sending out the seven bit address of the device followed by a read/write bit.
//    The bit is set to 0 for writes and 1 for reads.
//    This is another common failure point,
//    as manufacturers tend to report the I²C address of the peripheral in a variety of ways.
//    Some report the address as a seven bit number,
//    meaning that the address must be shifted left by a bit and then have the r/w bit tacked onto the end.
//    Others will provide it as an eight bit number and assume you will set the last bit accordingly.
//    Although a few manufacturers actually say which method they use to describe the address,
//    the vast majority do not, and the user may have to resort to testing via trial and error.


    addr = 0b00101001;          // The I2C address of the ADC
    if (ioctl(file, I2C_SLAVE, addr) < 0) {
        printf("Failed to acquire bus access and/or talk to slave.\n");
        /* ERROR HANDLING; you can check errno to see what went wrong */
        exit(1);
    }
}

void i2c::openI2C(){
// In order to communicate with an I²C peripheral with this simple structure,
// you must first open the bus for reading and writing like you would any file.
// A call to open must be used rather than fopen so that writes to the bus are not buffered.
// Open returns a new file descriptor (a non-negative integer) which can then be used to configure the bus.
// A typical reason for failure at this stage is a lack of permissions to access /dev/i2c-2.
// Adding the user to a group which has permissions to access the file will alleviate this problem,
// as will adjusting the file permissions to enable user access.
// Adding a udev rule to set the I²C device group is the most permanent solution.
    int file;
    const char *filename = "/dev/i2c-2";
    if ((file = open(filename, O_RDWR)) < 0) {
        /* ERROR HANDLING: you can check errno to see what went wrong */
        perror("Failed to open the i2c bus");
        exit(1);
    }
}
