#include <QCoreApplication>
#include "i2c.hpp"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    i2c * I2C;
    I2C = new i2c();
    return a.exec();
}
