#ifndef _OLED_SSD1306_H_
#define _OLED_SSD1306_H_

/*
#include "OLED_SSD1306.h"
#endif
*/

//#endif
//#include <WiFiUdp.h>//
#include <Adafruit_SSD1306.h>
#include <NTPClient.h> //时钟

//#include "wifi.h" //wifi
//#include "splash.h"

//#include <Wire.h>
//#include <Adafruit_GFX.h>

////////////////////////OLED_setting
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    // Reset pin # (or -1 if sharing Arduino reset pin)

#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

//String weekDays[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
//Month names
//String months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
//中文：年
typedef struct lasttime
{
  int years;
  String monthsStr;
  String daysStr;
  String hoursStr;
  String minuteStr;
};

class OLED_SSD1306 : public Adafruit_SSD1306
{
public:
    OLED_SSD1306(uint8_t w, uint8_t h, TwoWire *twi,
                 int8_t rst_pin);
    /*
    OLED_SSD1306(uint8_t w, uint8_t h, int8_t mosi_pin, int8_t sclk_pin,
                 int8_t dc_pin, int8_t rst_pin, int8_t cs_pin);

    OLED_SSD1306(uint8_t w, uint8_t h, SPIClass *spi, int8_t dc_pin,
                 int8_t rst_pin, int8_t cs_pin, uint32_t bitrate);

    // DEPRECATED CONSTRUCTORS - for back compatibility, avoid in new projects
    OLED_SSD1306(int8_t mosi_pin, int8_t sclk_pin, int8_t dc_pin,
                 int8_t rst_pin, int8_t cs_pin);

    OLED_SSD1306(int8_t dc_pin, int8_t rst_pin, int8_t cs_pin);
    OLED_SSD1306(int8_t rst_pin);
    */
    void Initial(void);
    void drawHeartRate(int HR);
    void drawBodyTemperature(float BodyTemperature);
    void drawSpO2(float SpO2);
    void drawtime(NTPClient timeClient,lasttime *ltime);

    void drawInfo(void);
    void drawMonitoring(void);
    void drawConfig(void);
    void drawtan(int_fast32_t x);
    void drawConfirm(void);
    void drawBack(void);

        ~OLED_SSD1306(void);
    //OLED();
    //~OLED(void);

private:
    //SPIClass *spi;
    TwoWire *wire;
    uint8_t *buffer;
    int8_t i2caddr, vccstate, page_end;
    //int8_t mosiPin, clkPin, dcPin, csPin, rstPin;
};
#endif