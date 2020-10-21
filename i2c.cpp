#include "i2c.hpp"

/**
 * @brief i2c::i2c
 * @param parent
 */
i2c::i2c(QObject *parent) : QObject(parent)
{
    qDebug()<<__FUNCTION__<<__LINE__<<"I2C";
//    400kHz, allowing us to use the fastest data rates.
//    The I2C port is designed to be operated at speeds up to
//    400kHz. It has built-in timing delays to ensure correct
//    operation when addressed from an I2C compliant master
//    device. It also contains input filters designed to suppress
//    glitches should the bus become corrupted.
    frequency(400000);
    // initialize the BW data rate
    char tx[2];
    tx[0] = ADXL345_BW_RATE_REG;
    tx[1] = ADXL345_1600HZ;
    //value greater than or equal to 0x0A is written into the rate bits (Bit D3 through Bit D0) in the BW_RATE register
    writeI2C( ADXL345_I2C_WRITE , tx, 2);
//    Data format (for +-16g) - This is done by setting Bit D3 of the DATA_FORMAT register (Address 0x31)
//    and writing a value of 0x03 to the range bits (Bit D1 and Bit D0) of the DATA_FORMAT register (Address 0x31).
    char rx[2];
    rx[0] = ADXL345_DATA_FORMAT_REG;
    rx[1] = 0x0B;
    // full res and +_16g
    writeI2C( ADXL345_I2C_WRITE , rx, 2);
    // Set Offset  - programmed into the OFSX, OFSY, and OFXZ registers, respectively, as 0xFD, 0x03 and 0xFE.
    char x[2];
    x[0] = ADXL345_OFSX_REG ;
    x[1] = 0xFD;
    writeI2C( ADXL345_I2C_WRITE , x, 2);
    char y[2];
    y[0] = ADXL345_OFSY_REG ;
    y[1] = 0x03;
    writeI2C( ADXL345_I2C_WRITE , y, 2);
    char z[2];
    z[0] = ADXL345_OFSZ_REG ;
    z[1] = 0xFE;
    writeI2C( ADXL345_I2C_WRITE , z, 2);
}

/**
 * @brief i2c::sensors_ADXL_init
 * @param address
 * @param buffer
 * @param length
 */
