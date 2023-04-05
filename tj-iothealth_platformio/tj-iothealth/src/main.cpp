/*************************************************
Copyright: LiuChangjing

Author:LiuChangjing

Date:2021-05-28

Description:终端设备的wifi联网功能，连接物联网平台功能，数据监测功能，以及各类人机交互功能

**************************************************/

/*各类标准头文件*/
#include <Ticker.h>

#include <Arduino.h>     //Arduino库
#include <NTPClient.h>   //NTP时钟
#include <Wire.h>        //I2C
#include <ESP8266WiFi.h> //wifi基础功能
#include <WiFiUdp.h>
#include <ESP8266WiFiMulti.h>
#include <PubSubClient.h>
/*各类自建头文件*/
#include "MAX30205.h"     //温度模块
#include "MAX30105.h"     //血氧模块
#include "AliyunIOT.h"    //阿里云物联网平台
#include "wifi.h"         //WiFi模块
#include "OLED_SSD1306.h" //显示模块

/*显示模块 #define*/
#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

/*阿里云物联网平台 #define*/
/* 设置产品和设备的信息，从阿里云设备信息里查看*/
#define PRODUCT_KEY "a1RXUd800wl" //固定不变
#define REGION_ID "cn-shanghai"   //固定不变
//#define DEVICE_NAME "esp8266-1" //设备1
#define DEVICE_NAME "esp8266-2" //设备2
//#define DEVICE_SECRET "178e8e484b40eacec500c901f90c4e59"   //密码1
#define DEVICE_SECRET "d1d2a0fe381aa9ab64ca94b2b47af956" // 密码2
/* 发布和订阅的Topic*/
#define ALINK_TOPIC_PROP_POST "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/event/property/post"
//#define ALINK_TOPIC_PROP_SET "/sys/" PRODUCT_KEY "/" DEVICE_SECRET "/event/property/post_reply"
#define ALINK_TOPIC_PROP_SET "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/service/property/set"

/*创建各类对象*/
static OLED_SSD1306 myOLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); //建立 显示模块对象
static WiFiClient espClient;                                                //建立 wifi客户端
static WiFiUDP espUdp;                                                      //建立 wifiUdp对象
//ESP8266WiFiMulti wifiMulti; // 建立ESP8266WiFiMulti对象
PubSubClient mqttClient(espClient);                                           //建立 MQTT客户端对象
NTPClient timeClient(espUdp, "ntp1.aliyun.com", 60 * 60 * 8, 30 * 60 * 1000); //建立 NTP时钟对象
AliyunIOT IOTdeviceClient;                                                    //建立 连接物联网平台客户端对象
wifi mywifi;                                                                  //建立 wifi对象
MAX30205 tempSensor;                                                          //建立 温度模块对象
MAX30105 spO2Sensor;                                                          //建立 血氧模块对象
Ticker time1;
/* 设置开关1和开关2对应的GPIO*/
const int BtnPin1 = 0;  //右边的按键
const int BtnPin2 = 14; //右边的按键

/*声明各类函数*/
void mqtt_callback(char *topic, byte *payload, unsigned int length); //MQTT客户端回调函数
ICACHE_RAM_ATTR void callBack_Btn1(void);                            //外部引脚中断 开关1
ICACHE_RAM_ATTR void callBack_Btn2(void);                            //外部引脚中断 开关2
void mqtt_check_connect();
void mqtt_interval_post(float BodyTemperature);

/*声明各类变量*/
float spo2;         //血氧
int32_t heart_rate; //心率
float temperature;  //温度

/*声明 中断（回调）中使用的变量*/

/*消除按键抖动*/
volatile unsigned long triggertime = 0; //按键按下，第一次触发中断的时刻
volatile unsigned long delta = 200;     //去抖期的时长 （毫秒）
/*按键选择变量*/
volatile int32_t btn_num = 0; //默认为“信息”
volatile boolean btn_click = false;
/*控制闪烁时间*/
volatile unsigned long triggertime1 = 0; //按键按下，第一次触发中断的时刻
volatile unsigned long delta1 = 4200;    //闪烁时间
/*控制定时测量体征参数的时间*/
volatile unsigned long triggertime2 = 0;
volatile unsigned long triggertime3 = 0;
volatile unsigned long delta2 = 30000; //间隔时间60 000ms，即60s //若主动或被动测量，则清零
volatile unsigned long delta3 = 30000;
/*血氧变量、心率变量*/
volatile float spo2_1;
volatile int32_t heart_rate_1;

/*测量失败标志*/
volatile boolean flag1 = 0;

