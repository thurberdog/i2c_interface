#ifndef I2C_HPP
#define I2C_HPP
#include <QtGlobal>
#include <glib.h>
#include <glib/gprintf.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <QObject>
#include <QDebug>
//Registers.
#define ADXL345_DEVID_REG          0x00
#define ADXL345_THRESH_TAP_REG     0x1D
#define ADXL345_OFSX_REG           0x1E
#define ADXL345_OFSY_REG           0x1F
#define ADXL345_OFSZ_REG           0x20
#define ADXL345_DUR_REG            0x21
#define ADXL345_LATENT_REG         0x22
#define ADXL345_WINDOW_REG         0x23
#define ADXL345_THRESH_ACT_REG     0x24
#define ADXL345_THRESH_INACT_REG   0x25
#define ADXL345_TIME_INACT_REG     0x26
#define ADXL345_ACT_INACT_CTL_REG  0x27
#define ADXL345_THRESH_FF_REG      0x28
#define ADXL345_TIME_FF_REG        0x29
#define ADXL345_TAP_AXES_REG       0x2A
#define ADXL345_ACT_TAP_STATUS_REG 0x2B
#define ADXL345_BW_RATE_REG        0x2C
#define ADXL345_POWER_CTL_REG      0x2D
#define ADXL345_INT_ENABLE_REG     0x2E
#define ADXL345_INT_MAP_REG        0x2F
#define ADXL345_INT_SOURCE_REG     0x30
#define ADXL345_DATA_FORMAT_REG    0x31
#define ADXL345_DATAX0_REG         0x32
#define ADXL345_DATAX1_REG         0x33
#define ADXL345_DATAY0_REG         0x34
#define ADXL345_DATAY1_REG         0x35
#define ADXL345_DATAZ0_REG         0x36
#define ADXL345_DATAZ1_REG         0x37
#define ADXL345_FIFO_CTL           0x38
#define ADXL345_FIFO_STATUS        0x39

//Data rate codes.
#define ADXL345_3200HZ      0x0F
#define ADXL345_1600HZ      0x0E
#define ADXL345_800HZ       0x0D
#define ADXL345_400HZ       0x0C
#define ADXL345_200HZ       0x0B
#define ADXL345_100HZ       0x0A
#define ADXL345_50HZ        0x09
#define ADXL345_25HZ        0x08
#define ADXL345_12HZ5       0x07
#define ADXL345_6HZ25       0x06
// read or write bytes
#define ADXL345_I2C_READ    0xA7
#define ADXL345_I2C_WRITE   0xA6
#define ADXL345_I2C_ADDRESS 0x53   //the ADXL345 7-bit address is 0x53 when ALT ADDRESS is low as it is on the sparkfun chip: when ALT ADDRESS is high the address is 0x1D

/////////////when ALT ADDRESS pin is high:
//#define ADXL345_I2C_READ    0x3B
//#define ADXL345_I2C_WRITE   0x3A
//#define ADXL345_I2C_ADDRESS 0x1D
#define ADXL345_X           0x00
#define ADXL345_Y           0x01
#define ADXL345_Z           0x02

class i2c : public QObject
{
    Q_OBJECT
public:
    explicit i2c(QObject *parent = nullptr);
    int file;
    char filename[40];
//    const gchar *buffer;
    int addr = 0b00101001;   // The I2C address of the ADC
    char buf[10] = {0};
    float data;
    char channel;
    void openI2C();
    void initiateI2C(int address);
    void frequency(int hz);
    char SingleByteRead(char address);
    int SingleByteWrite(char address, char data);
    void multiByteRead(char address, char *output, int size);
    int multiByteWrite(char address, char *ptr_data, int size);
    void getOutput(int *readings);
    char getDeviceID();
    int setPowerMode(char mode);
    char getPowerControl();
    int setPowerControl(char settings);
    char getDataFormatControl();
    int setDataFormatControl(char settings);
    int setDataRate(char rate);
    char getOffset(char axis);
    int setOffset(char axis, char offset);
    char getFifoControl();
    int setFifoControl(char settings);
    char getFifoStatus();
    char getTapThreshold();
    int setTapThreshold(char threshold);
    float getTapDuration();
    int setTapDuration(short duration_us);
    float getTapLatency();
    int setTapLatency(short latency_ms);
    float getWindowTime();
    int setWindowTime(short window_ms);
    char getActivityThreshold();
    int setActivityThreshold(char threshold);
    char getInactivityThreshold();
    int setInactivityThreshold(char threshold);
    char getTimeInactivity();
    int setTimeInactivity(char timeInactivity);
    char getActivityInactivityControl();
    int setActivityInactivityControl(char settings);
    char getFreefallThreshold();
    int setFreefallThreshold(char threshold);
    char getFreefallTime();
    int setFreefallTime(short freefallTime_ms);
    char getTapAxisControl();
    int setTapAxisControl(char settings);
    char getTapSource();
    char getInterruptEnableControl();
    int setInterruptEnableControl(char settings);
    char getInterruptMappingControl();
    int setInterruptMappingControl(char settings);
    char getInterruptSource();
    void sensors_ADXL_init(int address, const gchar *buffer, int length);
    int writeI2C(int address, const gchar *buffer, int length);
    void readI2C(int address, const gchar *buffer, int length);
signals:

};

#endif // I2C_HPP
