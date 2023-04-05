#ifndef _ALIYUNIOT_H_
#define _ALIYUNIOT_H_

#include <Arduino.h>
#include <PubSubClient.h>
#include "OLED_SSD1306.h"
class AliyunIOT
{
public:
    void getinfo(
        PubSubClient &mqttClient,
        const char *productKey,
        const char *deviceName,
        const char *deviceSecret,
        const char *topic_prop_post,
        const char *topic_prop_set);
    void mqtt_prepare(
        const char *timestamp,
        const char *productKey,
        const char *deviceName,
        const char *deviceSecret,
        const char *region = "cn-shanghai");
    bool connect_aliyun_mqtt(
        OLED_SSD1306 &myOLED,
        PubSubClient &mqttClient,
        const char *productKey,
        const char *deviceName,
        const char *deviceSecret,
        const char *region = "cn-shanghai");
    /*
    bool connect_aliyun_mqtt_With_password(
        PubSubClient &mqttClient,
        const char *password);
    */
    void mqtt_check_connect(
        OLED_SSD1306 &myOLED,
        PubSubClient &mqttClient,
        const char *productKey,
        const char *deviceName,
        const char *deviceSecret,
        const char *topic_prop_set);
    void mqtt_loop(PubSubClient &mqttClient);

    void mqtt_callback(char *topic, byte *payload, unsigned int length);

    void mqtt_post(PubSubClient &mqttClient, float BodyTemperature, int32_t HeartRate, float SpO2, const char *MACAddress,const char* ALINK_TOPIC_PROP_POST);
};

#endif