/*记录显示的时间*/
lasttime ltime;

void get();

void setup()
{
  Serial.begin(115200);
  ///////开关初始化设置
  pinMode(BtnPin1, INPUT);        //内部上拉
  pinMode(BtnPin2, INPUT_PULLUP); //设置上拉
  ///////
  Wire.begin();
  timeClient.begin(); //时间
  myOLED.Initial();
  //初始化wifi
  //mywifi.wifi_Init(myOLED);
  mywifi.wifimanager_Init(myOLED);

  //更新
  IOTdeviceClient.getinfo(mqttClient, PRODUCT_KEY, DEVICE_NAME, DEVICE_SECRET, ALINK_TOPIC_PROP_POST, ALINK_TOPIC_PROP_SET);
  IOTdeviceClient.mqtt_check_connect(myOLED, mqttClient, PRODUCT_KEY, DEVICE_NAME, DEVICE_SECRET, ALINK_TOPIC_PROP_SET);
  // 初始化 iot，需传入 wifi 的 client，和设备产品信息
  //AliyunIoTSDK::begin(mqttClient, PRODUCT_KEY, DEVICE_NAME, DEVICE_SECRET, REGION_ID);

  mqttClient.setCallback(mqtt_callback);            //阿里云物联网平台回调函数
  attachInterrupt(BtnPin1, callBack_Btn1, FALLING); //当电平发生变化时，触发中断
  attachInterrupt(BtnPin2, callBack_Btn2, FALLING); //当电平发生变化时，触发中断

  //温度传感器初始化
  tempSensor.Initial();
  //血氧传感器初始化
  spO2Sensor.Initial();
  time1.attach(60, get);
  //time2.attach(60,)
  WiFi.setSleepMode (WIFI_MODEM_SLEEP,3);
}

void loop()
{
  myOLED.clearDisplay(); //清屏
  /*

  //判断定时监测时间间隔
  if (millis() - triggertime2 > delta2)
  {
    Serial.println("定时上报数据");
    triggertime2 = millis();
    temperature = tempSensor.getTemperature(); // 获取温度
    spO2Sensor.getSpO2(&spo2, &heart_rate);    //获取血氧和心率
    String MAC = WiFi.macAddress();
    if (temperature == 0.00 || spo2 == -999 || heart_rate == -999)
    {
      Serial.println("测量失败，2分钟后重试一次");
      triggertime2 = millis(); //重置定时测温计时器
      flag1 = 1;               //测量失败标志
    }
    else
    {
      flag1 = 0; //测量成功标志
      //向物联网平台上报数据
      IOTdeviceClient.mqtt_post(mqttClient, temperature, heart_rate, spo2, MAC.c_str(), ALINK_TOPIC_PROP_POST);
    }
  }
  */
  myOLED.drawtime(timeClient, &ltime); //显示屏第一行 时间
  if (btn_click && btn_num == 0)
  {
    mywifi.drawInfo(myOLED);
    myOLED.drawBack();
  }
  else
  {
    //显示屏第二行 温度 血氧
    myOLED.drawBodyTemperature(temperature);
    myOLED.drawSpO2(spo2);
    myOLED.drawHeartRate(heart_rate); //显示屏第三行 心率
    myOLED.drawConfirm();
    myOLED.drawfail(flag1);
  }
  myOLED.drawInfo();
  myOLED.drawMonitoring();

  if (millis() - triggertime1 < delta1)
  {
    //闪烁时间
    if (btn_num == 0)
      myOLED.drawtan(0);
    else if (btn_num == 1)
      myOLED.drawtan(31);
    else if (btn_num == 2)
    {
      myOLED.drawtan(65);
    }
    Serial.println("闪烁1");
  }
  myOLED.display(); //显示屏幕

  /*检查WiFi连接*/
  /*
  if (WiFi.status() != WL_CONNECTED)
  {
    //mywifi.wifi_Init(myOLED);
    mywifi.wifimanager_Init(myOLED);
  }
  */
  /*检查物联网平台连接*/
  IOTdeviceClient.mqtt_check_connect(myOLED, mqttClient, PRODUCT_KEY, DEVICE_NAME, DEVICE_SECRET, ALINK_TOPIC_PROP_SET);
  IOTdeviceClient.mqtt_loop(mqttClient);
  //WiFi.forceSleepBegin(); // Wifi Off
}

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
    delay(500);
    triggertime2 = millis();
    Serial.println("监听触发回调_上报数据");
    temperature = tempSensor.getTemperature(); // 获取温度

    Serial.println(temperature);
    Serial.println("'c");
    //delay(100);
    spO2Sensor.getSpO2(&spo2, &heart_rate); //获取血氧和心率

    Serial.printf("SP02 ");
    Serial.print(spo2);
    Serial.print(", Pulse ");
    Serial.print(heart_rate);
    Serial.println();

    String MAC = WiFi.macAddress();
    if (temperature == 0.00 || spo2 == -999 || heart_rate == -999)
    {
      Serial.println("测量失败，2分钟后重试一次");
      triggertime2 = millis(); //重置定时测温计时器
      flag1 = 1;
    }
    else
    {
      flag1 = 0;
      IOTdeviceClient.mqtt_post(mqttClient, temperature, heart_rate, spo2, MAC.c_str(), ALINK_TOPIC_PROP_POST);
    }
  }
}

