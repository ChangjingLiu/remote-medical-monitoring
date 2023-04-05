#ifndef _WIFI_H_
#define _WIFI_H_
/*
#ifndef OLED_SSD1306_H_
#define OLED_SSD1306_H_

#endif
*/
#include <WiFiManager.h> //
#include "OLED_SSD1306.h"


class wifi
{
public:
    //boolean autoConnect(OLED_SSD1306 &myOLED);
    //boolean autoConnect(char const *apName,OLED_SSD1306 &myOLED,char const *apPassword = NULL);
    void wifi_Init(OLED_SSD1306 &myOLED); //所以在对类对象进行参数传递的过程中，采用引用传递是一个正确的方式。
    void wifimanager_Init(OLED_SSD1306 &myOLED);
    void drawInfo(OLED_SSD1306 &myOLED);
    void webconnect(OLED_SSD1306 &myOLED);
    void webconfig(OLED_SSD1306 &myOLED);
    //void webconnect();
    //void configModeCallback (WiFiManager *myWiFiManager);
    //void saveConfigCallback ();
    //void Write_String(int a, int b, String str);
    //String Read_String(int a, int b);
    //private:
    //int tjulogin();
};
#endif