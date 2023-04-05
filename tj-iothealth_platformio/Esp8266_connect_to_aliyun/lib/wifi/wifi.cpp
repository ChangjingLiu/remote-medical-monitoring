#include"wifi.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

static WiFiClient espClient;
ESP8266WiFiMulti wifiMulti; // 建立ESP8266WiFiMulti对象,对象名称是 'wifiMulti'


int tjulogin()
{
    const char *host = "192.168.192.10";
    const int httpPort = 80;

    WiFiClient client; //建立客户端

    if (!client.connect(host, httpPort))
    {
        Serial.println("connection failed"); //成功建立连接
        return 1;
    }
    //Serial.println(" Success!");
    delay(10);
    String postDate = "action=login&username=1751054&password={B}TGNqOTkwNDI5&ac_id=1'&save_me=1&ajax=1"; //将从串口接收的数据发送到服务器，readLine()方法可以自行设计
    if (postDate.length() && postDate != "0")
    {
        String data = (String)postDate;
        int length = data.length();
        String postRequest = (String)("POST ") + "/include/auth_action.php HTTP/1.1\r\n" +
                             "Host: " + host + "\r\n" +
                             "Connection: Keep Alive\r\n" +
                             "Content-Length: " + length + "\r\n" +
                             "Accept: */*\r\n" +
                             "Origin: http://192.168.192.10\r\n" +
                             "Content-Type: application/x-www-form-urlencoded" + "\r\n" +
                             "User-Agent: lcjESP8266\r\n" +
                             "\r\n" +
                             data + "\r\n";
        Serial.println();
        client.print(postRequest); // 向服务器发送HTTP请求
        //Serial.print(postRequest); //打印请求
        //Serial.println("test3");
        delay(600);
        //处理返回信息
        String line = client.readStringUntil('\n');
        //Serial.println("line is");
        //Serial.print(line);
        while (client.available() > 0)
        {
            line += client.readStringUntil('\n');
        }
        //  Serial.println(line);
        client.stop();
        if (line.indexOf("OK") != -1)
        { //认证成功
            return 0;
        }
        else
        {
            return 2;
        }
    }
}

void wifi::wifi_Init()
{
    WiFi.begin();
    wifiMulti.addAP("TJ-GUEST-WIFI", "");          //校园无线网
    wifiMulti.addAP("Tongj.portal", "9901050429"); //手机热点
    wifiMulti.addAP("305", "123456789+");          //宿舍无线网
    wifiMulti.addAP("bgl5605500", "ab990429");     //家庭无线网
    wifiMulti.addAP("bgl5605500-5G", "ab990429");  //家庭无线网

    Serial.println("Connecting ...");
    int i = 0;
    while ((wifiMulti.run() != WL_CONNECTED) && i < 75)
    { //等待
        delay(1000);
        Serial.print(i++);
        Serial.print(' ');
    }
    //对于需要验证的网络进行处理
    if (WiFi.SSID() == "TJ-GUEST-WIFI")
    {
        if (tjulogin() == 0)
        {
            Serial.println("TJU Login Success!");
        }
        else
        {
            Serial.println("TJU Login Fail!");
        }
    }
    delay(500);

    //连接成功后的信息展示
    Serial.println('\n');                         // WiFi连接成功后
    Serial.print("Connected to ");                // NodeMCU将通过串口监视器输出。
    Serial.println(WiFi.SSID());                  // 连接的WiFI名称
    Serial.print("IP address:\t");                // 以及
    Serial.println(WiFi.localIP());               // NodeMCU的IP地址
    Serial.print("ESP8266 Board MAC Address:  "); //
    Serial.println(WiFi.macAddress());            //开发板的mac地址
    return;
}
//针对需要验证的网页