void i2c::sensors_ADXL345_init(int address, const gchar *buffer, int length) {
    qDebug()<<__FUNCTION__<<__LINE__<<"I2C"<<address<<buffer;
     addr = address;          // The I2C address of the ADC
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
        if (read(file,buf,length) != length) {
            /* ERROR HANDLING: i2c transaction failed */
            printf("Failed to read from the i2c bus.\n");
            buffer = g_strerror(errno);
            printf("%s", buffer);
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
    buf[0] = address;

    if (write(file,buf,length) != length) {
        /* ERROR HANDLING: i2c transaction failed */
        printf("Failed to write to the i2c bus.\n");
        buffer = g_strerror(errno);
        printf("%s", buffer);
        printf("\n\n");
    }
}

/**
  * @brief i2c::writeI2C
  * @param address
  * @param buffer
  * @param length
  * @return
  */
 int i2c::writeI2C(int address, const gchar *buffer, int length){
//
//    The write system call is used to obtain data from the I²C peripheral.
//    Write requires a file handle, a buffer in which the data is stored, and a number of bytes to write.
//    Write will attempt to write the number of bytes specified and will return the actual number of bytes written,
//    which can be used to detect errors.
//    Some devices require an internal address to be sent prior to the data to specify the register on the external device to access.
    //unsigned char reg = 0x10; // Device register to access
    //buf[0] = reg;
 qDebug()<<__FUNCTION__<<__LINE__<<"I2C"<<address<<buffer;
    buf[0] = address;
    if (write(file,buf,length) != length) {
        /* ERROR HANDLING: i2c transaction failed */
        printf("Failed to write to the i2c bus.\n");
        buffer = g_strerror(errno);
        printf("%s", buffer);
        printf("\n\n");
    }
    /** Write single byte out on the I2C bus
     *
     *  @param data data to write out on bus
     *
     *  @returns
     *    '1' if an ACK was received,
     *    '0' otherwise
     */
    return 0;
}

/**
 * @brief i2c::readI2C
 * @param address
 * @param buffer
 * @param length
 */
void i2c::readI2C(int address, const gchar *buffer, int length){
//
//    The read system call is used to obtain data from the I²C peripheral.
//    Read requires a file handle, a buffer to store the data,
//    and a number of bytes to read.
//    Read will attempt to read the number of bytes specified and will return the actual number of bytes read,
//    which can be used to detect errors.
//    The code in the else block below calculates the voltage present at the ADC pin assuming a 5 volt reference/supply voltage.
 qDebug()<<__FUNCTION__<<__LINE__<<"I2C"<<address<<buffer;
buf[0] = address;
    for (int i = 0; i<4; i++) {
        // Using I2C Read
        if (read(file,buf,length) != length) {
            /* ERROR HANDLING: i2c transaction failed */
            printf("Failed to read from the i2c bus.\n");
            buffer = g_strerror(errno);
            printf("%s", buffer);
            printf("\n\n");
        } else {
            data = (float)((buf[0] & 0b00001111)<<8)+buf[1];
            data = data/4096*5;
            channel = ((buf[0] & 0b00110000)>>4);
            printf("Channel %02d Data:  %04f\n",channel,data);
        }
    }

}

/**
 * @brief i2c::initiateI2C
 * @param address
 */
void i2c::initiateI2C(int address) {
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

 qDebug()<<__FUNCTION__<<__LINE__<<"I2C"<<address;
    addr = address;          // The I2C address of the ADXL
    if (ioctl(file, I2C_SLAVE, addr) < 0) {
        printf("Failed to acquire bus access and/or talk to slave.\n");
        /* ERROR HANDLING; you can check errno to see what went wrong */
        exit(1);
    }
}

/**
 * @brief i2c::openI2C
 */
void i2c::openI2C(){
// In order to communicate with an I²C peripheral with this simple structure,
// you must first open the bus for reading and writing like you would any file.
// A call to open must be used rather than fopen so that writes to the bus are not buffered.
// Open returns a new file descriptor (a non-negative integer) which can then be used to configure the bus.
// A typical reason for failure at this stage is a lack of permissions to access /dev/i2c-2.
// Adding the user to a group which has permissions to access the file will alleviate this problem,
// as will adjusting the file permissions to enable user access.
// Adding a udev rule to set the I²C device group is the most permanent solution.
 qDebug()<<__FUNCTION__<<__LINE__<<"I2C open";
    int file;
    const char *filename = "/dev/i2c-2";
    if ((file = open(filename, O_RDWR)) < 0) {
        /* ERROR HANDLING: you can check errno to see what went wrong */
        perror("Failed to open the i2c bus");
        exit(1);
    }
}

/** Set the frequency of the I2C interface
 * @brief i2c::frequency
 * @param hz The bus frequency in hertz
 */

void i2c::frequency(int hz){
 qDebug()<<__FUNCTION__<<__LINE__<<"I2C"<<hz;
}

/**
 * @brief i2c::SingleByteRead
 * @param address
 * @return
 */
char i2c::ADXL345_SingleByteRead(char address){
// ADXL343
// MASTER ->| Slave Address + Write |     | Register Address |     | Slave Address + Read |              | NACK |
// SLAVE  <-|                       | ACK |                  | ACK |                      | ACK | | Data |
     qDebug()<<__FUNCTION__<<__LINE__<<"I2C"<<address;
    char tx = address;
    char output = '\0';
    writeI2C( ADXL345_I2C_WRITE , &tx, 1);  //tell it what you want to read
    readI2C( ADXL345_I2C_READ , &output, 1);    //tell it where to store the data
    qDebug()<<__FUNCTION__<<__LINE__<<"I2C"<<output;
    return output;

}

/**
 * @brief i2c::SingleByteWrite
 * @param address
 * @param data
 * @return
 */
int i2c::ADXL345_SingleByteWrite(char address, char data){
// ADXL343
// MASTER ->| Slave Address + Write |     | Register Address |     | Data |
// SLAVE  <-|                       | ACK |                  | ACK |      | ACK |
    qDebug()<<__FUNCTION__<<__LINE__<<"I2C"<<address<<data;
    int ack = 0;
    char tx[2];
    tx[0] = address;
    tx[1] = data;
    return   ack | writeI2C( ADXL345_I2C_WRITE , tx, 2);
}

/**
 * @brief i2c::multiByteRead
 * @param address
 * @param output
 * @param size
 */
void i2c::ADXL345_multiByteRead(char address, char* output, int size) {
// ADXL343
// MASTER ->| Slave Address + Write |     | Register Address |     | Slave Address + Read |              | ACK |
// SLAVE  <-|                       | ACK |                  | ACK |                      | ACK | | Data |     | Data |
    writeI2C( ADXL345_I2C_WRITE, &address, 1);  //tell it where to read from
    readI2C( ADXL345_I2C_READ , output, size);      //tell it where to store the data read
    qDebug()<<__FUNCTION__<<__LINE__<<"I2C"<<address<<output;
}

/**
 * @brief i2c::multiByteWrite
 * @param address
 * @param ptr_data
 * @param size
 * @return
 */
int i2c::ADXL345_multiByteWrite(char address, char* ptr_data, int size) {
// ADXL343
// MASTER ->| Slave Address + Write |     | Register Address |     | Data |     | Data |
// SLAVE  <-|                       | ACK |                  | ACK |      | ACK |
    int ack;
    qDebug()<<__FUNCTION__<<__LINE__<<"I2C"<<address<<ptr_data;
    ack = writeI2C( ADXL345_I2C_WRITE, &address, 1);  //tell it where to write to
    return ack | writeI2C( ADXL345_I2C_READ, ptr_data, size);  //tell it what data to write

}

/**
 * @brief i2c::getOutput
 * @param readings
 */
void i2c::get_ADXL345_Output(int* readings){
    char buffer[6];
    ADXL345_multiByteRead(ADXL345_DATAX0_REG, buffer, 6);

    readings[0] = (int)buffer[1] << 8 | (int)buffer[0];
    readings[1] = (int)buffer[3] << 8 | (int)buffer[2];
    readings[2] = (int)buffer[5] << 8 | (int)buffer[4];
    qDebug()<<__FUNCTION__<<__LINE__<<"I2C"<<buffer;

}

/**
 * @brief i2c::getDeviceID
 * @return
 */
char i2c::get_ADXL345_DeviceID() {
    qDebug()<<__FUNCTION__<<__LINE__<<"I2C";
    return ADXL345_SingleByteRead(ADXL345_DEVID_REG);
}

/**
 * @brief i2c::setPowerMode
 * @param mode
 * @return
 */
int i2c::set_ADXL345_PowerMode(char mode) {
 qDebug()<<__FUNCTION__<<__LINE__<<"I2C"<<mode;
    //Get the current register contents, so we don't clobber the rate value.
    char registerContents = (mode << 4) | ADXL345_SingleByteRead(ADXL345_BW_RATE_REG);

    return ADXL345_SingleByteWrite(ADXL345_BW_RATE_REG, registerContents);

}

/**
 * @brief i2c::getPowerControl
 * @return
 */
char i2c::get_ADXL345_PowerControl() {
    qDebug()<<__FUNCTION__<<__LINE__<<"I2C";
    return ADXL345_SingleByteRead(ADXL345_POWER_CTL_REG);
}

/**
 * @brief i2c::setPowerControl
 * @param settings
 * @return
 */
int i2c::set_ADXL345_PowerControl(char settings) {
    qDebug()<<__FUNCTION__<<__LINE__<<"I2C"<<settings;
    return ADXL345_SingleByteWrite(ADXL345_POWER_CTL_REG, settings);

}

/**
 * @brief i2c::getDataFormatControl
 * @return
 */
char i2c::get_ADXL345_DataFormatControl(void){
 qDebug()<<__FUNCTION__<<__LINE__<<"I2C";
    return ADXL345_SingleByteRead(ADXL345_DATA_FORMAT_REG);
}

/**
 * @brief i2c::setDataFormatControl
 * @param settings
 * @return
 */
int i2c::set_ADXL345_DataFormatControl(char settings){
 qDebug()<<__FUNCTION__<<__LINE__<<"I2C"<<settings;
    return ADXL345_SingleByteWrite(ADXL345_DATA_FORMAT_REG, settings);

}

/**
 * @brief i2c::setDataRate
 * @param rate
 * @return
 */
int i2c::set_ADXL345_DataRate(char rate) {
 qDebug()<<__FUNCTION__<<__LINE__<<"I2C"<<rate;
    //Get the current register contents, so we don't clobber the power bit.
    char registerContents = ADXL345_SingleByteRead(ADXL345_BW_RATE_REG);

    registerContents &= 0x10;
    registerContents |= rate;

    return ADXL345_SingleByteWrite(ADXL345_BW_RATE_REG, registerContents);

}

/**
 * @brief i2c::getOffset
 * @param axis
 * @return
 */
char i2c::get_ADXL345_Offset(char axis) {
 qDebug()<<__FUNCTION__<<__LINE__<<"I2C"<<axis;
    char address = 0;

    if (axis == ADXL345_X) {
        address = ADXL345_OFSX_REG;
    } else if (axis == ADXL345_Y) {
        address = ADXL345_OFSY_REG;
    } else if (axis == ADXL345_Z) {
        address = ADXL345_OFSZ_REG;
    }

    return ADXL345_SingleByteRead(address);
}

/**
 * @brief i2c::setOffset
 * @param axis
 * @param offset
 * @return
 */
int i2c::set_ADXL345_Offset(char axis, char offset) {
 qDebug()<<__FUNCTION__<<__LINE__<<"I2C"<<axis;
    char address = 0;

    if (axis == ADXL345_X) {
        address = ADXL345_OFSX_REG;
    } else if (axis == ADXL345_Y) {
        address = ADXL345_OFSY_REG;
    } else if (axis == ADXL345_Z) {
        address = ADXL345_OFSZ_REG;
    }

    return ADXL345_SingleByteWrite(address, offset);

}

/**
 * @brief i2c::getFifoControl
 * @return
 */
char i2c::get_ADXL345_FifoControl(void){
 qDebug()<<__FUNCTION__<<__LINE__<<"I2C";
    return ADXL345_SingleByteRead(ADXL345_FIFO_CTL);

}

/**
 * @brief i2c::setFifoControl
 * @param settings
 * @return
 */
int i2c::set_ADXL345_FifoControl(char settings){
     qDebug()<<__FUNCTION__<<__LINE__<<"I2C"<<settings;
    return ADXL345_SingleByteWrite(ADXL345_FIFO_STATUS, settings);

}

/**
 * @brief i2c::getFifoStatus
 * @return
 */
char i2c::get_ADXL345_FifoStatus(void){
 qDebug()<<__FUNCTION__<<__LINE__<<"I2C";
    return ADXL345_SingleByteRead(ADXL345_FIFO_STATUS);

}


/**
 * @brief i2c::getTapThreshold
 * @return
 */
char i2c::get_ADXL345_TapThreshold(void) {
 qDebug()<<__FUNCTION__<<__LINE__<<"I2C";
    return ADXL345_SingleByteRead(ADXL345_THRESH_TAP_REG);
}

/**
 * @brief i2c::setTapThreshold
 * @param threshold
 * @return
 */
int i2c::set_ADXL345_TapThreshold(char threshold) {
 qDebug()<<__FUNCTION__<<__LINE__<<"I2C"<<threshold;
    return ADXL345_SingleByteWrite(ADXL345_THRESH_TAP_REG, threshold);

}

/**
 * @brief i2c::getTapDuration
 * @return
 */
float i2c::get_ADXL345_TapDuration(void) {
 qDebug()<<__FUNCTION__<<__LINE__<<"I2C";
    return (float)ADXL345_SingleByteRead(ADXL345_DUR_REG)*625;
}

/**
 * @brief i2c::setTapDuration
 * @param duration_us
 * @return
 */
int i2c::set_ADXL345_TapDuration(short int duration_us) {
 qDebug()<<__FUNCTION__<<__LINE__<<"I2C"<<duration_us;
    short int tapDuration = duration_us / 625;
    char tapChar[2];
    tapChar[0] = (tapDuration & 0x00FF);
    tapChar[1] = (tapDuration >> 8) & 0x00FF;
    return ADXL345_multiByteWrite(ADXL345_DUR_REG, tapChar, 2);

}

/**
 * @brief i2c::getTapLatency
 * @return
 */
float i2c::get_ADXL345_TapLatency(void) {
 qDebug()<<__FUNCTION__<<__LINE__<<"I2C";
    return (float)ADXL345_SingleByteRead(ADXL345_LATENT_REG)*1.25;
}

/**
 * @brief i2c::setTapLatency
 * @param latency_ms
 * @return
 */
int i2c::set_ADXL345_TapLatency(short int latency_ms) {
 qDebug()<<__FUNCTION__<<__LINE__<<"I2C"<<latency_ms;
    latency_ms = latency_ms / 1.25;
    char latChar[2];
    latChar[0] = (latency_ms & 0x00FF);
    latChar[1] = (latency_ms << 8) & 0xFF00;
    return ADXL345_multiByteWrite(ADXL345_LATENT_REG, latChar, 2);

}

/**
 * @brief i2c::getWindowTime
 * @return
 */
float i2c::get_ADXL345_WindowTime(void) {
 qDebug()<<__FUNCTION__<<__LINE__<<"I2C";
    return (float)ADXL345_SingleByteRead(ADXL345_WINDOW_REG)*1.25;
}

/**
 * @brief i2c::setWindowTime
 * @param window_ms
 * @return
 */
int i2c::set_ADXL345_WindowTime(short int window_ms) {
 qDebug()<<__FUNCTION__<<__LINE__<<"I2C"<<window_ms;
    window_ms = window_ms / 1.25;
    char windowChar[2];
    windowChar[0] = (window_ms & 0x00FF);
    windowChar[1] = ((window_ms << 8) & 0xFF00);
    return ADXL345_multiByteWrite(ADXL345_WINDOW_REG, windowChar, 2);

}

/**
 * @brief i2c::getActivityThreshold
 * @return
 */
char i2c::get_ADXL345_ActivityThreshold(void) {
 qDebug()<<__FUNCTION__<<__LINE__<<"I2C";
    return ADXL345_SingleByteRead(ADXL345_THRESH_ACT_REG);
}

/**
 * @brief i2c::setActivityThreshold
 * @param threshold
 * @return
 */
int i2c::set_ADXL345_ActivityThreshold(char threshold) {
     qDebug()<<__FUNCTION__<<__LINE__<<"I2C"<<threshold;
    return ADXL345_SingleByteWrite(ADXL345_THRESH_ACT_REG, threshold);

}

/**
 * @brief i2c::getInactivityThreshold
 * @return
 */
char i2c::get_ADXL345_InactivityThreshold(void) {
     qDebug()<<__FUNCTION__<<__LINE__<<"I2C";
    return ADXL345_SingleByteRead(ADXL345_THRESH_INACT_REG);

}

/**
 * @brief i2c::setInactivityThreshold
 * @param threshold
 * @return
 */
int i2c::set_ADXL345_InactivityThreshold(char threshold) {
     qDebug()<<__FUNCTION__<<__LINE__<<"I2C"<<threshold;
    return ADXL345_SingleByteWrite(ADXL345_THRESH_INACT_REG, threshold);

}

/**
 * @brief i2c::getTimeInactivity
 * @return
 */
char i2c::get_ADXL345_TimeInactivity(void) {
 qDebug()<<__FUNCTION__<<__LINE__<<"I2C";
    return ADXL345_SingleByteRead(ADXL345_TIME_INACT_REG);

}

/**
 * @brief i2c::setTimeInactivity
 * @param timeInactivity
 * @return
 */
int i2c::set_ADXL345_TimeInactivity(char timeInactivity) {
     qDebug()<<__FUNCTION__<<__LINE__<<"I2C"<<timeInactivity;
    return ADXL345_SingleByteWrite(ADXL345_TIME_INACT_REG, timeInactivity);

}

/**
 * @brief i2c::getActivityInactivityControl
 * @return
 */
char i2c::get_ADXL345_ActivityInactivityControl(void) {
 qDebug()<<__FUNCTION__<<__LINE__<<"I2C";
    return ADXL345_SingleByteRead(ADXL345_ACT_INACT_CTL_REG);

}

/**
 * @brief i2c::setActivityInactivityControl
 * @param settings
 * @return
 */
int i2c::set_ADXL345_ActivityInactivityControl(char settings) {
     qDebug()<<__FUNCTION__<<__LINE__<<"I2C"<<settings;
    return ADXL345_SingleByteWrite(ADXL345_ACT_INACT_CTL_REG, settings);

}

/**
 * @brief i2c::getFreefallThreshold
 * @return
 */
char i2c::get_ADXL345_FreefallThreshold(void) {
 qDebug()<<__FUNCTION__<<__LINE__<<"I2C";
    return ADXL345_SingleByteRead(ADXL345_THRESH_FF_REG);

}

/**
 * @brief i2c::setFreefallThreshold
 * @param threshold
 * @return
 */
int i2c::set_ADXL345_FreefallThreshold(char threshold) {
     qDebug()<<__FUNCTION__<<__LINE__<<"I2C"<<threshold;
    return ADXL345_SingleByteWrite(ADXL345_THRESH_FF_REG, threshold);

}

/**
 * @brief i2c::getFreefallTime
 * @return
 */
char i2c::get_ADXL345_FreefallTime(void) {
 qDebug()<<__FUNCTION__<<__LINE__<<"I2C";
    return ADXL345_SingleByteRead(ADXL345_TIME_FF_REG)*5;

}

/**
 * @brief i2c::setFreefallTime
 * @param freefallTime_ms
 * @return
 */
int i2c::set_ADXL345_FreefallTime(short int freefallTime_ms) {
     qDebug()<<__FUNCTION__<<__LINE__<<"I2C"<<freefallTime_ms;
    freefallTime_ms = freefallTime_ms / 5;
    char fallChar[2];
    fallChar[0] = (freefallTime_ms & 0x00FF);
    fallChar[1] = (freefallTime_ms << 8) & 0xFF00;

    return ADXL345_multiByteWrite(ADXL345_TIME_FF_REG, fallChar, 2);

}

/**
 * @brief i2c::getTapAxisControl
 * @return
 */
char i2c::get_ADXL345_TapAxisControl(void) {
 qDebug()<<__FUNCTION__<<__LINE__<<"I2C";
    return ADXL345_SingleByteRead(ADXL345_TAP_AXES_REG);

}

/**
 * @brief i2c::setTapAxisControl
 * @param settings
 * @return
 */
int i2c::set_ADXL345_TapAxisControl(char settings) {
     qDebug()<<__FUNCTION__<<__LINE__<<"I2C"<<settings;
    return ADXL345_SingleByteWrite(ADXL345_TAP_AXES_REG, settings);

}

/**
 * @brief i2c::getTapSource
 * @return
 */
char i2c::get_ADXL345_TapSource(void) {
 qDebug()<<__FUNCTION__<<__LINE__<<"I2C";
    return ADXL345_SingleByteRead(ADXL345_ACT_TAP_STATUS_REG);

}


/**
 * @brief i2c::getInterruptEnableControl
 * @return
 */
char i2c::get_ADXL345_InterruptEnableControl(void) {
 qDebug()<<__FUNCTION__<<__LINE__<<"I2C";
    return ADXL345_SingleByteRead(ADXL345_INT_ENABLE_REG);

}

/**
 * @brief i2c::setInterruptEnableControl
 * @param settings
 * @return
 */
int i2c::set_ADXL345_InterruptEnableControl(char settings) {
     qDebug()<<__FUNCTION__<<__LINE__<<"I2C"<<settings;
    return ADXL345_SingleByteWrite(ADXL345_INT_ENABLE_REG, settings);

}

/**
 * @brief i2c::getInterruptMappingControl
 * @return
 */
char i2c::get_ADXL345_InterruptMappingControl(void) {
 qDebug()<<__FUNCTION__<<__LINE__<<"I2C";
    return ADXL345_SingleByteRead(ADXL345_INT_MAP_REG);

}

/**
 * @brief i2c::setInterruptMappingControl
 * @param settings
 * @return
 */
int i2c::set_ADXL345_InterruptMappingControl(char settings) {
     qDebug()<<__FUNCTION__<<__LINE__<<"I2C"<<settings;
    return ADXL345_SingleByteWrite(ADXL345_INT_MAP_REG, settings);

}

/**
 * @brief i2c::getInterruptSource
 * @return
 */
char i2c::get_ADXL345_InterruptSource(void){
 qDebug()<<__FUNCTION__<<__LINE__<<"I2C";
    return ADXL345_SingleByteRead(ADXL345_INT_SOURCE_REG);

}
