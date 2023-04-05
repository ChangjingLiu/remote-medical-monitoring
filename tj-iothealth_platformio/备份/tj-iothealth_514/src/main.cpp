
#include <Arduino.h>
#include <NTPClient.h>   //时钟
#include <Wire.h>        //I2C
#include <ESP8266WiFi.h> //
#include <WiFiUdp.h>
#include <ESP8266WiFiMulti.h> //
#include <PubSubClient.h>     //PubSubClient库
//#include <ArduinoJson.h>      //jsonV5版本

#include "MAX30205.h" //温度模块
#include "MAX30105.h" //血氧模块
//#include "aliyun_mqtt.h" //阿里云mqtt
#include "AliyunIOT.h" //阿里云mqtt
#include "wifi.h"      //wifi模块

//#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>
#include "OLED_SSD1306.h"
//#include <U8g2lib.h>

#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

//开关
const int BtnPin1 = 0;  //右边的按键
const int BtnPin2 = 14; //右边的按键

static OLED_SSD1306 myOLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
//OLED.display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
////////////////////////
//OLED myOLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
//myOLED.display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
//OLED myOLED;

static WiFiClient espClient; //esp-12F作为客户端
static WiFiUDP espUdp;       //espUdp
//ESP8266WiFiMulti wifiMulti; // 建立ESP8266WiFiMulti对象,对象名称是 'wifiMulti'
PubSubClient mqttClient(espClient);

NTPClient timeClient(espUdp, "ntp1.aliyun.com", 60 * 60 * 8, 30 * 60 * 1000); //

// 引入阿里云 IoT SDK
//#include <AliyunIoTSDK.h>

// 设置产品和设备的信息，从阿里云设备信息里查看(注意一机一密)
#define PRODUCT_KEY "a1RXUd800wl" //固定
#define REGION_ID "cn-shanghai"//固定

#define DEVICE_NAME "esp8266-1"
#define DEVICE_SECRET "178e8e484b40eacec500c901f90c4e59"


/////////////////////TOPIC
//属性上报TOPIC
#define ALINK_TOPIC_PROP_POST "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/event/property/post"
//#define ALINK_TOPIC_PROP_SET "/sys/" PRODUCT_KEY "/" DEVICE_SECRET "/event/property/post_reply"
#define ALINK_TOPIC_PROP_SET "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/service/property/set"

AliyunIOT IOTdeviceClient;

/////////////////////
//unsigned long lastMs = 0;

wifi mywifi;         //定义wifi实例
MAX30205 tempSensor; //定义温度模块实例
MAX30105 spO2Sensor; //定义血氧模块实例
//声明体征参数
float spo2;         //血氧
int32_t heart_rate; //心率
float temperature;  //温度

void mqtt_callback(char *topic, byte *payload, unsigned int length);
void callback(char *topic, byte *payload, unsigned int length);
ICACHE_RAM_ATTR void callBack_Btn1(void);
ICACHE_RAM_ATTR void callBack_Btn2(void);
void mqtt_check_connect();
void mqtt_interval_post(float BodyTemperature);
//void testdrawtemperature(int BPM);
//void drawtime(String time);

//消除抖动 volatitle 定义的变量能在中断中使用
volatile unsigned long triggertime = 0; //按键按下，第一次触发中断的时刻
volatile unsigned long delta = 200;     //去抖期的时长 （毫秒）
//按键选择 会自动清零
volatile int32_t btn_num = 0; //默认为“信息”
volatile boolean btn_click = false;
//控制闪烁
volatile unsigned long triggertime1 = 0; //按键按下，第一次触发中断的时刻
volatile unsigned long delta1 = 4200;    //闪烁时间

//回调函数的变量定义
volatile float spo2_1;
volatile int32_t heart_rate_1;

lasttime ltime;

void setup()
{
  Serial.begin(115200);
  ///////开关初始化设置
  pinMode(BtnPin1, INPUT);        //内部上拉
  pinMode(BtnPin2, INPUT_PULLUP); //设置上拉
  ///////
  Wire.begin();
  timeClient.begin(); //时间
  //u8g2.begin();

  //delay(2000);
  myOLED.Initial();
  //OLED setting
  //初始化wifi
  //mywifi.wifi_Init(myOLED);
  mywifi.wifimanager_Init(myOLED);
  //更新

  IOTdeviceClient.getinfo(mqttClient, PRODUCT_KEY, DEVICE_NAME, DEVICE_SECRET, ALINK_TOPIC_PROP_POST, ALINK_TOPIC_PROP_SET);
  IOTdeviceClient.mqtt_check_connect(myOLED, mqttClient, PRODUCT_KEY, DEVICE_NAME, DEVICE_SECRET, ALINK_TOPIC_PROP_SET);
  //mywifi.wifimanager_Init(myOLED);
  // 初始化 iot，需传入 wifi 的 client，和设备产品信息
  //AliyunIoTSDK::begin(mqttClient, PRODUCT_KEY, DEVICE_NAME, DEVICE_SECRET, REGION_ID);

  mqttClient.setCallback(mqtt_callback); //回调函数
  //mqttClient.setCallback(callback); //回调函数
  attachInterrupt(BtnPin1, callBack_Btn1, FALLING); //当电平发生变化时，触发中断

  attachInterrupt(BtnPin2, callBack_Btn2, FALLING); //当电平发生变化时，触发中断

  //AliyunIoTSDK::bindData("PowerSwitch", powerCallback);
  //温度传感器初始化
  tempSensor.Initial();
  //血氧传感器初始化
  spO2Sensor.Initial();
}

