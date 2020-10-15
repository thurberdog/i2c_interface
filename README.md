# i2c_interface

The write system call is used to obtain data from the I²C peripheral. Write requires a file handle, a buffer in which the data is stored, and a number of bytes to write. Write will attempt to write the number of bytes specified and will return the actual number of bytes written, which can be used to detect errors.

Some devices require an internal address to be sent prior to the data to specify the register on the external device to access. 
