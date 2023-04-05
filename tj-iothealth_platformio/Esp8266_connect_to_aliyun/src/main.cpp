
#include <Arduino.h>
#include <NTPClient.h> //时钟
#include <Wire.h>             //I2C
#include <ESP8266WiFi.h>      //
#include <WiFiUdp.h>
#include <ESP8266WiFiMulti.h> //
#include <PubSubClient.h>     //PubSubClient库
#include <ArduinoJson.h>      //jsonV5版本

#include "MAX30205.h"    //温度模块
#include "MAX30105.h"    //血氧模块
#include "aliyun_mqtt.h" //阿里云mqtt
#include "wifi.h"        //wifi模块

//#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>
//#include "OLED_SSD1306.h"
//#include <U8g2lib.h>
/*
String weekDays[7]={"Sunday", "Monday", "Tuesday","Wednesday", "Thursday", "Friday", "Saturday"};
//Month names
String months[12]={"January", "February", "March", "April","May", "June", "July", "August", "September", "October", "November", "December"};

//中文：心
static const unsigned char PROGMEM str_1[] =
    {
        0x00, 0x00, 0x02, 0x00, 0x01, 0x00, 0x00, 0x80, 0x00, 0x80, 0x04, 0x00, 0x04, 0x08, 0x24, 0x04,
        0x24, 0x04, 0x24, 0x02, 0x44, 0x02, 0x44, 0x12, 0x84, 0x10, 0x04, 0x10, 0x03, 0xF0, 0x00, 0x00};
//中文：率
static const unsigned char PROGMEM str_2[] =
    {
        0x02, 0x00, 0x01, 0x00, 0x7F, 0xFC, 0x02, 0x00, 0x44, 0x44, 0x2F, 0x88, 0x11, 0x10, 0x22, 0x48,
        0x4F, 0xE4, 0x00, 0x20, 0x01, 0x00, 0xFF, 0xFE, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00};
*/
////////////////////////OLED_setting

#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

//Adafruit_SSD1306 myOLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
//OLED.display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
////////////////////////
//OLED myOLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
//myOLED.display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
//OLED myOLED;
//U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

static WiFiClient espClient; //esp-12F作为客户端
static WiFiUDP espUdp;//espUdp
//ESP8266WiFiMulti wifiMulti; // 建立ESP8266WiFiMulti对象,对象名称是 'wifiMulti'
PubSubClient mqttClient(espClient);

NTPClient timeClient(espUdp,"ntp1.aliyun.com",60*60*8,30*60*1000);//
// 引入阿里云 IoT SDK
//#include <AliyunIoTSDK.h>

// 设置产品和设备的信息，从阿里云设备信息里查看(注意一机一密)
#define PRODUCT_KEY "a1RXUd800wl"
#define DEVICE_NAME "esp8266-1"
#define DEVICE_SECRET "178e8e484b40eacec500c901f90c4e59"
#define REGION_ID "cn-shanghai"

/////////////////////TOPIC
//属性上报TOPIC
#define ALINK_TOPIC_PROP_POST "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/event/property/post"
//#define ALINK_TOPIC_PROP_SET "/sys/" PRODUCT_KEY "/" DEVICE_SECRET "/event/property/post_reply"
#define ALINK_TOPIC_PROP_SET "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/service/property/set"
/////////////////////
unsigned long lastMs = 0;

wifi mywifi;         //定义wifi实例
MAX30205 tempSensor; //定义温度模块实例
MAX30105 spO2Sensor; //定义血氧模块实例
//声明体征参数
float spo2;         //血氧
int32_t heart_rate; //心率
float temperature;  //温度

void mqtt_callback(char *topic, byte *payload, unsigned int length);
void callback(char *topic, byte *payload, unsigned int length);
void mqtt_check_connect();
void mqtt_interval_post(float BodyTemperature);
void testdrawtemperature(int BPM);
void drawtime(String time);

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  timeClient.begin();//时间

  //u8g2.begin();
  
  //delay(2000);
  //myOLED.Initial();
  //OLED setting
  //初始化wifi
  mywifi.wifi_Init();
  // 初始化 iot，需传入 wifi 的 client，和设备产品信息
  //AliyunIoTSDK::begin(mqttClient, PRODUCT_KEY, DEVICE_NAME, DEVICE_SECRET, REGION_ID);

  mqttClient.setCallback(mqtt_callback); //回调函数
  //mqttClient.setCallback(callback); //回调函数
  
                                        
/*
  if (!OLED.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }
  else{
    OLED.clearDisplay();
    OLED.display();
  }
*/
  
  //AliyunIoTSDK::bindData("PowerSwitch", powerCallback);
  //温度传感器初始化
  //tempSensor.Initial();
  //学氧传感器初始化
  //spO2Sensor.Initial();
}

