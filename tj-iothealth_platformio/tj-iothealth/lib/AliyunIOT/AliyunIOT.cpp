#include "AliyunIOT.h"

#include <SHA256.h>

#define MQTT_PORT 1883
#define SHA256HMAC_SIZE 32

/*
  Aliyun_mqtt.h - Library for connect to Aliyun MQTT server.
*/

// Verify tool: http://tool.oschina.net/encrypt?type=2
static String hmac256(const String &signcontent, const String &ds)
{
  byte hashCode[SHA256HMAC_SIZE];
  SHA256 sha256;

  const char *key = ds.c_str();
  size_t keySize = ds.length();

  sha256.resetHMAC(key, keySize);
  sha256.update((const byte *)signcontent.c_str(), signcontent.length());
  sha256.finalizeHMAC(key, keySize, hashCode, sizeof(hashCode));

  String sign = "";
  for (byte i = 0; i < SHA256HMAC_SIZE; ++i)
  {
    sign += "0123456789ABCDEF"[hashCode[i] >> 4];
    sign += "0123456789ABCDEF"[hashCode[i] & 0xf];
  }

  return sign;
}

static String mqttBroker;
static String mqttClientID;
static String mqttUserName;
static String mqttPassword;

// call this function once
void AliyunIOT::getinfo(PubSubClient &mqttClient, const char *productKey, const char *deviceName, const char *deviceSecret, const char *topic_prop_post, const char *topic_prop_set){
//#define PRODUCT_KEY "a1RXUd800wl"
//#define DEVICE_NAME "esp8266-1"
//#define DEVICE_SECRET "178e8e484b40eacec500c901f90c4e59"
//#define REGION_ID "cn-shanghai"

/////////////////////TOPIC
//属性上报TOPIC
//#define ALINK_TOPIC_PROP_POST "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/event/property/post"
//#define ALINK_TOPIC_PROP_SET "/sys/" PRODUCT_KEY "/" DEVICE_SECRET "/event/property/post_reply"
//#define ALINK_TOPIC_PROP_SET "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/service/property/set"
};

//MQTT连接前的准备工作
void AliyunIOT::mqtt_prepare(const char *timestamp, const char *productKey, const char *deviceName, const char *deviceSecret, const char *region)
{
  mqttBroker = productKey;
  mqttBroker += ".iot-as-mqtt.";
  mqttBroker += String(region);
  mqttBroker += ".aliyuncs.com";

  // Serial.println(mqttBroker);

  mqttUserName = deviceName;
  mqttUserName += '&';
  mqttUserName += productKey;
  //Serial.println(mqttUserName);

  mqttClientID = deviceName; // device name used as client ID
  mqttClientID += "|securemode=3,signmethod=hmacsha256,timestamp=";
  mqttClientID += timestamp;
  mqttClientID += '|';
  //Serial.println(mqttClientID);
};

/*
bool AliyunIOT::connect_aliyun_mqtt_With_password(PubSubClient &mqttClient, const char *password)
{
  mqttClient.setServer(mqttBroker.c_str(), MQTT_PORT);

  byte mqttConnectTryCnt = 5;
  while (!mqttClient.connected() && mqttConnectTryCnt > 0)
  {
    Serial.println("Connecting to MQTT Server ...");
    if (mqttClient.connect(mqttClientID.c_str(), mqttUserName.c_str(), password))
    {

      Serial.println("MQTT Connected!");
      return true;
    }
    else
    {
      byte errCode = mqttClient.state();
      Serial.print("MQTT connect failed, error code:");
      Serial.println(errCode);
      if (errCode == MQTT_CONNECT_BAD_PROTOCOL || errCode == MQTT_CONNECT_BAD_CLIENT_ID || errCode == MQTT_CONNECT_BAD_CREDENTIALS || errCode == MQTT_CONNECT_UNAUTHORIZED)
      {
        Serial.println("No need to try again.");
        break; // No need to try again for these situation
      }
      delay(3000);
    }
    mqttConnectTryCnt -= 1;
  }

  return false;
};
*/

