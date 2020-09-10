#ifndef COMM_H_
#define COMM_H_

#include <stdint.h>

int i2cSetup(int addr);
int i2cMem8Read(int dev, int add, uint8_t* buff, int size);
int i2cMem8Write(int dev, int add, uint8_t* buff, int size);


#endif //COMM_H_