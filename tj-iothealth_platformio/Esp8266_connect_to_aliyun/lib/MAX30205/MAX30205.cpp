#include "Arduino.h"
#include <Wire.h>
#include "MAX30205.h"

//在Initial的结尾加入sutdown()进入休眠模式
void MAX30205::Initial(void){
    while (!MAX30205::scanAvailableSensors())
    {
        Serial.println("Couldn't find the temperature sensor, please connect the sensor.");
        delay(3000);
    }

    MAX30205::begin(); // set continuos mode, active mode
}

//后期改变逻辑在get里面先begin()启动，随后检测温度，然后shutdown()关闭:目的是节省电量
float MAX30205::getTemperature(void){
	uint8_t readRaw[2] = {0}; //一个字节
    I2CreadBytes(sensorAddress,MAX30205_TEMPERATURE, &readRaw[0] ,2); // read two bytes 读取两个字节
	int16_t raw = readRaw[0] << 8 | readRaw[1];  //combine two bytes //将两个字节组合，先左移再拼在一起
    temperature = raw  * 0.00390625;     // convert to temperature
	return  temperature;
}

bool MAX30205::scanAvailableSensors(void){
  bool sensorFound = false;

  Wire.beginTransmission (MAX30205_ADDRESS1);
  //自己添加的一行代码
    //Serial.println(Wire.endTransmission ());//输出显示 2 NACK 未响应
  if (Wire.endTransmission () == 0){
    sensorAddress = MAX30205_ADDRESS1;
    sensorFound = true;
  }
  else{
      //Serial.println(Wire.endTransmission ());
  }
  
  Wire.beginTransmission (MAX30205_ADDRESS2);
  if(Wire.endTransmission () == 0){
    sensorAddress = MAX30205_ADDRESS2;
    sensorFound = true;
  }

////////自己添加的新地址
  Wire.beginTransmission (MAX30205_ADDRESS3);
  if(Wire.endTransmission () == 0){
    sensorAddress = MAX30205_ADDRESS3;
    sensorFound = true;
  }
  return sensorFound;
}

//max30205休眠
void MAX30205::shutdown(void){
  uint8_t reg = I2CreadByte(sensorAddress, MAX30205_CONFIGURATION);  // Get the current register
  I2CwriteByte(sensorAddress, MAX30205_CONFIGURATION, reg | 0x80);
}

//max30205启动
void MAX30205::begin(void){
  I2CwriteByte(sensorAddress, MAX30205_CONFIGURATION, 0x00); //mode config
  I2CwriteByte(sensorAddress, MAX30205_THYST , 		 0x00); // set threshold
  I2CwriteByte(sensorAddress, MAX30205_TOS, 			 0x00); //
}

void MAX30205::printRegisters(void){
  Serial.println(I2CreadByte(sensorAddress, MAX30205_TEMPERATURE),  BIN);
  Serial.println(I2CreadByte(sensorAddress, MAX30205_CONFIGURATION),  BIN);
  Serial.println(I2CreadByte(sensorAddress, MAX30205_THYST), BIN);
  Serial.println(I2CreadByte(sensorAddress, MAX30205_TOS), BIN);

}

// Wire.h read and write protocols
void MAX30205::I2CwriteByte(uint8_t address, uint8_t subAddress, uint8_t data){
	Wire.beginTransmission(address);  // Initialize the Tx buffer
	Wire.write(subAddress);           // Put slave register address in Tx buffer
	Wire.write(data);                 // Put data in Tx buffer
	Wire.endTransmission();           // Send the Tx buffer
}

uint8_t MAX30205::I2CreadByte(uint8_t address, uint8_t subAddress){
	uint8_t data; // `data` will store the register data
	Wire.beginTransmission(address);
	Wire.write(subAddress);
	Wire.endTransmission(false);
	Wire.requestFrom(address, (uint8_t) 1);
	data = Wire.read();
	return data;
}

void MAX30205::I2CreadBytes(uint8_t address, uint8_t subAddress, uint8_t * dest, uint8_t count){
	Wire.beginTransmission(address);   // Initialize the Tx buffer 开始要传输数据的目的设备的7位地址
	// Next send the register to be read. OR with 0x80 to indicate multi-read.
	Wire.write(subAddress); // 写入温度寄存器地址的数据
	Wire.endTransmission(false); //结束传输
	uint8_t i = 0;
	Wire.requestFrom(address, count);  // Read bytes from slave register address 主机 请求 从机的 寄存器地址中读取数据
    //连续获取count个数据
	while (Wire.available())//可否获取一个数据
	{
		dest[i++] = Wire.read();
	}
}