void loop()
{
  myOLED.clearDisplay(); //清屏
  if (WiFi.status() != WL_CONNECTED)
  {
    mywifi.wifi_Init(myOLED);
    //mywifi.wifimanager_Init(myOLED);
  }
  //testdrawtemperature(37);
  //int status=WiFi.status();
  //Serial.println(status);
  //if(status == WL_CONNECTED){
  myOLED.drawtime(timeClient,&ltime); //显示屏第一行 时间

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
  }

  myOLED.drawInfo();
  myOLED.drawMonitoring();
  myOLED.drawConfig();
  //myOLED.drawtan(0);
  /*
  if (millis() - triggertime1 < delta1 * 2 / 7 && millis() - triggertime1 > delta1 * 1 / 7)
  {
    if (btn_num == 0)
      myOLED.drawMonitoring();
    else
      myOLED.drawInfo();
    Serial.println("闪烁1");
  }
  else if (millis() - triggertime1 < delta1 * 4 / 7 && millis() - triggertime1 > delta1 * 3 / 7)
  {
    if (btn_num == 0)
      myOLED.drawMonitoring();
    else
      myOLED.drawInfo();
    Serial.println("闪烁2");
  }
  else if (millis() - triggertime1 < delta1 * 6 / 7 && millis() - triggertime1 > delta1 * 5 / 7)
  {
    if (btn_num == 0)
      myOLED.drawMonitoring();
    else
      myOLED.drawInfo();
    Serial.println("闪烁3");
  }
  else if (millis() - triggertime1 > delta1)
  {
    myOLED.drawInfo();
    myOLED.drawMonitoring();
  }
  else
  {
    myOLED.drawInfo();
    myOLED.drawMonitoring();
  }
  */

  if (millis() - triggertime1 < delta1)
  {
    //闪烁时间
    if (btn_num == 0)
      myOLED.drawtan(0);
    else if(btn_num == 1)
      myOLED.drawtan(31);
    else if(btn_num==2){
      myOLED.drawtan(65);
    }
    Serial.println("闪烁1");
  }
  /*
  else if (millis() - triggertime1 < delta1 / 2 && millis() - triggertime1 > delta1 * 1 / 3)
  {
    myOLED.drawtan(0);
    Serial.println("闪烁2");
  }
  else if (millis() - triggertime1 < delta1 * 5 / 6 && millis() - triggertime1 > delta1 * 2 / 3)
  {
    myOLED.drawtan(0);
    Serial.println("闪烁3");
  }
*/
  myOLED.display(); //显示屏幕
  //}
  //temperature = tempSensor.getTemperature(); // 获取温度
  //Serial.println(temperature);
  //myOLED.drawHR(77);
  /////////////////////////////////阿里云检查连接
  IOTdeviceClient.mqtt_check_connect(myOLED, mqttClient, PRODUCT_KEY, DEVICE_NAME, DEVICE_SECRET, ALINK_TOPIC_PROP_SET);
  IOTdeviceClient.mqtt_loop(mqttClient);
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
    delay(500);
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

    String a = WiFi.macAddress();
    IOTdeviceClient.mqtt_post(mqttClient, temperature, heart_rate, spo2, a.c_str());
  }
}

//开关1中断 用于移动选项
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
        if (btn_num == 1)//触发 配置 后 回到 信息
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

//开关2中断 用于确认选项
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
        else if(btn_num ==1 )
        {
          //测量体征参数
          temperature = tempSensor.getTemperature(); // 获取温度
          PA b = spO2Sensor.getSpO2_test();
          spo2 = b->n_spo2;
          heart_rate = b->n_heart_rate;
        }
        else if(btn_num ==2){
          //设置wifi
          //mywifi.webconfig(myOLED);
          WiFi.mode(WIFI_AP_STA);

        }
        Serial.print("btn_click:");
        Serial.println(btn_click);
      }
    }
  }

  /*
  if (millis() - triggertime > delta)
  {
    triggertime = millis();
    Serial.println("触发了中断2");
    if (btn_num == 1)
    {
      Serial.println("开始测量");
      temperature = tempSensor.getTemperature(); // 获取温度
      PA b = spO2Sensor.getSpO2_test();
      spo2 = b->n_spo2;
      heart_rate = b->n_heart_rate;
    }
    else
    {
      Serial.println("打印info信息");
    }

  }
  */
}
/*
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
*/

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