//Arduino Side of things
#include <printf.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include "RF24.h"
#include "sensor_fusion.h"

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
quaternion quat = {0,0,0,0};

uint8_t int_status = 0; //for reading in loop

uint8_t pwr_mgmt = 0;
uint8_t reg1 = 0;
uint8_t reg2 = 0;
uint8_t reg3 = 0;

unsigned long t_delay = 0;
float angle = 0;

vector raw_acc = {0,0,0};
vector raw_gyro = {0,0,0};
vector avg_gyro = {0,0,0};
vector norm_acc = {0,0,0};
vector norm_gyro = {0,0,0};
vector gyro_init = {0,0,1};
vector combo = {0,0,1};
vector combo_norm = {0,0,1};

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

void bias_val()
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
    bias_val();
    //printf("\t\tAccelerometer\t\t\t\tGyroscope\n\n");

    
    
}

void loop()
{
  
    readReg(0x3A, &int_status, 1);
    if ((int_status & 0x01))
    {
        readReg(0x3B, mpu_ptr, 6);
        readReg(0x43, mpu_ptr + 6, 6);
        raw_acc.x = flipByte(mpu.accel_x) - offax;
        raw_acc.y = flipByte(mpu.accel_y) - offay;
        raw_acc.z = flipByte(mpu.accel_z) - offaz;
        raw_gyro.x = flipByte(mpu.gyro_x) - offgx;
        raw_gyro.y = flipByte(mpu.gyro_y) - offgy;
        raw_gyro.z = flipByte(mpu.gyro_z) - offgz;
        t_delay = millis();
        raw_acc.x /= 16384;
        raw_acc.y /= 16384;
        raw_acc.z /= 16384;
        
    }

    delay(10);
    readReg(0x3A, &int_status, 1);
    if ((int_status & 0x01))
    {
      readReg(0x43, mpu_ptr + 6, 6);
      avg_gyro.x = ((raw_gyro.x + flipByte(mpu.gyro_x) - offgx) / 2);
      avg_gyro.y = ((raw_gyro.y + flipByte(mpu.gyro_y) - offgy) / 2);
      avg_gyro.z = ((raw_gyro.z + flipByte(mpu.gyro_z) - offgz) / 2);
      
      t_delay = millis() - t_delay;
//      Serial.print("Average Gyro raw:");
//      Serial.print("\t");
//      Serial.print(avg_gyro.x);
//      Serial.print(" ");
//      Serial.print(avg_gyro.y);
//      Serial.print(" ");
//      Serial.print(avg_gyro.z);
//      Serial.println("");

      avg_gyro.x *= ((t_delay) * 0.0010642 * 6.28);
      avg_gyro.y *= ((t_delay) * 0.0010642 * 6.28);
      avg_gyro.z *= ((t_delay) * 0.0010642 * 6.28);
      
      avg_gyro.x /= -1000;
      avg_gyro.y /= -1000;
      avg_gyro.z /= -1000;

//      Serial.print("Average Gyro AFTER MULTIPLICATIONS:");
//      Serial.print("\t");
//      Serial.print(avg_gyro.x);
//      Serial.print(" ");
//      Serial.print(avg_gyro.y);
//      Serial.print(" ");
//      Serial.print(avg_gyro.z);
//      Serial.println("");

      vector_normalize(&raw_acc, &norm_acc);
      angle = vector_normalize(&avg_gyro, &norm_gyro);
      quaternion_create(&norm_gyro, angle, &quat);
      quaternion_rotate(&gyro_init, &quat, &gyro_init);
      quaternion_rotate(&combo, &quat, &combo);
      combo.x = 0.3*norm_acc.x + 0.7*combo.x;
      combo.y = 0.3*norm_acc.y + 0.7*combo.y;
      combo.z = 0.3*norm_acc.z + 0.7*combo.z;
      vector_normalize(&combo, &combo_norm);
      
    }


  delay(50);
  /*
  Serial.print(raw_acc.x);
  Serial.print("\t");
  Serial.print(raw_acc.y);
  Serial.print("\t");
  Serial.print(raw_acc.z);
  Serial.print("\t\t");
  Serial.print(raw_gyro.x);
  Serial.print("\t");
  Serial.print(raw_gyro.y);
  Serial.print("\t");
  Serial.print(raw_gyro.z);
  Serial.println("");
  */
  
  Serial.print(norm_acc.x);
  Serial.print(" ");
  Serial.print(norm_acc.y);
  Serial.print(" ");
  Serial.print(norm_acc.z);
  Serial.print(" ");
  Serial.print(gyro_init.x);
  Serial.print(" ");
  Serial.print(gyro_init.y);
  Serial.print(" ");
  Serial.print(gyro_init.z);
  Serial.print(" ");
  Serial.print(combo_norm.x);
  Serial.print(" ");
  Serial.print(combo_norm.y);
  Serial.print(" ");
  Serial.print(combo_norm.z);
  Serial.println("");
  
}














