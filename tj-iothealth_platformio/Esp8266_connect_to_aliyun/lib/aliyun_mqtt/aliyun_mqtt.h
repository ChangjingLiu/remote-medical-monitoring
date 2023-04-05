#ifndef _ALIYUN_MATT_H
#define _ALIYUN_MATT_H

#include <Arduino.h>
#include <PubSubClient.h>

/**
 * 连接到阿里云物联网平台。 
 *
 * @param mqttClient: 提供有效的PubSubClient对象（用于网络客户端初始化）。

 * @param productKey: Product Key, 从阿里云物联网平台的产品获取。

 * @param deviceName: Device Name, 从阿里云物联网平台的设备获取。

 * @param deviceSecret: Device Secret, 从阿里云物联网平台的设备获取。
 *
 * @param region: 默认为 "cn-shanghai" 。
 *
 *
 * @return true if connect succeed, otherwise false.
 */
extern "C" bool connect_aliyun_mqtt(
    PubSubClient &mqttClient,
    const char *productKey,
    const char *deviceName,
    const char *deviceSecret,
    const char *region = "cn-shanghai");

/**
 * Two new added APIs are designed for devices with limited resource like Arduino UNO.
 * Since it is hard to calculate HMAC256 on such devices, the calculation can be done externally.
 *
 * These two APIs should be used together with external HMAC256 calculation tools, e.g.
 * http://tool.oschina.net/encrypt?type=2
 * They can be used together to replace connectAliyunMQTT on resource-limited devices.
 */

/**
 * This API should be called in setup() phase to init all MQTT parameters. Since HMAC256
 * calculation is executed extenrally, a fixed timestamp string should be provided, such
 * as "23668" etc. The same timestamp string is also used to calculate HMAC256 result.
 *
 * Other params are similar to them in connectAliyunMQTT.
 */
extern "C" void mqtt_prepare(
    const char *timestamp,
    const char *productKey,
    const char *deviceName,
    const char *deviceSecret,
    const char *region = "cn-shanghai");

/**
 * Use tools here to calculate HMAC256: http://tool.oschina.net/encrypt?type=2
 * The calculated result should be defined as constants and passed when call this function.
 */
extern "C" bool connect_aliyun_mqtt_With_password(PubSubClient &mqttClient, const char *password);

#endif
