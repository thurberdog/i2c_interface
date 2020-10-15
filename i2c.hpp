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

class i2c : public QObject
{
    Q_OBJECT
public:
    explicit i2c(QObject *parent = nullptr);
    int file;
    char filename[40];
    const gchar *buffer;
    int addr = 0b00101001;   // The I2C address of the ADC
    char buf[10] = {0};
    float data;
    char channel;
    void sensors_ADC_init();
    void writeI2C();
    void readI2C();
    void initiateI2C();
    void openI2C();
signals:

};

#endif // I2C_HPP