void loop()
{
  //testdrawtemperature(37);
  //timeClient.update();
  //myOLED.drawHR(77);
  /*
  unsigned long epochTime = timeClient.getEpochTime();
  drawtime(timeClient.getFormattedTime());
  struct tm *ptm = gmtime ((time_t *)&epochTime);
    int monthDay = ptm->tm_mday;
    Serial.print("Month day: ");
    Serial.println(monthDay);
    int currentMonth = ptm->tm_mon+1;
    Serial.print("Month: ");
    Serial.println(currentMonth);
    String currentMonthName = months[currentMonth-1];
    Serial.print("Month name: ");
    Serial.println(currentMonthName);
    int currentYear = ptm->tm_year+1900;
    Serial.print("Year: ");
    Serial.println(currentYear);
  */
  //AliyunIoTSDK::loop();

  //temperature = tempSensor.getTemperature(); // 获取温度
  //Serial.print(temperature, BIN);
  //Serial.println("'c");
  //delay(100);
  //spO2Sensor.getSpO2(&spo2, &heart_rate); //获取血氧和心率
  //Serial.printf("SP02 ");
  //Serial.print(spo2);
  //Serial.print(", Pulse ");
  //Serial.print(heart_rate);
  //Serial.println();

  if (millis() - lastMs >= 5000)
  {
    lastMs = millis();
    mqtt_check_connect();
    //mqtt_interval_post(37);
  }
  mqttClient.loop();
  //testdrawtemperature(37);
}

/*
//阿里云SDK的回调函数 其中一部分用于解析来自物联网平台转发的微信消息
void callback(char *topic, byte *payload, unsigned int length)
{
    
    Serial.print("Message arrived [");
    
    Serial.print("] ");
    payload[length] = '\0';
    Serial.println((char *)payload);
    
    Serial.println(topic);
    Serial.println(AliyunIoTSDK::ALINK_TOPIC_SUB);
    
    if (strstr(topic, AliyunIoTSDK::ALINK_TOPIC_SUB))
    {

        StaticJsonDocument<200> doc;
        DeserializationError error = deserializeJson(doc, payload); //反序列化JSON数据

        if (!error) //检查反序列化是否成功
        {
            AliyunIoTSDK::parmPass(doc.as<JsonVariant>()); //将参数传递后打印输出
            //
            //JsonArray arr = doc.as<JsonArray>();
            Serial.println(doc[0].as<char *>()); //本应该是 1 ,但是输出为空
                //
        }
    //temperature = tempSensor.getTemperature(); // 获取温度
    AliyunIoTSDK::send("BodyTemperature", 36);
    }
    
    //temperature = tempSensor.getTemperature(); // 获取温度
    //AliyunIoTSDK::send("BodyTemperature", temperature);
}
*/

//回调函数-阿里云端数据发送过来之后，此函数进行解析

void mqtt_callback(char *topic, byte *payload, unsigned int length) //mqtt回调函数“byte *payload”这东西是个指针
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  payload[length] = '\0';

  Serial.println((char *)payload);

  if (strstr(topic, ALINK_TOPIC_PROP_SET))
  {
    //mqtt_interval_post(37);
  }
}

//订阅topic
void mqtt_check_connect()
{
  if (!mqttClient.connected())
  {
    while (connect_aliyun_mqtt(mqttClient, PRODUCT_KEY, DEVICE_NAME, DEVICE_SECRET))
    {
      Serial.println("MQTT connect succeed!");
      mqttClient.subscribe(ALINK_TOPIC_PROP_SET); //这个就是引用开始定义的topic 订阅topic
      Serial.println("subscribe done");
    }
  }
  else
  {
    Serial.println("state is connected");
  }
}

//上传数据到云端
void mqtt_interval_post(float BodyTemperature)
{
  char param[512];
  //char jsonBuf[1024];

  sprintf(param, "{\"params\":{\"BodyTemperature\":%f}}", BodyTemperature);
  Serial.println(param);
  mqttClient.publish(ALINK_TOPIC_PROP_POST, param); //这个是上传数据的topic,jsonBuf这个是上传的数据
}
/*
void testdrawtemperature(int BPM)
{
  OLED.clearDisplay();

  //display.setTextSize(2);             // Normal 1:1 pixel scale
  //display.setTextColor(SSD1306_WHITE);        // Draw white text
  //display.setCursor(0,0);             // Start at top-left corner

  OLED.drawBitmap(0, 0, str_1, 16, 16, 1);  //在坐标X:26  Y:16的位置显示中文字符心
  OLED.drawBitmap(16, 0, str_2, 16, 16, 1); //在坐标X:42  Y:16的位置显示中文字符率

  OLED.setCursor(32, 0); //起点坐标
  OLED.setTextSize(2);   // Draw 2X-scale text
  OLED.setTextColor(SSD1306_WHITE);
  OLED.println(BPM);

  OLED.display();
  delay(2000);
}

void drawtime(String time)
{
  OLED.clearDisplay();
  OLED.setCursor(0, 0); //起点坐标
  OLED.setTextSize(2);   // Draw 2X-scale text
  OLED.setTextColor(SSD1306_WHITE);
  OLED.println(time);

  OLED.display();
  delay(2000);
}
*/

/*
#include <Arduino.h>
#include <Wire.h>
 
void setup(){
  Wire.begin();
  Serial.begin(115200);
  Serial.println("\nI2C Scanner");
}
void loop(){
  byte error, address;
  int nDevices;
  Serial.println("Scanning...");
  nDevices = 0;
  for (address = 1; address < 127; address++ ){
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0){
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println(" !");
      nDevices++;
    }else if (error == 4){
      Serial.print("Unknow error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");
  delay(5000); // wait 5 seconds for next scan
}
*/