bool AliyunIOT::connect_aliyun_mqtt(
    OLED_SSD1306 &myOLED,
    PubSubClient &mqttClient,
    const char *productKey,
    const char *deviceName,
    const char *deviceSecret,
    const char *region)
{
  String timestamp = String(millis());
  mqtt_prepare(timestamp.c_str(), productKey, deviceName, deviceSecret, region);

  // Generate MQTT Password, use deviceName as clientID
  String signcontent = "clientId";
  signcontent += deviceName;
  signcontent += "deviceName";
  signcontent += deviceName;
  signcontent += "productKey";
  signcontent += productKey;
  signcontent += "timestamp";
  signcontent += timestamp;

  String mqttPassword = hmac256(signcontent, deviceSecret);

  //Serial.print("HMAC256 data: ");
  //Serial.println(signcontent);
  //Serial.print("HMAC256 key: ");
  // Serial.println(deviceSecret);
  // Serial.println(mqttPassword);

  //return connect_aliyun_mqtt_With_password(mqttClient, mqttPassword.c_str());
  mqttClient.setServer(mqttBroker.c_str(), MQTT_PORT);

  byte mqttConnectTryCnt = 5; //连接次数
  while (!mqttClient.connected() && mqttConnectTryCnt > 0)
  {
    ///////OLED
    myOLED.clearDisplay();
    myOLED.setCursor(0, 3); //起点坐标
    myOLED.setTextSize(1);  // Draw 2X-scale text
    myOLED.println("Connecting AliyunIOT");
    myOLED.display();
    delay(1000);
    ///////
    Serial.println("Connecting to MQTT Server ...");
    if (mqttClient.connect(mqttClientID.c_str(), mqttUserName.c_str(), mqttPassword.c_str()))
    {
      ///////OLED
      //myOLED.clearDisplay();
      myOLED.setCursor(0, 12); //起点坐标
      myOLED.setTextSize(1);   // Draw 2X-scale text
      myOLED.println("AliyunIOT Connected!");
      myOLED.display();
      delay(1000);
      ///////
      Serial.println("MQTT Connected!");
      return true;
    }
    else
    {
      byte errCode = mqttClient.state();
      Serial.print("MQTT connect failed, error code:");
      Serial.println(errCode);
      if (errCode == MQTT_CONNECT_BAD_PROTOCOL || errCode == MQTT_CONNECT_BAD_CLIENT_ID || errCode == MQTT_CONNECT_BAD_CREDENTIALS || errCode == MQTT_CONNECT_UNAUTHORIZED)
      {
        Serial.println("No need to try again.");
        break; // No need to try again for these situation
      }
      delay(3000);
    }
    mqttConnectTryCnt -= 1;
  }

  return false;
};

unsigned long lastMs = 0;
void AliyunIOT::mqtt_check_connect(OLED_SSD1306 &myOLED, PubSubClient &mqttClient, const char *productKey, const char *deviceName, const char *deviceSecret, const char *topic_prop_set)
{
  if (millis() - lastMs >= 30000)
  {
    lastMs = millis();
    if (!mqttClient.connected())
    {
      while (connect_aliyun_mqtt(myOLED, mqttClient, productKey, deviceName, deviceSecret))
      {
        Serial.println("MQTT connect succeed!");

        mqttClient.subscribe(topic_prop_set); //这个就是引用开始定义的topic 订阅topic
        Serial.println("subscribe done");
      }
    }
    else
    {
      Serial.println("state is connected");
    }
  }
}

/*
void AliyunIOT::mqtt_callback(char *topic, byte *payload, unsigned int length) //mqtt回调函数“byte *payload”这东西是个指针
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  payload[length] = '\0';

  Serial.println((char *)payload);

  if (strstr(topic, ALINK_TOPIC_PROP_SET))
  {
    AliyunIOT::mqtt_interval_post(mqttClient,37);
  }
}
*/

void AliyunIOT::mqtt_post(PubSubClient &mqttClient, float BodyTemperature, int32_t HeartRate, float SpO2,const char *MACAddress,const char* ALINK_TOPIC_PROP_POST)
{
  char param[512];
  //char jsonBuf[1024];
  sprintf(param, "{\"params\":{\"BodyTemperature\":%.2f,\"HeartRate\":%d,\"SpO2\":%.2f,\"MACAddress\":\"%s\"}}", BodyTemperature,HeartRate,SpO2,MACAddress);
  //sprintf(param, "{\"params\":{\"BodyTemperature\":%.2f,\"HeartRate\":%d}}", BodyTemperature,HeartRate);
  //sprintf(param, "{\"params\":{\"BodyTemperature\":%.2f,\"SpO2\":%.2f}}", BodyTemperature,SpO2);//测试成功
  //sprintf(param, "{\"params\":{\"BodyTemperature\":%.2f,\"HeartRate\":%d,\"SpO2\":%.2f}}", BodyTemperature,HeartRate,SpO2);
  Serial.println(param);
  mqttClient.publish(ALINK_TOPIC_PROP_POST, param); //这个是上传数据的topic,jsonBuf这个是上传的数据
};

void AliyunIOT::mqtt_loop(PubSubClient &mqttClient)
{
  mqttClient.loop();
};