// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// MS5803_02BA
// This code is designed to work with the MS5803_02BA_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Analog-Digital-Converters?sku=MS5803-02BA_I2CS#tabs-0-product_tabset-2

#include <Wire.h>

// MS5803_02BA I2C address is 0x76(118)
#define Addr 0x76

void setup()
{
// Initialise I2C communication as MASTER
Wire.begin();
// Initialise serial communication, set baud rate = 9600
Serial.begin(9600);

// Start I2C Transmission
Wire.beginTransmission(Addr);
// Reset device command
Wire.write(0x1E);
// Stop I2C transmission
Wire.endTransmission();
delay(300);
}

void loop()
{
unsigned int data[2];
byte value[3];
byte value1[3];

// Read 12 bytes of calibration data, msb first
// Start I2C Transmission
Wire.beginTransmission(Addr);
// Calling conversion result register, 0x00(0)
Wire.write(0xA2);
// Stop I2C transmission
Wire.endTransmission();

// Request 2 bytes of data
Wire.requestFrom(Addr, 2);
if(Wire.available() == 2)
{
data[0] = Wire.read();
data[1] = Wire.read();
}
// Convert the data to 8 bits
unsigned int C1 = data[0] * 256 + data[1];

Wire.beginTransmission(Addr);
// Calling conversion result register, 0x00(0)
Wire.write(0xA4);
// Stop I2C transmission
Wire.endTransmission();

// Request 2 bytes of data
Wire.requestFrom(Addr, 2);
if(Wire.available() == 2)
{
data[0] = Wire.read();
data[1] = Wire.read();
}
// Convert the data to 8 bits
unsigned int C2 = data[0] * 256 + data[1];

Wire.beginTransmission(Addr);
// Calling conversion result register, 0x00(0)
Wire.write(0xA6);
// Stop I2C transmission
Wire.endTransmission();

// Request 2 bytes of data
Wire.requestFrom(Addr, 2);
if(Wire.available() == 2)
{
data[0] = Wire.read();
data[1] = Wire.read();
}
// Convert the data to 8 bits
unsigned int C3 = data[0] * 256 + data[1];

// Start I2C Transmission
Wire.beginTransmission(Addr);
// Calling conversion result register, 0x00(0)
Wire.write(0xA8);
// Stop I2C transmission
Wire.endTransmission();

// Request 2 bytes of data
Wire.requestFrom(Addr, 2);
if(Wire.available() == 2)
{
data[0] = Wire.read();
data[1] = Wire.read();
}
// Convert the data to 8 bits
unsigned int C4 = data[0] * 256 + data[1];

// Start I2C Transmission
Wire.beginTransmission(Addr);
// Calling conversion result register, 0x00(0)
Wire.write(0xAA);
// Stop I2C transmission
Wire.endTransmission();

// Request 2 bytes of data
Wire.requestFrom(Addr, 2);
if(Wire.available() == 2)
{
data[0] = Wire.read();
data[1] = Wire.read();
}
// Convert the data to 8 bits
unsigned int C5 = data[0] * 256 + data[1];

// Start I2C Transmission
Wire.beginTransmission(Addr);
// Calling conversion result register, 0x00(0)
Wire.write(0xAC);
// Stop I2C transmission
Wire.endTransmission();

// Request 2 bytes of data
Wire.requestFrom(Addr, 2);
if(Wire.available() == 2)
{
data[0] = Wire.read();
data[1] = Wire.read();
}
// Convert the data to 8 bits
unsigned int C6 = data[0] * 256 + data[1];

// Start I2C Transmission
Wire.beginTransmission(Addr);
// Reset device command
Wire.write(0x1E);
// Stop I2C transmission
Wire.endTransmission();

// Start I2C Transmission
Wire.beginTransmission(Addr);
// Command to initiate pressure conversion(OSR = 256)
Wire.write(0x40);
delay(500);
// Stop I2C transmission
Wire.endTransmission();

// Start I2C Transmission
Wire.beginTransmission(Addr);
// Read data from 0x00
Wire.write(0x00);
// Stop I2C transmission
Wire.endTransmission();
// Request 2 bytes of data
Wire.requestFrom(Addr, 3);

// Read 3 bytes of data
// msb, lsb
if(Wire.available() == 3)
{
value[0] = Wire.read();
value[1] = Wire.read();
value[2] = Wire.read();
}

// Convert the data
unsigned long D1 = (((value[0] & 0xFF) * 65536.0) + ((value[1] & 0xFF) * 256.0) + (value[2] & 0xFF));

// output to screen
Wire.beginTransmission(Addr);
// Command to initiate temperature conversion(OSR = 256)
Wire.write(0x50);
// Stop I2C transmission
Wire.endTransmission();
delay(500);

Wire.beginTransmission(Addr);
// Read data from 0x00
Wire.write(0x00);
// Stop I2C transmission
Wire.endTransmission();

// Request 2 bytes of data
Wire.requestFrom(Addr, 3);

// Read 2 bytes of data
// msb, lsb
if(Wire.available() == 3)
{
value1[0] = Wire.read();
value1[1] = Wire.read();
value1[2] = Wire.read();
}

// Convert the data
unsigned long D2 = (((value1[0] & 0xFF) * 65536.0) + ((value1[1] & 0xFF) * 256.0) + (value1[2] & 0xFF));

// output to screen
unsigned long dT = D2 - (C5 * 256.0);
unsigned long Temp = 2000.0 + dT * (C6 / 8388608.0);
unsigned long OFF = (C2 * 65536.0) + ((C4 * dT) / 64.0);
unsigned long SENS = (C1 * 32768.0) + ((C3 * dT ) / 128.0);
unsigned long T2 = 0;
unsigned long OFF2 = 0;
unsigned long SENS2 = 0;

if(Temp >= 2000)
{
T2 = 0;
OFF2 = 0;
SENS2 = 0;
}
else if(Temp < 2000)
{
T2 = (dT * dT) / 2147483648.0;
OFF2= 61.0 * ((Temp - 2000.0) * (Temp - 2000.0)) / 16.0;
SENS2= 2.0 * ((Temp - 2000.0) * (Temp - 2000.0));
if(Temp < -1500)
{
OFF2 = OFF2 + 20.0 * ((Temp + 1500.0) * (Temp + 1500.0));
SENS2 = SENS2 + 12.0 * ((Temp + 1500.0) * (Temp + 1500.0));
}
}

unsigned long temp = temp - T2;
OFF2 = (long)OFF - (long)OFF2;
SENS2 = (long)SENS - (long)SENS2;
double pressure = ((((SENS2 /2097152.0) * D1 - OFF2) / 327680.0) / 100.0);
double cTemp = Temp / 100.0;
double fTemp = cTemp * 1.8 + 32.0;

// Output data to screen
Serial.print("Temperature compensated Pressure : ");
Serial.println(pressure);
Serial.print("Temperature in Celsius : ");
Serial.println(cTemp);
Serial.print("Temperature in Fahrenheit : ");
Serial.println(fTemp);
}
