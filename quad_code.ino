//Arduino Side of things
#include <printf.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include "RF24.h"
#include "sensor_fusion.h"

//////////////////////////////
// MOTOR PINS
// 9, 6, 5, 3
//
// 5    +x    3
//
// +y    +   -y
//
// 6    -x    9
//////////////////////////////
struct Data
{
  int16_t accel_x;   // reg 59-60
  int16_t accel_y;   // reg 61-62
  int16_t accel_z;   // reg 63-64
  int16_t gyro_x;    // reg 69-70
  int16_t gyro_y;    // reg 71-72
  int16_t gyro_z;    // reg 73-74
};

int16_t offax;
int16_t offay;
int16_t offaz;
int16_t offgx;
int16_t offgy;
int16_t offgz;

Data mpu = {0,0,0,0,0,0};
uint8_t *mpu_ptr = (uint8_t *)&mpu;
uint8_t int_status = 0;

uint8_t setfl = 1;

uint8_t pwr_mgmt = 0;
uint8_t reg1 = 0;
uint8_t reg2 = 0;
uint8_t reg3 = 0;
int16_t acx = 0;
vector accel = {0,0,0};
vector raw_acc_data = {0,0,0};
vector norm_acc = {0,0,0};
SoftwareSerial mySerial (3,4);// (Rx, Tx)

int16_t flipByte(int16_t x)
{
  char *a = (char *)&x;
  char *b = a + 1;
  *a = (*a) ^ (*b);
  *b = (*a) ^ (*b);
  *a = (*a) ^ (*b);
  return x;
}

void normalize_val()
{
  int i = 0;
  int64_t ax = 0;
  int64_t ay = 0;
  int64_t az = 0;
  int64_t gx = 0;
  int64_t gy = 0;
  int64_t gz = 0;
  //printf("Starting Configuration...\n");
  while (i < 300)
  {
    readReg(0x3A, &int_status, 1);
    if ((int_status & 0x01))
    {
        readReg(0x3B, mpu_ptr, 6);
        readReg(0x43, mpu_ptr + 6, 6);
        ax += flipByte(mpu.accel_x);
        ay += flipByte(mpu.accel_y);
        az += flipByte(mpu.accel_z);
        gx += flipByte(mpu.gyro_x);
        gy += flipByte(mpu.gyro_y);
        gz += flipByte(mpu.gyro_z);
        i++;
    }

  }

  offax = (int16_t)(ax/300);
  offay = (int16_t)(ay/300);
  offaz = (int16_t)(az/300);
  offaz -= 16384;
  offgx = (int16_t)(gx/300);
  offgy = (int16_t)(gy/300);
  offgz = (int16_t)(gz/300);
  //printf("... Done.\n");
  //printf(" Offset Data::: %d\t%d\t%d\t\t\t%d\t%d\t%d\n\n\n", offax, offay, offaz, offgx, offgy, offgz);
}



void setup()
{
    printf_begin();
    Serial.begin(115200);
    mySerial.begin(9600);
    Wire.begin();
    pinMode(5, INPUT);
    pinMode(6, INPUT);
    pinMode(7, INPUT);
    delay(100);
    readReg(0x6B, &pwr_mgmt, 1);
    readReg(0x1A, &reg1, 1);
    readReg(0x1B, &reg2, 1);
    readReg(0x1C, &reg3, 1);
    //printf("Current values of registers:\npwr_mgmt:       %02x\nconfig:       %02x\ngyro_config:       %02x\n", pwr_mgmt, reg1, reg2);
    pwr_mgmt &= 0xBF;
    writeReg(0x6B, &pwr_mgmt, 1);
    reg1 &= 0xF8;
    reg2 |= 0x18;
    reg3 &= 0xE7;
    writeReg(0x1A, &reg1, 1);
    writeReg(0x1B, &reg2, 1);
    writeReg(0x1C, &reg3, 1);
    delay (100);
    readReg(0x6B, &pwr_mgmt, 1);
    readReg(0x1A, &reg1, 1);
    readReg(0x1B, &reg2, 1);
    //printf("\n\nNEW values of registers:\npwr_mgmt:       %02x\nconfig:       %02x\ngyro_config:       %02x\n", pwr_mgmt, reg1, reg2);
    normalize_val();
    //printf("\t\tAccelerometer\t\t\t\tGyroscope\n\n");

    
    
}

void loop()
{
  readReg(0x3A, &int_status, 1);
  if ((int_status & 0x01))
  {
      readReg(0x3B, mpu_ptr, 6);
      readReg(0x43, mpu_ptr + 6, 6);
      raw_acc_data.x = flipByte(mpu.accel_x) - offax;
      raw_acc_data.y = flipByte(mpu.accel_y) - offay;
      raw_acc_data.z = flipByte(mpu.accel_z) - offaz;
      mpu.gyro_x = flipByte(mpu.gyro_x) - offgx;
      mpu.gyro_y = flipByte(mpu.gyro_y) - offgy;
      mpu.gyro_z = flipByte(mpu.gyro_z) - offgz;
  }
  else
    return;

  //accel.x = (float)mpu.accel_x/(16384/9.81);
  //accel.y = (float)mpu.accel_y/(16384/9.81);
  //accel.z = (float)mpu.accel_z/(16384/9.81);
  vector_normalize(&raw_acc_data, &norm_acc);
  
  //double x = (double)acx / (16384/9.81);
  //printf("%d\t%d\t%d\t\t\t%d\t%d\t%d\n", mpu.accel_x, mpu.accel_y, mpu.accel_z, mpu.gyro_x, mpu.gyro_y, mpu.gyro_z);
  //delay(10);
  //printf("%f\t%f\t%f\n", accel.x, accel.y, accel.z);
  Serial.print(norm_acc.x);
  Serial.print(" ");
  Serial.print(norm_acc.y);
  Serial.print(" ");
  Serial.print(norm_acc.z);
  Serial.println("");
  delay (250);
}














