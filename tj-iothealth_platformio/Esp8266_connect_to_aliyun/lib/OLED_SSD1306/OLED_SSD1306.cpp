#include "OLED_SSD1306.h"
#include<Arduino.h>

OLED_SSD1306::OLED_SSD1306(uint8_t w, uint8_t h, TwoWire *twi,
                           int8_t rst_pin) : Adafruit_SSD1306(w, h, twi, rst_pin)
{
    //OLED::Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, wire, -1, 400000UL, 100000UL);
}
/*
OLED_SSD1306::OLED_SSD1306(uint8_t w, uint8_t h, int8_t mosi_pin, int8_t sclk_pin,
                           int8_t dc_pin, int8_t rst_pin, int8_t cs_pin) : Adafruit_SSD1306(w, h, mosi_pin, sclk_pin, dc_pin, rst_pin, cs_pin)
{
}

OLED_SSD1306::OLED_SSD1306(uint8_t w, uint8_t h, SPIClass *spi, int8_t dc_pin,
                           int8_t rst_pin, int8_t cs_pin, uint32_t bitrate) : Adafruit_SSD1306(w, h, spi, dc_pin, rst_pin, cs_pin, bitrate)
{
}

// DEPRECATED CONSTRUCTORS - for back compatibility, avoid in new projects
OLED_SSD1306::OLED_SSD1306(int8_t mosi_pin, int8_t sclk_pin, int8_t dc_pin,
                           int8_t rst_pin, int8_t cs_pin) : Adafruit_SSD1306(mosi_pin, sclk_pin, dc_pin, rst_pin, cs_pin)
{
}
OLED_SSD1306::OLED_SSD1306(int8_t dc_pin, int8_t rst_pin, int8_t cs_pin) : Adafruit_SSD1306(dc_pin, rst_pin, cs_pin)
{
}
OLED_SSD1306::OLED_SSD1306(int8_t rst_pin) : Adafruit_SSD1306(rst_pin) {}
*/
/*
OLED_SSD1306::~OLED_SSD1306(void)
{
    if (buffer)
    {
        free(buffer);
        buffer = NULL;
    }
}
*/
/*
OLED::OLED(uint8_t w, uint8_t h, TwoWire *twi,
           int8_t rst_pin, uint32_t clkDuring,
           uint32_t clkAfter)
{
    //OLED::Adafruit_SSD1306()
    OLED::Adafruit_SSD1306(w, h, twi, -1, 400000UL, 100000UL);
}
*/
/*
OLED::~OLED(void)
{
    if (buffer)
    {
        free(buffer);
        buffer = NULL;
    }
}
*/
void OLED_SSD1306::Initial(void)
{

    if (!OLED_SSD1306::begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    {
        Serial.println("SSD1306 allocation failed");
        for (;;)
            //yield();
            ; // Don't proceed, loop forever
    }
    Serial.println("SSD1306 ok");
    //delay(2000);
    OLED_SSD1306::clearDisplay();
    OLED_SSD1306::display();
    //delay(2000);
}

void OLED_SSD1306::drawHR(int HR)
{
    Serial.println("begin to draw");
    OLED_SSD1306::clearDisplay();

    //display.setTextSize(2);             // Normal 1:1 pixel scale
    //display.setTextColor(SSD1306_WHITE);        // Draw white text
    //display.setCursor(0,0);             // Start at top-left corner

    OLED_SSD1306::drawBitmap(0, 0, str_1, 16, 16, 1);  //在坐标X:26  Y:16的位置显示中文字符心
    OLED_SSD1306::drawBitmap(16, 0, str_2, 16, 16, 1); //在坐标X:42  Y:16的位置显示中文字符率

    OLED_SSD1306::setCursor(32, 0); //起点坐标
    OLED_SSD1306::setTextSize(2);   // Draw 2X-scale text
    OLED_SSD1306::setTextColor(SSD1306_WHITE);
    OLED_SSD1306::println(HR);

    OLED_SSD1306::display();
    delay(2000);
    Serial.println("end to draw");
}
void OLED_SSD1306::drawtime(NTPClient timeClient)
{
    unsigned long epochTime = timeClient.getEpochTime(); //获取EpochTime
    struct tm *ptm = gmtime((time_t *)&epochTime);

    OLED_SSD1306::clearDisplay(); //清屏

    OLED_SSD1306::setCursor(0, 0);                    //起点坐标
    OLED_SSD1306::setTextSize(2);                     // Draw 2X-scale text
    OLED_SSD1306::setTextColor(SSD1306_WHITE);        //字体颜色为白色
    OLED_SSD1306::println(ptm->tm_year + 1900);       //年份
    OLED_SSD1306::drawBitmap(12, 0, nian, 16, 16, 1); //在坐标X:26  Y:16的位置显示中文字符 年
    OLED_SSD1306::println(ptm->tm_mon + 1);           //月份
    OLED_SSD1306::drawBitmap(28, 0, nian, 16, 16, 1); //在坐标X:26  Y:16的位置显示中文字符 月
    OLED_SSD1306::println(ptm->tm_mday);              //日
    OLED_SSD1306::drawBitmap(30, 0, nian, 16, 16, 1); //在坐标X:26  Y:16的位置显示中文字符 月

    //int monthDay = ptm->tm_mday;
    //Serial.print("Month day: ");
    //Serial.println(monthDay);
    //int currentMonth = ptm->tm_mon + 1;
    //Serial.print("Month: ");
    //Serial.println(currentMonth);
    //String currentMonthName = months[currentMonth - 1];
    //Serial.print("Month name: ");
    //Serial.println(currentMonthName);
    //int currentYear = ptm->tm_year + 1900;
    //Serial.print("Year: ");
    //Serial.println(currentYear);

    OLED_SSD1306::display();
    delay(2000);
}