//外部引脚中断  开关1中断 用于移动选项
ICACHE_RAM_ATTR void callBack_Btn1(void)
{
  if (millis() - triggertime > delta) //消除抖动
  {
    triggertime = millis();
    if (!btn_click) //此时与btn2互锁 btn_click默认为0
    {
      //判断是“信息”,“测量”还是 “配置”
      if (millis() - triggertime1 > delta1)
      {
        //如果大于delta1，按键从信息开始显示
        triggertime1 = millis();
        btn_num = 0;
      }
      else
      {
        triggertime1 = millis();
        if (btn_num == 1) //触发 配置 后 回到 信息
        {
          btn_num = 0;
        }
        else
        {
          btn_num++;
        }
      }

      Serial.println(btn_num);

      Serial.println("触发了中断1");
      //temperature = tempSensor.getTemperature(); // 获取温度

      //Serial.println(temperature);
    }
  }
  //int level = digitalRead(13); //读取GPIO_13上的电平
}

//外部引脚中断  开关2中断 用于确认选项
ICACHE_RAM_ATTR void callBack_Btn2(void)
{
  if (millis() - triggertime > delta) //消除抖动
  {
    triggertime = millis();

    if (btn_click) //此时与btn1互锁btn_click默认为0
    {
      //确认状态
      //再次点击退出确认状态
      btn_click = !btn_click;
      Serial.println("退出info菜单");
    }
    else
    {
      Serial.println("触发了中断2");
      Serial.println(btn_num);
      if (millis() - triggertime1 < delta1)
      {
        //在闪烁时间内才能点击确认按钮

        triggertime1 = millis() - delta1;
        if (btn_num == 0)
        {
          btn_click = !btn_click;
          Serial.println("进入info菜单");
        }
        else if (btn_num == 1)
        {
          triggertime2 = millis();
          Serial.println("按键触发外部中断_主动上报数据");
          //测量体征参数
          temperature = tempSensor.getTemperature(); // 获取温度
          PA b = spO2Sensor.getSpO2_test();
          spo2 = b->n_spo2;
          heart_rate = b->n_heart_rate;
          String MAC = WiFi.macAddress();
          if (temperature == 0.00 || spo2 == -999 || heart_rate == -999)
          {
            Serial.println("测量失败，2分钟后重试一次");
            triggertime2 = millis(); //重置定时测温计时器
            flag1 = 1;
          }
          else
          {
            flag1 = 0;
            IOTdeviceClient.mqtt_post(mqttClient, temperature, heart_rate, spo2, MAC.c_str(), ALINK_TOPIC_PROP_POST);
          }
        }
        else if (btn_num == 2)
        {
          //设置wifi
          //mywifi.webconfig(myOLED);
          WiFi.mode(WIFI_AP_STA);
        }
        Serial.print("btn_click:");
        Serial.println(btn_click);
      }
    }
  }
}

//定时器中断
void get()
{
  //WiFi.forceSleepWake(); // Wifi on
  Serial.println("定时器中断_主动上报数据");
  //测量体征参数
  temperature = tempSensor.getTemperature(); // 获取温度
  PA b = spO2Sensor.getSpO2_test();
  spo2 = b->n_spo2;
  heart_rate = b->n_heart_rate;
  String MAC = WiFi.macAddress();
  if (temperature == 0.00 || spo2 == -999 || spo2 == 0 || heart_rate == -999 || heart_rate == 0)
  {
    Serial.println("测量失败，60秒后重试一次");
    triggertime2 = millis(); //重置定时测温计时器
    flag1 = 1;
  }
  else
  {
    flag1 = 0;
    IOTdeviceClient.mqtt_post(mqttClient, temperature, heart_rate, spo2, MAC.c_str(), ALINK_TOPIC_PROP_POST);
  }
}